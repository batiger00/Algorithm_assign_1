#include <iostream>
using namespace std;
void comb_sort(int* A, int n){
    int gap = n;
    float shrink = 1.3;
    bool swapped = true;
    while(gap>1||swapped){
        gap = gap/shrink;
        if(gap < 1)
            gap = 1;
        swapped = false;
        for(int i=0;i<n-gap ; i++){
            if(A[i] > A[i + gap]){
                swap(A[i],A[i + gap]);
                swapped = true;
            }
        }
    }
}
