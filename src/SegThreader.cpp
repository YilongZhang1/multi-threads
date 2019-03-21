#include "HW3.h"
#include <boost/thread/thread.hpp>

SegThreader::SegThreader(float** dataArray, float** buckets, int mapRows, int* start_row, int* end_row)
{
	_dataArray = dataArray;
	_buckets = buckets;
	_mapRows = mapRows;
	
	// allocate memory for elemCount on heap
	_elemCount = new long long[21*2];	
	// initialize _elemCount
	for(int i = 0; i < 21; i++)
	{
		*(_elemCount+2*i) = 0;		// counts in each bucket
		*(_elemCount+2*i+1) = i;		// index of each bucket
	}	

	// then the mutex
	_thread_mutex = new boost::mutex();
	for(int i = 0; i < 21; i++)
		_bucket_mutex[i] = new boost::mutex();
	
	_LineIdx = 0;
	
	_start_row = start_row;
	_end_row = end_row;
	_threadIdx = 0;
}

SegThreader::~SegThreader()
{
	delete[] _elemCount;
	
	delete _thread_mutex;
	for(int i = 0; i < 21; i++)
		delete _bucket_mutex[i];
}

long long* SegThreader::getCounts()
{ 
	return _elemCount;
}

void SegThreader::operator()()
{
/*	// Who am I?
	const boost::thread::id id = boost::this_thread::get_id(); 	
	std::cout <<"\t\t thread_id = "<< id << endl; 
*/
	int start, end;
	// first, give each thread a region of work, defined by start position and end position
	// MUTEX segment
	{
		boost::mutex::scoped_lock lock(*(_thread_mutex));	
		start = *(_start_row+_threadIdx);
		end = *(_end_row+_threadIdx);
		_threadIdx++; // NOTE: If all threads were independent, I could release the mutex here...				
	} // mutex released		
	
	int curIdx = start;
	while (true)
	{
		// read data doesn't need a MUTEX
		if(curIdx > end)
			break;

		float max = *(*(_dataArray+curIdx)+2);	// initialize max to feature0
		int pos = 0;						// pos denotes bucket #
		for(int i = 1; i < 21; i++)
		{
			float curVal = *(*(_dataArray+curIdx)+2+i);
			if( max < curVal )
			{
				max = curVal;
				pos = i;
			}					
		}	
		// now we are clear which bucket to drop our (X,Y) tuple, right?	
		// prepare to drop the data into the right bucket
		// do bucket MUTEX segment
		{
			boost::mutex::scoped_lock lock(*(_bucket_mutex[pos]));		
			// drop (X, Y) into corresponding bucket, denoted by pos
			*( *(_buckets+pos) + (*(_elemCount+2*pos))*2 ) = *(*(_dataArray+curIdx));			// copy X from dataArray
			*( *(_buckets+pos) + (*(_elemCount+2*pos))*2 + 1 ) = *(*(_dataArray+curIdx) + 1);	// copy Y from dataArray
			(*(_elemCount+2*pos))++;		
		} // mutex released	
		curIdx++;
	}	
}

