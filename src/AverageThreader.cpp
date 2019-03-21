#include "HW3.h"
#include <boost/thread/thread.hpp>

AverageThreader::AverageThreader(float** matrices, int* bucketMat, int matCnt, int segment_size)
{
	_matrices = matrices;
	_bucketMat = bucketMat;
	_matCnt = matCnt;

	// initialize _lineSum	
	_lineSum = new float*[matCnt];
	for(int i = 0; i < _matCnt; i++)
	{
		*(_lineSum+i) = new float[*(_bucketMat+2*i)];
		for(int j = 0; j < *(_bucketMat+2*i); j++)
			*(*(_lineSum+i) + j) = 0;
	}
	
	_LineIdx = 0;
	_LineIdx2 = 1;
	_MatrixIdx = 0;
	_MatrixIdx2 = 0;
	
	_segment_mutex = new boost::mutex();
	_line_mutex2 = new boost::mutex();
	
	_segment_size = segment_size;
}

AverageThreader::~AverageThreader()
{
	for(int i = 0; i < _matCnt; i++)
		delete[] *(_lineSum+i);
	delete[] _lineSum;
	
	delete _segment_mutex;
	delete _line_mutex2;
}

void AverageThreader::operator()()
{
/*	// Who am I?
	const boost::thread::id id = boost::this_thread::get_id(); 	
	std::cout <<"\t\t thread_id = "<< id << endl; 
*/
	
	while (true)
	{
		int curLine;
		int start;		// including
		int end;		// excluding
		int curMatrix; 
		// do line MUTEX segment
		{
			boost::mutex::scoped_lock lock(*(_segment_mutex));		
			if (_LineIdx > *(_bucketMat+2*_MatrixIdx)-1)
			{
				_LineIdx = 0;
				_MatrixIdx++;
			}	
			start = _LineIdx;
			end = (_LineIdx+_segment_size)<*(_bucketMat+2*_MatrixIdx)-1 ? (_LineIdx+_segment_size) : *(_bucketMat+2*_MatrixIdx)-1;
			curMatrix = _MatrixIdx;						
			_LineIdx += _segment_size; // NOTE: If all threads were independent, I could release the mutex here...				
		} // mutex released
		if(curMatrix >= _matCnt)
			break;
		for(curLine = start; curLine < end; curLine++)
		{		
			int startIdx = curLine;						// including
			int endIdx = *(_bucketMat+2*curMatrix);		// excluding
			int offset = *(_bucketMat+2*curMatrix);	// equals to column size of current matrix
			for(int i = startIdx; i < endIdx; i++)		
			{
				*(*(_lineSum+curMatrix)+curLine) += *(*(_matrices+curMatrix) + curLine*offset + i);			
			}
		}			
	}	

	// now, all the lineSum are calculated
	while (true)
	{
		// do line MUTEX segment
		{
			boost::mutex::scoped_lock lock(*(_line_mutex2));		
			if (_LineIdx2 > *(_bucketMat+2*_MatrixIdx2)-1)
			{
				// go to the next matrix
				_LineIdx2 = 1;
				_MatrixIdx2++;
			}		
			int curLine = _LineIdx2;
			int curMatrix = _MatrixIdx2;
			if(curMatrix >= _matCnt)
				break;			
			_LineIdx2++; // NOTE: If all threads were independent, I could release the mutex here...		
			// sum-up to the first element for each _lineSum
			*(*(_lineSum+curMatrix)) += *(*(_lineSum+curMatrix)+curLine);	
		} // mutex released
	}
}


float** AverageThreader::getAverages()
{
	return _lineSum;
}

















