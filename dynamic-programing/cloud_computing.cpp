/**
 * Multi-dimensional Knapsack Problem Solver
 * Using Dynamic Programming (Memory Optimized with Hash Map)
 * 
 * Problem: Resource Management in Cloud Computing
 * - Maximize total value from selected requests
 * - Constraints: CPU, RAM, Bandwidth limits
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <unordered_map>
#include <map>

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

ProblemData readInputFile(const string& filename) {
    ProblemData data;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Không thể mở được file" << filename << endl;
        return data;
    }
    
    string line;
    
    while (getline(file, line)) {
        // Bỏ qua các dòng không có gì
        if (line.empty()) continue;
        
        // Bỏ qua các khoảng trắng
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == string::npos) continue;
        
        // Bỏ qua các dòng comment
        if (line[start] == '#') continue;
        
        // Trích xuất các dữ liệu
        string content = line.substr(start);
        
        if (content.find("C_max") != string::npos) {
            if (sscanf(content.c_str(), "C_max = %d", &data.C_max) != 1) {
                cerr << "Cảnh báo: Không thể trích xuất C_max: " << content << endl;
            }
        } else if (content.find("R_max") != string::npos) {
            if (sscanf(content.c_str(), "R_max = %d", &data.R_max) != 1) {
                cerr << "Cảnh báo: Không thể trích xuất R_max: " << content << endl;
            }
        } else if (content.find("B_max") != string::npos) {
            if (sscanf(content.c_str(), "B_max = %d", &data.B_max) != 1) {
                cerr << "Cảnh báo: Không thể trích xuất B_max: " << content << endl;
            }
        } else {
            // Đọc các bản ghi: ID, CPU, RAM, Bandwidth
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

// State: Trạng thái là 1 dãy số nguyên 64 bit để cho mã hóa
// High 21 bits: CPU, middle 21 bits: RAM, low 21 bits: Bandwidth
using State = uint64_t;
using ItemMask = uint64_t;

State makeState(int c, int r, int b) {
    return ((State)c << 42) | ((State)r << 21) | (State)b;
}

void parseState(State s, int& c, int& r, int& b) {
    c = (s >> 42) & 0x1FFFFF;
    r = (s >> 21) & 0x1FFFFF;
    b = s & 0x1FFFFF;
}

/**
 * DP với tối ưu bộ nhớ sử dụng hashmap để lưu trữ trạng thái
 * Chỉ lưu trữ các trạng thái có thể đạt được với giá trị tối đa tương ứng
 * 
 * State: (cpu_used, ram_used, bandwidth_used)
 * Value: (max_value, selected_items_bitmask)
 */
pair<int, vector<int>> solveKnapsackDP(ProblemData& data) {
    int C = data.C_max;
    int R = data.R_max;
    int B = data.B_max;
    int N = data.N;
    
    // unordered_map để lưu trữ trạng thái và giá trị tối đa tương ứng 
    unordered_map<State, pair<int, ItemMask>> prev, curr;
    
    // Trạng thái ban đầu
    prev[makeState(0, 0, 0)] = {0, 0};
    
    for (int i = 0; i < N; i++) {
        Request& req = data.requests[i];
        curr.clear();
        
        // 
        curr.reserve(prev.size() * 2);
        
        for (auto& [state, val] : prev) {
            int c, r, b;
            parseState(state, c, r, b);
            int value = val.first;
            ItemMask selected = val.second;
            
            // TH1: không chọn item i
            State s1 = state;
            auto it1 = curr.find(s1);
            if (it1 == curr.end() || it1->second.first < value) {
                curr[s1] = {value, selected};
            }
            
            // TH2: chọn item i (nếu đủ dung lượng)
            int nc = c + req.cpu;
            int nr = r + req.ram;
            int nb = b + req.bandwidth;
            
            if (nc <= C && nr <= R && nb <= B) {
                int newValue = value + req.value;
                ItemMask newSelected = selected | (1LL << i);
                State s2 = makeState(nc, nr, nb);
                
                auto it2 = curr.find(s2);
                if (it2 == curr.end() || it2->second.first < newValue) {
                    curr[s2] = {newValue, newSelected};
                }
            }
        }
        
        swap(prev, curr);
    }
    
    // Tìm giá trị tối đa và các item được chọn từ prev
    int maxValue = 0;
    ItemMask bestSelected = 0;
    
    for (auto& [state, val] : prev) {
        if (val.first > maxValue) {
            maxValue = val.first;
            bestSelected = val.second;
        }
    }
    
    // Giải mã các item được chọn từ bitmask
    vector<int> selectedItems;
    
    for (int i = 0; i < N && i < 63; i++) {
        if (bestSelected & (1LL << i)) {
            selectedItems.push_back(data.requests[i].id);
        }
    }
    
    return {maxValue, selectedItems};
}

void printResults(const string& filename, int maxValue, const vector<int>& selectedItems, 
                  const ProblemData& data, double timeMs) {
    cout << "\n========================================" << endl;
    cout << "Results for: " << filename << endl;
    cout << "========================================" << endl;
    
    // Tính tổng tài nguyên đã sử dụng
    int totalCPU = 0, totalRAM = 0, totalBW = 0;
    for (int id : selectedItems) {
        for (const auto& req : data.requests) {
            if (req.id == id) {
                totalCPU += req.cpu;
                totalRAM += req.ram;
                totalBW += req.bandwidth;
                break;
            }
        }
    }
    
    cout << "Maximum Value: " << maxValue << endl;
    cout << "Selected Items (" << selectedItems.size() << " items): ";
    for (int id : selectedItems) {
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
    
    cout << "\nExecution Time: " << timeMs << " ms" << endl;
    cout << "Time Complexity: O(N * C_max * R_max * B_max) = O(" 
         << data.N << " * " << data.C_max << " * " << data.R_max << " * " << data.B_max 
         << ") = O(" << (long long)data.N * data.C_max * data.R_max * data.B_max << ")" << endl;
}

int main(int argc, char* argv[]) {
    vector<string> files = {
        "data/knapsack_data_n5.txt",
        "data/knapsack_data_n10.txt",
        "data/knapsack_data_n20.txt",
        "data/knapsack_data_n30.txt"
    };
    
    cout << "============================================" << endl;
    cout << "Multi-dimensional Knapsack Problem Solver" << endl;
    cout << "Dynamic Programming Approach (Memory Optimized)" << endl;
    cout << "============================================" << endl;
    
    for (const string& file : files) {
        ProblemData data = readInputFile(file);
        
        auto start = chrono::high_resolution_clock::now();
        auto result = solveKnapsackDP(data);
        auto end = chrono::high_resolution_clock::now();
        
        double timeMs = chrono::duration<double, milli>(end - start).count();
        printResults(file, result.first, result.second, data, timeMs);
    }
    
    cout << "\n========================================" << endl;
    cout << "All test cases completed!" << endl;
    cout << "========================================" << endl;
    
    return 0;
}