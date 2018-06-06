//Byung Hun Lee
//CS370 Homework 3

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <vector>

struct range{
	int start;
	int end;
	int value;
};

//function declaration
int calculateT (vector<int> newVector, int threadNum, range T[]);
int calculateM (vector<int> newVector, int threadNum, range M[]);
int calculateI (vector<int> newVector, int threadNum, range I[]);
int calculateF (vector<int> newVector, int threadNum, range F[]);

//main function
int main (int argc, char *argv[]){
	
	//find number of threads, usually specified in command line
	//assume we have enough thread to each thread only looks at 1 or 2 index
	int threadCount = strtol(argv[1], nullptr, 10);
	
	//ourVector will contain the integers
	//since this is pseudocode we can just assume the vectors has some random numbers
	vector <int> ourVector = {some numbers};
	
	//Allocate 4 arrays that will store total, maximal, initial, and final range
	range T[threadCount];
	range M[threadCount];
	range I[threadCount];
	range F[threadCount];
	
	/*Divide the vector until each thread holds a new array with 1 or 2 index
	if array size is odd, last thread will hold 1 index while rest hold 2, if even all thread will hold 2 index
	After dividing, calculate each new vector's T, M , I, F in parallel using nested threads(Assume nested thread is turned on)*/
	if (ourVector.size()%2 == 0){
#		pragma omp parallel num_threads(threadCount)
		{
			int threadNum = omp_get_thread_num();
			vector <int> ourVector = {ourVector[threadNum*2], ourVector[threadNum*2+1]};
#			pragma omp parallel for num_threads(4)
			for (int i = 0; i < 4; i++){
				if (i == 0)
					calculateT (newVector, threadNum, T);
				else if (i == 1)
					calculateM (newVector, threadNum, M);
				else if (i == 2)
					calculateI (newVector, threadNum, I);
				else
					calculateF (newVector, threadNum, F);
			}
		}
	}
	
	else{
#		pragma omp parallel num_threads(threadCount)
		{
			int threadNum = omp_get_thread_num();
			if (omp_get_thread_num() != threadCount-1){
				vector <int> ourVector = {ourVector[threadNum*2], ourVector[threadNumx2*1]};
			}
			else{
				vector <int> ourVector = {ourVector[threadNum*2]};
			}
#			pragma omp parallel for num_threads(4)
			for (int i = 0; i < 4; i++){
				if (i == 0)
					calculateT (newVector, threadNum, T);
				else if (i == 1)
					calculateM (newVector, threadNum, M);
				else if (i == 2)
					calculateI (newVector, threadNum, I);
				else
					calculateF (newVector, threadNum, F);
			}
		}
	}
	
	int loopCounter = 1;
	
	//Loop until only 1 thread remains
	//In each loop, find the new T, M, I, F from merging two vectors
	while (threadCount != 1){
		
#		pragma omp parallel for num_threads(threadCount)
		for (int i = 0; i < threadCount; i++){
			//New T will always be addition of two T
			T[i*loopCounter*2].value = T[i*loopCounter*2].value + T[i+loopCounter].value;
			T[i].start = T[i].start;
			T[i].end = T[i+loopCounter].end;
			
			//New M is from M1, M2, or from combining F1 and I2
			if (M[i].value < M[i+loopCounter].value){
				M[i].value = M[i+loopCounter].value;
				M[i].start = M[i+loopCounter].start;
				M[i].end = M[i+loopCounter].end;
			}
			
			if (M[i].value < F[i].value + I[i+loopCounter].value){
				M[i].value = F[i].value + I[i+loopCounter].value;
				M[i].start = F[i].start;
				M[i].end = I[i+loopCounter].end;
			}
			
			//New I is from I1, I2, or from combining T1 and I2
			if (I[i].value < I[i+loopCounter].value){
				I[i].value = I[i+loopCounter].value;
				I[i].start = I[i+loopCounter].start;
				I[i].end = I[i+loopCounter].end;
			}
			
			if (I[i].value < T[i].value + I[i+loopCounter].value){
				I[i].value = T[i].value + I[i+loopCounter].value;
				I[i].start = T[i].start;
				I[i].end = I[i+loopCounter].end;
			}
			
			//New F is from F1, F2, or from combining F1 and T2
			if (F[i].value < F[i+loopCounter].value){
				F[i].value = F[i+loopCounter].value;
				F[i].start = F[i+loopCounter].start;
				F[i].end = F[i+loopCounter].end;				
			}
			
			if (F[i].value < F[i].value + T[i+loopCounter].value){
				F[i].value = F[i].value + T[i+loopCounter].value;
				F[i].start = F[i].start;
				F[i].end = T[i+loopCounter].end;				
			}
		}

		//variable loopCounter keeps track of which index the array to be merged starts at (index+1, index+2, index+4, etc.)
		//variable threadCount keeps track of whether every array has been merged
		loopCounter = loopCounter * 2;
		threadCount = threadCount/2 + threadCount%1;		
	}
	
	cout << "Max subarray value:" << M[0].value << endl;
	cout << "Max subarray starting index:" << M[0].start << endl;
	cout << "Max subarray ending index:" << M[0].end << endl;
	
	
	//end program
	return 0;
}

//method to calculate T
int calculateT (vector<int> newVector, int threadNum, range T[]){
	
#	pragma omp parallel for num_threads(newVector.size()) reduction(+, T)
	for (int i = 0; i < ourVector.size(); i++){
		T[threadNum].value += ourVector[i];
	}
	T[threadNum].start = threadNumber *2;
	T[threadNum].end = T[threadNum].start + ourVector.size()-1;			
}

//method to calculate M
int calculateM (vector<int> newVector, int threadNum, range M[]){
	
	int currentSum = 0;		
	bool restart = false;
	int currentStart = 0;

	M[threadNum].start = threadNumber *2;
	M[threadNum].end = threadNumber*2;			
	
	for (int i = 0; i < ourVector.size(); i++){
		//add current sum and current value at index
		currentSum = currentSum + ourVector[i];
		//if a value was previously skipped (checked by flag), update start
		if (restart){
			currentStart = i;
			restart = false;
		}
		
		//if new sum exceed current sum, update the value, end, and start
		if (M[threadNum].value < currentSum){
			M[threadNum].value = currentSum;
			M[threadNum].start = M[threadNum].start + currentStart;
			M[threadNum].end = M[threadNum].end + i;
		}
		
		//if current sum is negative, place a flag 
		//flag is used to keep track of where new start index will be
		if (currentSum < 0){
			restart = true;
			currentSum = 0;
		}	
	}	
}

//method to calculate I
int calculateI (vector<int> newVector, int threadNum, range I[]){
	
	int currentSum = 0;		
	int currentStart = 0;

	I[threadNum].start = threadNumber *2;
	I[threadNum].end = threadNumber*2;			
	
	for (int i = 0; i < ourVector.size(); i++){
		//add current sum and current value at index
		currentSum = currentSum + ourVector[i];
		
		//if new sum exceed current sum, update the value, end, and start
		if (I[threadNum].value < currentSum){
			I[threadNum].value = currentSum;
			I[threadNum].end = I[threadNum].end + i;
		}
	}		
}

//method to calculate F
int calculateF (vector<int> newVector, int threadNum, range F[]){
	
	int currentSum = 0;		
	int currentStart = 0;
	
	F[threadNum].start = threadNumber *2;;
	F[threadNum].end = threadNumber*2;		
	
	for (int i = ourVector.size()-1; i > =0; i--){
		//add current sum and current value at index
		currentSum = currentSum + ourVector[i];
		
		//if new sum exceed current sum, update the value, end, and start
		if (F[threadNum].value < currentSum){
			F[threadNum].value = currentSum;
			F[threadNum].start = F[threadNum].end + i;
		}
	}		
}