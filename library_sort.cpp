#include <iostream>
#include <cmath>

using namespace std;

const double EPSILON = 0.5;

void rebalance(int*& S, bool*& filled, int insertedCount, int& sizeS) {
    int newSize = (int)ceil((1 + EPSILON) * insertedCount);
    int* newS = new int[newSize];
    bool* newFilled = new bool[newSize];
    for (int i = 0; i < newSize; i++) {
        newFilled[i] = false;
    }
    
    int gap = newSize / insertedCount;
    int pos = gap / 2;
    
    for (int i = 0; i < sizeS; i++) {
        if (filled[i]) {
            newS[pos] = S[i];
            newFilled[pos] = true;
            pos += gap;
            if (pos >= newSize)
                break;
        }
    }
    
    delete[] S;
    delete[] filled;
    S = newS;
    filled = newFilled;
    sizeS = newSize;
}

void library_sort(int* A, int n) {
    int M = (int)ceil((1 + EPSILON) * n);
    int* S = new int[M];
    bool* filled = new bool[M];
    for (int i = 0; i < M; i++)
        filled[i] = false;
    
    int mid = M / 2;
    S[mid] = A[0];
    filled[mid] = true;
    int inserted = 1;    
    int roundThreshold = 1; 
     
    for (int i = 1; i < n; i++) {
        if (inserted == roundThreshold * 2) {
            rebalance(S, filled, inserted, M);
            roundThreshold *= 2;
        }
        
        int* indices = new int[inserted];
        int idxCount = 0;
        for (int j = 0; j < M; j++) {
            if (filled[j])
                indices[idxCount++] = j;
        }
        
        int lo = 0, hi = idxCount - 1;
        int insertPosIdx = idxCount; 
        while (lo <= hi) {
            int midIdx = lo + (hi - lo) / 2;
            int pos = indices[midIdx];
            if (S[pos] < A[i]) {
                lo = midIdx + 1;
            } else {
                insertPosIdx = midIdx;
                hi = midIdx - 1;
            }
        }
        int target;
        if (insertPosIdx == idxCount)
            target = indices[idxCount - 1] + 1;
        else
            target = indices[insertPosIdx];
        
        if (target >= M)
            target = M - 1;
        
        delete[] indices;
        
        int posFound = -1;
        if (!filled[target]) {
            posFound = target;
        } else {
            int right = target;
            while (right < M && filled[right])
                right++;
            if (right < M)
                posFound = right;
            else {
                int left = target;
                while (left >= 0 && filled[left])
                    left--;
                posFound = left;
            }
        }
        
        if (posFound < 0) {
            rebalance(S, filled, inserted, M);
            int* indices2 = new int[inserted];
            int idxCount2 = 0;
            for (int j = 0; j < M; j++) {
                if (filled[j])
                    indices2[idxCount2++] = j;
            }
            lo = 0; hi = idxCount2 - 1; insertPosIdx = idxCount2;
            while (lo <= hi) {
                int midIdx = lo + (hi - lo) / 2;
                int pos = indices2[midIdx];
                if (S[pos] < A[i]) {
                    lo = midIdx + 1;
                } else {
                    insertPosIdx = midIdx;
                    hi = midIdx - 1;
                }
            }
            if (insertPosIdx == idxCount2)
                target = indices2[idxCount2 - 1] + 1;
            else
                target = indices2[insertPosIdx];
            if (target >= M)
                target = M - 1;
            if (!filled[target])
                posFound = target;
            else {
                int right = target;
                while (right < M && filled[right])
                    right++;
                if (right < M)
                    posFound = right;
                else {
                    int left = target;
                    while (left >= 0 && filled[left])
                        left--;
                    posFound = left;
                }
            }
            delete[] indices2;
        }
        
        if (posFound > target) {
            for (int k = posFound; k > target; k--) {
                S[k] = S[k - 1];
                filled[k] = filled[k - 1];
            }
            S[target] = A[i];
            filled[target] = true;
        } else if (posFound < target) {
            for (int k = posFound; k < target; k++) {
                S[k] = S[k + 1];
                filled[k] = filled[k + 1];
            }
            S[target] = A[i];
            filled[target] = true;
        } else {
            S[target] = A[i];
            filled[target] = true;
        }
        inserted++;
    }
    
    int index = 0;
    for (int i = 0; i < M; i++) {
        if (filled[i]) {
            A[index++] = S[i];
        }
    }
    
    delete[] S;
    delete[] filled;
}

