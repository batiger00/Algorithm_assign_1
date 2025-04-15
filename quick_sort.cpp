#include <iostream>
using namespace std;

int PARTITION(int* A, int p ,int r) {
    int pivotIndex = p + rand() % (r - p + 1); 
    swap(A[pivotIndex], A[r]);

    int x = A[r]; 
    int i = p - 1; 
    for (int j = p; j < r; j++) { 
        if (A[j] <= x) { 
            i++; 
            swap(A[i], A[j]); 
        }
    }
    swap(A[i + 1], A[r]); 
    return i + 1; 
}

void quick_sort(int* A, int p, int r) {
    if (p < r) { 
        int q = PARTITION(A, p, r); 
        quick_sort(A, p, q - 1); 
        quick_sort(A, q + 1, r); 
    }
}
