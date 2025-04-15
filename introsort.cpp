#include <iostream>
#include <cmath>

const int size_threshold = 16; 

void introsort(int *A, int f, int b);
void Introsort_loop(int* A, int f, int b, int depthLimit);
void insertion_sort(int* A, int f, int b);
int PARTITION(int* A, int p, int r);
int median_of_3(int* A, int f, int b) {
    int mid = f + (b - f) / 2;
    
    if (A[mid] < A[f]) swap(A[mid], A[f]);
    if (A[b - 1] < A[f]) swap(A[b - 1], A[f]);
    if (A[b - 1] < A[mid]) swap(A[b - 1], A[mid]);
    
    return mid;
}
void introsort(int *A, int f, int b){
    Introsort_loop(A, f, b, 2 * floor(log2(b - f)));
    insertion_sort(A, f, b);
}

void Introsort_loop(int* A, int f, int b, int depthLimit) {
    while(b - f > size_threshold){
        if (depthLimit == 0) {
            heap_sort(A, f, b);
            return;
        }
        int p = PARTITION(A, f, median_of_3(A, f, b));
        Introsort_loop(A, p, b, depthLimit);
        f = p + 1;
        depthLimit--;
    }
}
