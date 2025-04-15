#include <iostream>

using namespace std;

void cocktail_shaker_sort(int* A, int n) {
    bool swapped = true;
    int start = 0, end = n - 1;
    while (swapped) {
        swapped = false;

        for (int j = start; j < end; j++) {
            if (A[j] > A[j + 1]) {
                swap(A[j], A[j + 1]);
                swapped = true;
            }
        }
        end--;

        if (!swapped)
            break;

        swapped = false;
        for (int j = end; j > start; j--) {
            if (A[j] < A[j - 1]) {
                swap(A[j], A[j - 1]);
                swapped = true;
            }
        }
        start++;
    }
}
