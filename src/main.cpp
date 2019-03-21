#include "HW3.h"

/** boot up the program here */

int main( int argc, char * args[] )
{
	if( argc < 3 ) 
	{
		printf("Error: incorrect count of parameters.\n");
		printf("Please input in this format: multi_threads [number of threads] [the input file]\n");
		return 0;
	}
	
	int threads = atoi(args[1]);
	if(threads != 1 && threads != 2 && threads != 4 && threads != 6 && threads != 8)
	{
		printf("Please check if your thread number is one of these {1,2,4,6,8}\n");
		return 0;
	}
	// parsing by calling the csvFileParser function
	cout<<endl<<"---------------------------------------------------------------------------"<<endl;
	cout<<"Threads count: "<<threads<<", tackle the file: "<<args[2]<<endl;
	map<pair<float, float>, vector<float>> dataMap = csvFileParser(args[2]);
	// segment the data into 21 buckets and create the distance matrices for all of them
	argMax_and_distanceMatrix(dataMap, threads);

	return 0;
}
	
	

	
	
	
	
	