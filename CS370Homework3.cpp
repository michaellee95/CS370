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
int calculateT (vector<int> newVector, int threadNum)
int calculateM (vector<int> newVector, int threadNum)
int calculateI (vector<int> newVector, int threadNum)
int calculateF (vector<int> newVector, int threadNum)

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
	if array size is odd, last thread will hold 1 index while rest hold 2
	After dividing, calculate each new vector's */
	if (ourVector.size()%2 == 0){
#		pragma omp parallel num_thrads(threadCount)
		vector <int> ourVector = {ourVector[omp_get_thread_num()x2], ourVector[omp_get_thread_num()x2+1]};
		calculateT (vector<int> newVector, int threadNum);
		calculateM (vector<int> newVector, int threadNum);
		calculateI (vector<int> newVector, int threadNum);
		calculateF (vector<int> newVector, int threadNum);
	}
	
	else{
#		pragma omp parallel num_thrads(threadCount)
		if (omp_get_thread_num() != threadCount-1){
			vector <int> ourVector = {ourVector[omp_get_thread_num()x2], ourVector[omp_get_thread_num()x2+1]};
		}
		else{
			vector <int> ourVector = {ourVector[omp_get_thread_num()x2]};
		}
		calculateT (vector<int> newVector, int threadNum);
		calculateM (vector<int> newVector, int threadNum);
		calculateI (vector<int> newVector, int threadNum);
		calculateF (vector<int> newVector, int threadNum);
	}
	
	//end program
	return 0;
}

//method to calculate T
int calculateT (vector<int> newVector, int threadNum){
	
#	pragma omp parallel for num_threads(newVector.size()) reduction(+, T)
	for (int i = 0; i < ourVector.size(); i++){
		T[threadNum].value += ourVector[i];
	}
	T[threadNum].start = 0;
	T[threadNum].end = ourVector.size()-1;			
}

//method to calculate M
int calculateM (vector<int> newVector, int threadNum){
	
	int currentSum = 0;		
	bool restart = false;
	int currentStart = 1;

	M[threadNum].start = 1;
	M[threadNum].end = 1;		
	
	for (int i = 1; i < ourVector.size()-1; i++){
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
int calculateI (vector<int> newVector, int threadNum){
	
	currentSum = 0;	
	restart = false;
	currentStart = 0;
	
	for (int i = 0; i < ourVector.size(); i++){
		//add current sum and current value at index
		currentSum = currentSum + ourVector[i];
		//if a value was previously skipped (checked by flag), update start
		if (restart){
			currentStart = i;
			restart = false;
		}
		
		//if new sum exceed current sum, update the value, end, and start
		if (I[threadNum].value < currentSum){
			I[threadNum].value = currentSum;
			I[threadNum].start = currentStart;
			I[threadNum].end = i;
		}
		
		//if current sum is negative, place a flag 
		//flag is used to keep track of where new start index will be
		if (currentSum < 0){
			restart = true;
			currentSum = 0;
		}	
	}		
}

//method to calculate F
int calculateF (vector<int> newVector, int threadNum){
	
	currentSum = 0;		
	restart = false;
	currentStart = ourVector.size()-1;	
	
	for (int i = ourVector.size()-1; i < ourVector.size(); i--){
		//add current sum and current value at index
		currentSum = currentSum + ourVector[i];
		//if a value was previously skipped (checked by flag), update start
		if (restart){
			currentStart = i;
			restart = false;
		}
		
		//if new sum exceed current sum, update the value, end, and start
		if (F[threadNum].value < currentSum){
			F[threadNum].value = currentSum;
			F[threadNum].start = currentStart;
			F[threadNum].end = i;
		}
		
		//if current sum is negative, place a flag 
		//flag is used to keep track of where new start index will be
		if (currentSum < 0){
			restart = true;
			currentSum = 0;
		}	
	}		
}