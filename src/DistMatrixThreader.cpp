#include "HW3.h"
#include <boost/thread/thread.hpp>
#define PI 3.14159265
#define R 6378137.0		// earth's radius in meter

DistMatrixThreader::DistMatrixThreader(float** buckets, int* bucketMat, int matCnt, int segment_size)
{
	_buckets = buckets;
	_bucketMat = bucketMat;
	_matCnt = matCnt;
	_matrices = new float*[_matCnt];
	for(int i = 0; i < _matCnt; i++)
		_matrices[i] = new float[_bucketMat[2*i] * _bucketMat[2*i]];
	
	_LineIdx = 0;
	_MatrixIdx = 0;		// bucket index for bucketMat, not the original sequence in the 21 buckets
	_StateIdx = 1;
	_segment_mutex = new boost::mutex();
	
	_segment_size = segment_size;
}

DistMatrixThreader::~DistMatrixThreader()
{
	for(int i = 0; i < _matCnt; i++)
		delete[] _matrices[i];
	delete[] _matrices;
	
	delete _segment_mutex;	
}

float** DistMatrixThreader::getMatrices()
{
	return _matrices;
}

void DistMatrixThreader::operator()()
{
/*	// Who am I?
	const boost::thread::id id = boost::this_thread::get_id(); 	
	std::cout <<"\t\t thread_id = "<< id << endl; 
*/
	int curLine;
	int start;	// including 
	int end;	// excluding
	int curMatrix;
	int curState;
	while (true)
	{
		// do line MUTEX segment
		{
			boost::mutex::scoped_lock lock(*(_segment_mutex));		
			if (_LineIdx > *(_bucketMat+2*_MatrixIdx)-1)
			{
				if(_StateIdx == 1)
				{
					_StateIdx = 2;
					_LineIdx = 0;
				}				
				else if(_StateIdx == 2)
				{
					_MatrixIdx++;					
					_StateIdx = 1;
					_LineIdx = 0;
				}
			}		
			start = _LineIdx;
			end = (_LineIdx+_segment_size)<*(_bucketMat+2*_MatrixIdx)-1 ? (_LineIdx+_segment_size) : *(_bucketMat+2*_MatrixIdx)-1;
			curMatrix = _MatrixIdx;
			curState = _StateIdx;
			if(curMatrix >= _matCnt)
				break;				
			_LineIdx += _segment_size; // NOTE: If all threads were independent, I could release the mutex here...				
		} // mutex released

		// now we are clear which distance matrix and which line of the distance matrix we are working on
		// first, calculate the right half of the matrix
		if(curState == 1)
		{
			for(curLine = start; curLine < end; curLine++)
			{
				int startIdx = curLine;						// including
				int endIdx = *(_bucketMat+2*curMatrix);		// excluding
				int offset = *(_bucketMat+2*curMatrix);		// equals to column size of current matrix
				
				pair<float, float> point1 (*(*(_buckets + *(_bucketMat+2*curMatrix+1))+2*curLine), *(*(_buckets + *(_bucketMat+2*curMatrix+1))+2*curLine+1));			
				for(int i = startIdx; i < endIdx; i++)
				{
					pair<float, float> point2 (*(*(_buckets + *(_bucketMat+2*curMatrix+1))+2*i), *(*(_buckets + *(_bucketMat+2*curMatrix+1))+2*i+1));
					*(*(_matrices+curMatrix) + curLine*offset + i) = Haversine(point1, point2);
				}				
			}			
		}
		else if(curState == 2) // just copy the diagonal mirroring data  
		{
			for(curLine = start; curLine < end; curLine++)
			{			
				int startIdx = 0;						// including
				int endIdx = curLine;					// excluding
				int offset = *(_bucketMat+2*curMatrix);	// equals to column size of current matrix

				for(int i = startIdx; i < endIdx; i++)
					*(*(_matrices+curMatrix) + curLine*offset + i) = *(*(_matrices+curMatrix) + i*offset + curLine);
			}
		}
	}	
}


/**
args: Buckets: output from argMax;
return: vector of distance matrices
*/
float DistMatrixThreader::Haversine(pair<float, float> M, pair<float, float> N)
{
	float M_lon = M.first*PI/180, M_lat = M.second*PI/180;
	float N_lon = N.first*PI/180, N_lat = N.second*PI/180;
	return 2.0*R*asin(sqrt(pow(sin((M_lat-N_lat)/2.0), 2) + cos(M_lat)*cos(N_lat)*pow(sin((M_lon-N_lon)/2.0), 2)));
}









