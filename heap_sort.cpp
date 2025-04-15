#include <iostream>

using namespace std;

void MAXHEAPIFY(int* A, int f, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && A[f + left] > A[f + largest])
        largest = left;
    if (right < n && A[f + right] > A[f + largest])
        largest = right;

    if (largest != i) {
        swap(A[f + i], A[f + largest]);
        MAXHEAPIFY(A, f, n, largest);
    }
}

void BUILDMAXHEAP(int* A, int f, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        MAXHEAPIFY(A, f, n, i);
    }
}

void heap_sort(int* A, int f, int b) {
    int n = b - f;
    BUILDMAXHEAP(A, f, n);
    for (int i = n - 1; i >= 1; i--) {
        swap(A[f], A[f + i]);
        MAXHEAPIFY(A, f, i, 0);
    }
}
