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
			vector <int> ourVector = {ourVector[threadNumx2], ourVector[threadNumx2+1]};
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
				vector <int> ourVector = {ourVector[threadNumx2], ourVector[threadNumx2+1]};
			}
			else{
				vector <int> ourVector = {ourVector[threadNumx2]};
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
	
	int loopCount = threadCount/2 + threadCount%1;
	
	while (loopCount != 1){
		
	}
	
	//end program
	return 0;
}

//method to calculate T
int calculateT (vector<int> newVector, int threadNum, range T[]){
	
#	pragma omp parallel for num_threads(newVector.size()) reduction(+, T)
	for (int i = 0; i < ourVector.size(); i++){
		T[threadNum].value += ourVector[i];
	}
	T[threadNum].start = 0;
	T[threadNum].end = ourVector.size()-1;			
}

//method to calculate M
int calculateM (vector<int> newVector, int threadNum, range M[]){
	
	int currentSum = 0;		
	bool restart = false;
	int currentStart = 0;

	M[threadNum].start = 0;
	M[threadNum].end = 0;		
	
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
			M[threadNum].start = currentStart;
			M[threadNum].end = i;
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

	I[threadNum].start = 0;
	I[threadNum].end = 0;		
	
	for (int i = 0; i < ourVector.size(); i++){
		//add current sum and current value at index
		currentSum = currentSum + ourVector[i];
		
		//if new sum exceed current sum, update the value, end, and start
		if (I[threadNum].value < currentSum){
			I[threadNum].value = currentSum;
			I[threadNum].end = i;
		}
	}		
}

//method to calculate F
int calculateF (vector<int> newVector, int threadNum, range F[]){
	
	int currentSum = 0;		
	int currentStart = 0;
	
	F[threadNum].start = 0;
	F[threadNum].end = ourVector.size()-1;		
	
	for (int i = ourVector.size()-1; i > =0; i--){
		//add current sum and current value at index
		currentSum = currentSum + ourVector[i];
		
		//if new sum exceed current sum, update the value, end, and start
		if (F[threadNum].value < currentSum){
			F[threadNum].value = currentSum;
			F[threadNum].start = i;
		}
	}		
}