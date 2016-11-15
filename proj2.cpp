//*****************************************************************************
// David Poss
// CPSC 335
// Professor Bein
// Project 2
// ****************************************************************************

/* Program Description:
Finds the longest increasing subsequence for a list of random integers.
The two methods are a simple O(n^2) algorithm, and one where we generate an
exhaustive powerset, and find the longest element in it.
*/

#define MAX_NUM 10   //Max number produced by random number generator
#include <chrono>
#include <iostream>
#include <time.h>
#include <cstdlib>

void print(int*, int);                       // Prints an array
void printPowerset(int, int*, int&, int*);   // Gets powerset of array
void checkSet(int[], int, int*, int&, int*); // Compares two power sets' length
void generateArray(int*, int);               // Generates random n sized array 
void simple_algorithm(int*, int);            // Algorithm 1
void powerset_algorithm(int*, int);          // Algorithm 2

int main() {

    std::cout << "Enter list size: ";
    int n;                //Size of list
    std::cin >> n;
    int* A = new int[n];  // List to work with
    generateArray(A, n);  // Randomly generated array of size n
    //Algorithm 1
    std::cout << "\nSimple Algorithm\n";
    auto start = std::chrono::high_resolution_clock::now(); //start of timing
    simple_algorithm(A, n);
    auto end = std::chrono::high_resolution_clock::now();   //end of timing
    int microseconds = int(std::chrono::duration_cast<
            std::chrono::microseconds>(end - start).count());
    double seconds = microseconds / 1E7;                   //Convert to seconds
    std::cout << "Running time: " << seconds << " seconds.\n";
    //Algorithm 2
    std::cout << "\nExhaustive Powerset Algorithm\n";
    start = std::chrono::high_resolution_clock::now();
    powerset_algorithm(A, n);
    end = std::chrono::high_resolution_clock::now();
    microseconds = int(std::chrono::duration_cast<
            std::chrono::microseconds>(end - start).count());
    seconds = microseconds / 1E6;
    std::cout << "Running time: " << seconds << " seconds.\n" << std::endl;

    delete[] A;
    return 0;
}

void print(int arr[], int n) {
    std::cout << "   Sequence is: ";
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}// end of print

void generateArray(int *A, int n) {
    srand(unsigned(time(NULL)));       
    for (int i = 0; i < n; i++) {       
        A[i] = rand() % (MAX_NUM + 1);   
    }
}

void printPowerset(int n, int *bestSet, int &bestSize, int *A) {
    int *stack, k;
    stack = new int[n + 1];
    stack[0] = 0;
    k = 0;
    while (1) {        
        if (stack[k] < n) {
            stack[++k] = stack[k - 1] + 1;
        }
        else{
            stack[--k]++;
        }
        if (k == 0) {
            break;
        }
        checkSet(stack, k, bestSet, bestSize, A);
    }
    delete[] stack;
    return;
}//end of printPowerset

void checkSet(int* stack, int k, int *bestSet, int& bestSize, int* A) {
    if ((k == 1) && (k >= bestSize)) {
        bestSet[0] = stack[1];
        bestSize = k;
        return;
    }
    if (k < 2 && k > bestSize) {
        bestSet[0] = stack[0 + 1] - 1;
        bestSize = k;
        return;
    }
    else { //check if stack is in non-decreasing order
        for (int i = 0; i < k - 1; i++) {
            if (A[stack[i + 1] - 1] >= A[stack[i + 2] - 1]) {
                return; //elements are not in increasing order
            }
        }
    }
    if (k > bestSize) {
        for (int i = 0; i < k; i++) {
            bestSet[i] = stack[i + 1] ;
        }
        bestSize = k;
        return;
    }
}//end of checkSet

void simple_algorithm(int* A, int n) {
    int* H = new int[n];
    for (int i = 0; i < n; i++) {
        H[i] = 0;
    }
    print(A, n);  //Display list we're working with

    for (int i = n - 2; i >= 0; i--) {          
        for (int j = i + 1; j < n; j++) {        
            if ((A[j] > A[i]) && (H[j] >= H[i])) {
                H[i] = H[j] + 1;                   
            }
        }
    }
    int max = H[0];                   
    for (int i = 1; i < n; i++) {      
        if (max <= H[i]) {               
            max = H[i];                    
        }
    }
    max++;                                   

    int j = 0;                                
    int index = max - 1;                       
    int* subsequence = new int[max + 1];        

    for (int i = 0; i < n; i++) {                
        if (H[i] == index) {                    
            subsequence[j] = A[i];               
            index--;                              
            j++;                                   
        }
    }
    std::cout << "   Longest increasing subsequence: " << max << std::endl;
    print(subsequence, max);                                                

    delete[] subsequence;
    delete[] H;                                                             
    return;
}//end of simple_algorithm

void powerset_algorithm(int* A, int n) {
    int* R = new int[n];
    for (int i = 0; i < n; i++) {
        R[i] = 0;
    }
    print(A, n);

    int bestSize = 0;
    int* bestSet = new int[n];
    printPowerset(n, bestSet, bestSize, A);
    std::cout << "   Longest increasing subsequence: "
        << bestSize << std::endl;
    R[0] = A[0];
    for (int i = 0; i < bestSize; i++) {
        R[i] = A[bestSet[i] - 1];
    }
    print(R, bestSize);

    delete[] R;
    delete[] bestSet;
}//end of powerset_algorithm
