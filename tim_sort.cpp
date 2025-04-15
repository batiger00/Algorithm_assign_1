#include <iostream>
#include <algorithm>
using namespace std;

void insertion_sort(int* A, int f, int b);
void tim_sort(int* A, int n);
int count_run_and_make_ascending(int* A, int start, int n);
int compute_minrun(int n) {
    int r = 0;
    while (n >= 64) {
        r |= (n & 1);
        n >>= 1;
    }
    return n + r;
}
void tim_sort(int*A, int n) {
    int block_size = compute_minrun(n); 
    int left = 0;
    int size = block_size;
    int partial_size = 0;



    while(left < n) {
        partial_size = count_run_and_make_ascending(A, left, n);
        int run_len = partial_size;
    
        if (run_len < block_size) {
            int forced_end = min(left + block_size, n);
            insertion_sort(A, left, forced_end);
            run_len = forced_end - left;
        }
    
        left += run_len; 
    }
    
    while(size < n){
        for(left = 0; left < n; left += 2*size){
            int mid = min(left + size - 1, n - 1);
            int right = min(left + 2*size - 1, n-1);
            if (mid < right) {
                MERGE(A, left, mid, right);
            } else if (mid == right && mid < n - 1) {
                MERGE(A, left, mid, n - 1); 
            }
        }
        size *= 2;
    }
    merge_sort(A, 0, n - 1);

}

int count_run_and_make_ascending(int* A, int start, int n) {
    int run_end = start + 1;
    if (run_end == n) return 1;

    if (A[run_end] < A[start]) {
        while (run_end < n && A[run_end] < A[run_end - 1]) run_end++;
        reverse(A + start, A + run_end);
    } else {
        while (run_end < n && A[run_end] >= A[run_end - 1]) run_end++;
    }

    return run_end - start;
}