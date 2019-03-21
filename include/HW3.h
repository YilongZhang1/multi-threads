#ifndef HW3_H__
#define HW3_H__


#include <iostream>     // std::cout
#include <sstream>      // std::istringstream
#include <fstream>
#include <vector>
#include <map>
#include <utility>      // std::pair, std::make_pair
#include <string>
#include <stdlib.h>		/* atoi */
#include <math.h>       /* cos */
#include <limits.h>		// INT_MAX and INT_MIN
#include <chrono>		// timing the processes

#include <unistd.h>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <boost/thread/mutex.hpp>
#include <boost/utility.hpp> // for noncopyable
#include <boost/thread.hpp>

#define COLUMNS 21		// # of columns of the feature data in each line 

using namespace std;

// function prototypes
/**
args: 
	filename: path to file of the csv file;
return: map(key, value), where key = (X, Y); value = vector of 21 features
*/
map<pair<float, float>, vector<float>> csvFileParser(char* filename);

/**
args: dataMap: output from csvFileParser;
return: vector of distance matrices
*/
void argMax_and_distanceMatrix(map<pair<float, float>, vector<float>> dataMap, int threads);


// class prototypes
class SegThreader : boost::noncopyable
{
private:
	// This will be shared on heap by threads:
	float** _dataArray;	
	int _mapRows;
	long long * _elemCount;	// record # of elements in each bucket as well as its index
	float** _buckets;		// store the real data here
	
	// line index we are currently working on
	// threads will work in parallel in a interleave fashion
	int _LineIdx;			// the line a thread will work on
	
	// mutex for dropping data into buckets	
	boost::mutex * _thread_mutex;		
	boost::mutex * _bucket_mutex[21];

	int* _start_row;
	int* _end_row;
	int _threadIdx;
	
public:
	// constructor and destructor
	SegThreader(float** dataArray, float** buckets, int mapRows, int* start_row, int* end_row);
	~SegThreader();
	long long * getCounts();
	void operator()();
};

class DistMatrixThreader : boost::noncopyable
{
private:
	// This will be shared on heap by threads:
	float** _buckets;		// buckets of (X,Y) tuples
	float** _matrices;		// distance matrices
	int* _bucketMat;		// 1st column: bucket size; 2nd column: bucket number
	int _matCnt;			// count of distance matrices
	
	// line index we are currently working on
	// threads will work in parallel in a interleave fashion
	int _LineIdx;			// the line in distance matrix a thread is working on
	int _MatrixIdx;			// the bucket # a thread is working on
	int _StateIdx;			// 1 for right half matrix; 2 for left half matrix;
	// mutex for creating distance matrix
	boost::mutex * _segment_mutex;
	
	int _segment_size;
	
public:
	// constructor and destructor
	DistMatrixThreader(float** buckets, int* bucketMat, int matCnt, int segment_size);
	~DistMatrixThreader();
	float** getMatrices();
	void operator()();
	float Haversine(pair<float, float> M, pair<float, float> N);	
};

class AverageThreader : boost::noncopyable
{
private:
	// This will be shared on heap by threads:
	float** _matrices;		// distance matrices
	int* _bucketMat;		// 1st column: bucket size; 2nd column: bucket number
	int _matCnt;			// count of distance matrices
	float** _lineSum;		// Summation of the elements right to the diagonal 
	
	// line index we are currently working on
	// threads will work in parallel in a interleave fashion
	int _LineIdx;			// the line in distance matrix a thread is working on
	int _LineIdx2;			// _LineIdx is for summing up the lines of each matrix
	int _MatrixIdx;			// the bucket # a thread is working on
	int _MatrixIdx2 = 0;
	
	// mutex for creating distance matrix
	boost::mutex * _segment_mutex;
	boost::mutex * _line_mutex2;
	
	int _segment_size;
	
public:
	// constructor and destructor
	AverageThreader(float** matrices, int* bucketMat, int matCnt, int segment_size);
	~AverageThreader();
	void operator()();	
	float** getAverages();
};

#endif