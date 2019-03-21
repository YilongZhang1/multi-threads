#include "HW3.h"


// since it's hard to have random access to data in a map, how about we first convert it to a 2D array?
float** convert_map_array(map<pair<float, float>, vector<float>> dataMap)
{
	int mapRows = dataMap.size();
	float** dataArray = new float*[mapRows];
	for(int i = 0; i < mapRows; i++)
		*(dataArray+i) = new float[23];		// format in a row: X, Y, feature0,..., feature21
	
	int rowCnt = 0;
	for (map<pair<float, float>, vector<float>>::iterator iter = dataMap.begin(); iter != dataMap.end(); iter++)
	{
		*(*(dataArray+rowCnt)) = iter->first.first;
		*(*(dataArray+rowCnt)+1) = iter->first.second;
		vector<float> curVec = iter->second;	// access the current vector
		for(int i = 0; i < 21; i++)
			*(*(dataArray+rowCnt)+i+2) = curVec.at(i);
		rowCnt++;
	}	
	return dataArray;
	// remember to free dataArray after usage by calling clear_dataArray
}


/**
args:
	dataMap: output from csvFileParser;
	threads: number of threads in calculating argmax;
return: none
*/
void argMax_and_distanceMatrix(map<pair<float, float>, vector<float>> dataMap, int threads)
{
	// to traverse part of the map, we have to first convert it to an array
	float** dataArray = convert_map_array(dataMap);
	auto start1 = std::chrono::system_clock::now();	// start ticking
	cout<<"---------------------------------------------------------------------------"<<endl;
	cout<<endl<<"Now segment the (X, Y) tuples into 21 buckets based on argmax...\n";	
	
	// data size in shared memory
	int mapRows = dataMap.size();
	int bucket_size = mapRows * 2;
	float** buckets = new float*[21];
	// then allocate memory for each pointer of buckets
	for(int i = 0; i < 21; i++)
		buckets[i] = new float[bucket_size];
	
	// start and end row index for each thread, both are including
	int* start_row = new int[threads];
	int* end_row = new int[threads];
	int interval = mapRows / threads;
	
	*(start_row) = 0;
	for(int s = 1; s < threads; s++)
	{
		*(start_row+s) = interval*s;
		*(end_row+s-1) = interval*s - 1;
	}
	*(end_row+threads-1) = mapRows-1;	
	
	cout<<"\t spawn "<<threads<<" thread(s) and start segmenting data into buckets...\n";	
	SegThreader segThreads(dataArray, buckets, mapRows, start_row, end_row);
	// Create a thread group	
	boost::thread_group tg1;
	// For the number of threads requested, create a thread passing in the reference to the callable object
	for (int t = 0; t < threads ; ++t)
		tg1.create_thread( boost::ref(segThreads) );			

	tg1.join_all();
	cout<<"\t segmentation work done!"<<endl;
	
	auto end1 = std::chrono::system_clock::now();	// stop ticking
	std::chrono::duration<double> elapsed_seconds = end1-start1;	
	
	long long* elemCount = segThreads.getCounts();

	cout<<"SUMMARY:\n";	
	cout<<"\t Elapsed time in segmenting data into buckets: " << elapsed_seconds.count() << " s\n";
	cout<<"\t # of (X, Y) tuples in each bucket are: \n";
	int total = 0;
	for(int i = 0; i < 21; i++)
	{
		total += *(elemCount+2*i);
		cout<<"\t\t bucket "<<std::dec<<i<<":\t"<<(int)*(elemCount+2*i)<<endl;
	}
	cout<<"\t\t Total: "<<total<<endl;
		
	// free dataArray
	for(int i = 0; i < mapRows; i++)
		delete[] *(dataArray+i);
	delete[] dataArray;
	delete[] start_row;
	delete[] end_row;



	
	/// generate distance matrices. Only compute distance matrix for buckets less than 5001 numbers.
	auto start2 = std::chrono::system_clock::now();	// start ticking
	cout<<endl<<"---------------------------------------------------------------------------"<<endl;	
	cout<<endl<<"Now calculate distance matrices for (X, Y) tuples in the following buckets...\n";
	
	int countDM = 0;
	for(int i = 0; i < 21; i++)
		if( *(elemCount+2*i) < 5001 && *(elemCount+2*i) > 1)
			countDM++;
	// now got the number of buckets with less than 5001 elements
	int* bucketMat = new int[2*countDM];
	countDM = 0;
	for(int i = 0; i < 21; i++)
		if(  *(elemCount+2*i) < 5001 && *(elemCount+2*i) > 1 )
		{
			bucketMat[2*countDM] = *(elemCount+2*i);
			bucketMat[2*countDM+1] = i;
			cout<<"\t bucket "<<i<<" -- size "<<(int)bucketMat[2*countDM]<<endl;
			countDM++;
		}				

	int segment_size = 100;
	DistMatrixThreader distMatrixThreads(buckets, bucketMat, countDM, segment_size);
	// Create a thread group	
	boost::thread_group tg2;
	// For the number of threads requested, create a thread passing in the reference to the callable object
	cout<<"\t spawn "<<threads<<" thread(s) and start generating distance matrices...\n";
	for (int t = 0; t < threads ; ++t)	
		tg2.create_thread( boost::ref(distMatrixThreads) );			
	
	tg2.join_all();
	cout<<"\t distance matrices generated!"<<endl;
	
	auto end2 = std::chrono::system_clock::now();	// stop ticking
	elapsed_seconds = end2-start2;		
	cout<<"SUMMARY:\n";	
	cout<<"\t Elapsed time in generating distance matrix: " << elapsed_seconds.count() << " s\n";	
	float** matrices = distMatrixThreads.getMatrices();

	// free the buckets
	for(int i = 0; i < 21; i++)
		delete[] *(buckets+i);
	delete[] buckets;

	
	
	
		
	/// calculate average for each distance matrix
	auto start3 = std::chrono::system_clock::now();	// start ticking
	cout<<endl<<"---------------------------------------------------------------------------"<<endl;	
	cout<<endl<<"Now calculate average for each distance matrix...\n";
	cout<<"\t spawn "<<threads<<" thread(s) and start calculating averages...\n";

	int segment_size2 = 100;	
	AverageThreader averageThreads(matrices, bucketMat, countDM, segment_size2);
	// Create a thread group	
	boost::thread_group tg3;
	// For the number of threads requested, create a thread passing in the reference to the callable object
	for (int t = 0; t < threads ; ++t)	
		tg3.create_thread( boost::ref(averageThreads) );

	tg3.join_all();
	cout<<"\t average of distance matrices calculated!"<<endl;
	auto end3 = std::chrono::system_clock::now();	// stop ticking
	elapsed_seconds = end3-start3;		
	cout<<"SUMMARY:\n";	
	cout<<"\t Elapsed time in calculating averages: " << elapsed_seconds.count() << " s\n";	
	float** lineSum = averageThreads.getAverages();
	for(int i = 0; i < countDM; i++)
	{
		float avg = *(*(lineSum+i)) * 2 / pow(*(bucketMat+2*i), 2);
		cout<< "\t average of distance matrix for bucket "<<std::dec<<*(bucketMat+2*i+1)<<" is "<<avg<<endl;
	}	

	// free bucketMat
	delete[] bucketMat;
}





















