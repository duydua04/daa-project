#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <iomanip>

using namespace std;

struct Request {
    int id;
    int cpu;
    int ram;
    int bandwidth;
    int value;
};

struct ProblemData {
    int C_max = 0;
    int R_max = 0;
    int B_max = 0;
    int N = 0;
    vector<Request> requests;
};

struct SolveResult {
    int maxValue;
    vector<int> selectedItems;
    size_t memoryBytes;
};

static size_t estimateRequestVectorMemory(const vector<Request>& v) {
    return sizeof(Request) * v.capacity();
}

static size_t estimateIntVectorMemory(const vector<int>& v) {
    return sizeof(int) * v.capacity();
}

ProblemData readInputFile(const string& filename) {
    ProblemData data;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Không thể mở được file " << filename << endl;
        return data;
    }
    
    string line;
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == string::npos) continue;
        if (line[start] == '#') continue;
        string content = line.substr(start);
        
        if (content.find("C_max") != string::npos) {
            if (sscanf(content.c_str(), "C_max = %d", &data.C_max) != 1) {
                cerr << "Không thể trích xuất C_max: " << content << endl;
            }
        } else if (content.find("R_max") != string::npos) {
            if (sscanf(content.c_str(), "R_max = %d", &data.R_max) != 1) {
                cerr << "Không thể trích xuất R_max: " << content << endl;
            }
        } else if (content.find("B_max") != string::npos) {
            if (sscanf(content.c_str(), "B_max = %d", &data.B_max) != 1) {
                cerr << "Không thể trích xuất B_max: " << content << endl;
            }
        } else {

            Request req;
            if (sscanf(content.c_str(), "%d %d %d %d %d", 
                       &req.id, &req.cpu, &req.ram, &req.bandwidth, &req.value) == 5) {
                data.requests.push_back(req);
            }
        }
    }
    
    data.N = data.requests.size();
    file.close();
    
    return data;
}

/**
 * Giải bài toán bằng phương pháp vét cạn
 */
SolveResult solveKnapsackBruteForce(ProblemData& data) {
    int C = data.C_max;
    int R = data.R_max;
    int B = data.B_max;
    int N = data.N;
    
    int bestValue = 0;
    long long bestMask = 0;
    long long totalCombinations = 1LL << N; 
    
    size_t memoryBytes = sizeof(ProblemData) + estimateRequestVectorMemory(data.requests);
    
    for (long long mask = 0; mask < totalCombinations; mask++) {
        int totalCPU = 0;
        int totalRAM = 0;
        int totalBW = 0;
        int totalValue = 0;
        
        for (int i = 0; i < N; i++) {
            if (mask & (1LL << i)) {
                totalCPU   += data.requests[i].cpu;
                totalRAM   += data.requests[i].ram;
                totalBW    += data.requests[i].bandwidth;
                totalValue += data.requests[i].value;
            }
        }
        if (totalCPU <= C && totalRAM <= R && totalBW <= B) {
            if (totalValue > bestValue) {
                bestValue = totalValue;
                bestMask = mask;
            }
        }
    }
    vector<int> selectedItems;
    for (int i = 0; i < N; i++) {
        if (bestMask & (1LL << i)) {
            selectedItems.push_back(data.requests[i].id);
        }
    }
    
    memoryBytes += estimateIntVectorMemory(selectedItems);
    return {bestValue, selectedItems, memoryBytes};
}

void printResults(const string& filename, const SolveResult& result, 
                  const ProblemData& data, double timeMs) {
    cout << "\n========================================" << endl;
    cout << "Results for: " << filename << endl;
    cout << "========================================" << endl;
    
    int totalCPU = 0, totalRAM = 0, totalBW = 0;
    for (int id : result.selectedItems) {
        for (const auto& req : data.requests) {
            if (req.id == id) {
                totalCPU += req.cpu;
                totalRAM += req.ram;
                totalBW += req.bandwidth;
                break;
            }
        }
    }
    
    cout << "Maximum Value: " << result.maxValue << endl;
    cout << "Selected Items (" << result.selectedItems.size() << " items): ";
    for (int id : result.selectedItems) {
        cout << id << " ";
    }
    cout << endl;
    
    cout << "\nResource Usage:" << endl;
    cout << "  CPU: " << totalCPU << " / " << data.C_max 
         << " (" << (100.0 * totalCPU / data.C_max) << "%)" << endl;
    cout << "  RAM: " << totalRAM << " / " << data.R_max 
         << " (" << (100.0 * totalRAM / data.R_max) << "%)" << endl;
    cout << "  Bandwidth: " << totalBW << " / " << data.B_max 
         << " (" << (100.0 * totalBW / data.B_max) << "%)" << endl;
    
    cout << "\nTotal Combinations Checked: 2^" << data.N << " = " << (1LL << data.N) << endl;
    cout << "Memory Usage    : " << result.memoryBytes << " bytes";
    if (result.memoryBytes >= 1024) {
        cout << " (" << fixed << setprecision(2)
             << result.memoryBytes / 1024.0 << " KB)";
    }
    cout << endl;
    cout << "Execution Time: " << timeMs << " us" << endl;
    cout << "Time Complexity: O(2^N * N) = O(2^" << data.N << " * " << data.N 
         << ") = O(" << (1LL << data.N) * data.N << ")" << endl;
}

int main(int argc, char* argv[]) {
    vector<string> files = {
        "data/knapsack_data_n5.txt",
        "data/knapsack_data_n10.txt",
        "data/knapsack_data_n20.txt",
        "data/knapsack_data_n30.txt",
        "data/knapsack_data_n40.txt",
        "data/knapsack_data_n50.txt"  
    };
    
    cout << "============================================" << endl;
    cout << "Multi-dimensional Knapsack Problem Solver" << endl;
    cout << "Brute Force Approach (Exhaustive Search)" << endl;
    cout << "============================================" << endl;
    
    for (const string& file : files) {
        ProblemData data = readInputFile(file);
        
        // Cảnh báo nếu N quá lớn
        if (data.N > 25) {
            cout << "\n========================================" << endl;
            cout << "Results for: " << file << endl;
            cout << "========================================" << endl;
            cout << "WARNING: N = " << data.N << " quá lớn cho Brute Force!" << endl;
            cout << "Số tổ hợp cần duyệt: 2^" << data.N << " = " << (1LL << data.N) << endl;
            cout << "Ước tính thời gian rất lâu. Bỏ qua test case này." << endl;
            cout << "Đề xuất: Sử dụng Dynamic Programming hoặc Branch & Bound." << endl;
            continue;
        }
        
        auto start = chrono::high_resolution_clock::now();
        auto result = solveKnapsackBruteForce(data);
        auto end = chrono::high_resolution_clock::now();
        
        double timeUs = chrono::duration<double, micro>(end - start).count();
        printResults(file, result, data, timeUs);
    }
    
    cout << "\n========================================" << endl;
    cout << "All test cases completed!" << endl;
    cout << "========================================" << endl;
    
    return 0;
}
