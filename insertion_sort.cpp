#include <iostream>

using namespace std;

void insertion_sort(int* A, int f, int b) {
    for (int j = f + 1; j < b; j++){
        int key = A[j]; 
        int i = j - 1;
        while (i >= f && A[i] > key) {
            A[i + 1] = A[i];
            i--;
        }
        A[i + 1] = key;
    }
}
