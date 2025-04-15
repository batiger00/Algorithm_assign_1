#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <map>
#include <tuple>
#include <functional>
#include <set>
#include <unistd.h>  // sysconf(_SC_PAGE_SIZE) 사용

using namespace std;
namespace fs = std::filesystem;

// 1. 정렬 알고리즘 헤더 포함
#include "algorithms.h"
// 2. 메모리 사용 측정 (리눅스 /proc/self/statm 기반, 단순 예시)
size_t getCurrentMemoryUsage() {
    ifstream statm("/proc/self/statm");
    if (!statm) return 0;
    size_t totalPages, residentPages;
    statm >> totalPages >> residentPages;
    statm.close();
    long pageSize = sysconf(_SC_PAGE_SIZE);
    return residentPages * pageSize; // 단위: byte
}
// 3. 파일 이름 파싱 및 추가 정보 추출
struct FileInfo {
    string pattern;
    int dataSize;
    int fileIndex;
};

int getFileIndex(const string& filename) {
    vector<string> tokens;
    istringstream iss(filename);
    string token;
    while(getline(iss, token, '_')) {
        tokens.push_back(token);
    }
    if(tokens.size() >= 3) {
        string idxStr = tokens[2];
        size_t pos = idxStr.find(".");
        if(pos != string::npos)
            idxStr = idxStr.substr(0, pos);
        try {
            return stoi(idxStr);
        } catch(...) { return 0; }
    }
    return 0;
}

FileInfo parseFileName(const string& filename) {
    FileInfo info;
    vector<string> tokens;
    istringstream iss(filename);
    string token;
    while(getline(iss, token, '_')) {
        tokens.push_back(token);
    }
    if(!tokens.empty()) {
        info.pattern = tokens[0];
    }
    if(tokens.size() >= 2) {
        try {
            info.dataSize = stoi(tokens[1]);
        } catch(...) {
            info.dataSize = 0;
        }
    }
    info.fileIndex = getFileIndex(filename);
    return info;
}

int patternOrder(const string& pattern) {
    if(pattern == "ascending") return 0;
    if(pattern == "descending") return 1;
    if(pattern == "random") return 2;
    if(pattern == "partial") return 3;
    return 4;
}

bool fileComparator(const fs::directory_entry& a, const fs::directory_entry& b) {
    string fnameA = a.path().filename().string();
    string fnameB = b.path().filename().string();
    FileInfo infoA = parseFileName(fnameA);
    FileInfo infoB = parseFileName(fnameB);
    
    int orderA = patternOrder(infoA.pattern);
    int orderB = patternOrder(infoB.pattern);
    if(orderA != orderB)
        return orderA < orderB;
    if(infoA.dataSize != infoB.dataSize)
        return infoA.dataSize < infoB.dataSize;
    return infoA.fileIndex < infoB.fileIndex;
}
// 4. 정렬 함수 래퍼 설정
using SortFunction = function<vector<int>(const vector<int>&)>;

vector<pair<string, SortFunction>> algorithms = {
    {"tim_sort", [](const vector<int>& data) -> vector<int> {
        vector<int> copy = data;
        tim_sort(copy.data(), copy.size());
        return copy;
    }},
    {"introsort", [](const vector<int>& data) -> vector<int> {
        vector<int> copy = data;
        if (!copy.empty())
            introsort(copy.data(), 0, copy.size());
        return copy;
    }},
    {"quick_sort", [](const vector<int>& data) -> vector<int> {
        vector<int> copy = data;
        if (!copy.empty())
            quick_sort(copy.data(), 0, copy.size() - 1);
        return copy;
    }},
    {"merge_sort", [](const vector<int>& data) -> vector<int> {
        vector<int> copy = data;
        if (!copy.empty())
            merge_sort(copy.data(), 0, copy.size() - 1);
        return copy;
    }},
    {"heap_sort", [](const vector<int>& data) -> vector<int> {
        vector<int> copy = data;
        if (!copy.empty())
            heap_sort(copy.data(), 0, copy.size());
        return copy;
    }},
    {"library_sort", [](const vector<int>& data) -> vector<int> {
        vector<int> copy = data;
        library_sort(copy.data(), copy.size());
        return copy;
    }},
    {"tournament_sort", [](const vector<int>& data) -> vector<int> {
        vector<int> copy = data;
        return tournament_sort(copy);
    }},
    {"comb_sort", [](const vector<int>& data) -> vector<int> {
        vector<int> copy = data;
        comb_sort(copy.data(), copy.size());
        return copy;
    }},
    {"insertion_sort", [](const vector<int>& data) -> vector<int> {
        vector<int> copy = data;
        if (!copy.empty())
            insertion_sort(copy.data(), 0, copy.size());
        return copy;
    }},
    {"cocktail_shaker_sort", [](const vector<int>& data) -> vector<int> {
        vector<int> copy = data;
        cocktail_shaker_sort(copy.data(), copy.size());
        return copy;
    }},
    {"selection_sort", [](const vector<int>& data) -> vector<int> {
        vector<int> copy = data;
        selection_sort(copy.data(), copy.size());
        return copy;
    }},
    {"bubble_sort", [](const vector<int>& data) -> vector<int> {
        vector<int> copy = data;
        bubble_sort(copy.data(), copy.size());
        return copy;
    }}
};

// 타임아웃 기준 (10분 = 600초)
const double TIMEOUT_SECONDS = 600.0;

int main() {
    string folder = "./dataset/sorting_datasets";

    vector<fs::directory_entry> files;
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.path().extension() == ".txt") {
            files.push_back(entry);
        }
    }
    sort(files.begin(), files.end(), fileComparator);

    for (auto& algPair : algorithms) {
        string algName = algPair.first;
        SortFunction sortFunc = algPair.second;

        struct Stats {
            double totalTime = 0.0;
            size_t totalMemory = 0;
            int count = 0;
        };
        map<tuple<string, int>, Stats> algResults;
        set<tuple<string, int>> timeoutGroups;

        for (const auto& entry : files) {
            string filename = entry.path().filename().string();
            FileInfo info = parseFileName(filename);
            auto groupKey = make_tuple(info.pattern, info.dataSize);

            if ((info.dataSize == 500000 || info.dataSize == 1000000) &&
                timeoutGroups.find(groupKey) != timeoutGroups.end()) {
                cout << "[SKIP] (" << algName << ") Skipping " << filename
                     << " because group (" << info.pattern << ", " << info.dataSize
                     << ") timed out previously." << endl;
                continue;
            }

            cout << "[LOG] (" << algName << ") Processing " 
                 << info.pattern << " dataset (size " << info.dataSize 
                 << ") for file " << filename << endl;

            ifstream fin(entry.path().string());
            if (!fin) continue;
            vector<int> data;
            int x;
            while (fin >> x) {
                data.push_back(x);
            }
            fin.close();

            size_t memBefore = getCurrentMemoryUsage();
            auto startTime = chrono::high_resolution_clock::now();

            vector<int> sorted = sortFunc(data);

            auto endTime = chrono::high_resolution_clock::now();
            size_t memAfter = getCurrentMemoryUsage();

            vector<int> expected = data;
            sort(expected.begin(), expected.end());
            if (sorted != expected) {
                cerr << "[ERROR] Sorting FAILED for " << algName 
                     << " on file " << filename << endl;
            }

            double elapsedTime = chrono::duration<double>(endTime - startTime).count();
            size_t memUsed = (memAfter > memBefore) ? (memAfter - memBefore) : 0;

            auto& s = algResults[groupKey];
            s.totalTime += elapsedTime;
            s.totalMemory += memUsed;
            s.count++;

            if ((info.dataSize == 500000 || info.dataSize == 1000000) &&
                elapsedTime >= TIMEOUT_SECONDS) {
                cout << "[TIMEOUT] (" << algName << ") File " << filename 
                     << " took " << elapsedTime << " seconds. Skipping remaining files for group (" 
                     << info.pattern << ", " << info.dataSize << ")." << endl;
                timeoutGroups.insert(groupKey);
            }
        } // end for each file

        string csvFileName = "analysis_result_" + algName + ".csv";
        ofstream csvOut(csvFileName);
        csvOut << "Algorithm,Pattern,Size,AverageTime(s),AverageMemory(B)\n";
        for (const auto& [key, stat] : algResults) {
            auto [pattern, size] = key;
            double avgTime = (stat.count == 0) ? 0.0 : stat.totalTime / stat.count;
            size_t avgMem = (stat.count == 0) ? 0 : stat.totalMemory / stat.count;
            csvOut << algName << "," << pattern << "," << size << "," 
                   << avgTime << "," << avgMem << "\n";
        }
        csvOut.close();
        cout << "[INFO] Results for " << algName 
             << " saved to " << csvFileName << endl;
    } // end for each algorithm

    return 0;
}
