#include <iostream>

using namespace std;

void bubble_sort(int* A, int n) {
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false; 
        for (int j = 0; j < n - i - 1; j++) {
            if (A[j] > A[j + 1]) {
                swap(A[j], A[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            break;
        }
    }
}
