/**
 * Multi-dimensional Knapsack Problem Solver
 * Using Brute Force (Exhaustive Search)
 * 
 * Problem: Resource Management in Cloud Computing
 * - Maximize total value from selected requests
 * - Constraints: CPU, RAM, Bandwidth limits
 * 
 * Approach: Enumerate all 2^N subsets using bitmask,
 *           check constraints and track the best valid subset.
 * Time Complexity: O(2^N * N)
 * Space Complexity: O(N)
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <cstring>

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
        cerr << "Error: Không thể mở được file " << filename << endl;
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
            // Đọc các bản ghi: ID, CPU, RAM, Bandwidth, Value
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
 * Giải bài toán bằng phương pháp vét cạn (Brute Force)
 * 
 * Ý tưởng:
 * - Duyệt tất cả 2^N tổ hợp con có thể có của N yêu cầu
 * - Mỗi tổ hợp được biểu diễn bằng một bitmask (số nguyên từ 0 đến 2^N - 1)
 * - Với mỗi tổ hợp, tính tổng tài nguyên và kiểm tra ràng buộc
 * - Giữ lại tổ hợp hợp lệ có tổng giá trị lớn nhất
 * 
 * Ví dụ với N=5, mask = 13 (01101 nhị phân):
 *   bit 0 = 1 → chọn yêu cầu 1
 *   bit 1 = 0 → bỏ yêu cầu 2
 *   bit 2 = 1 → chọn yêu cầu 3
 *   bit 3 = 1 → chọn yêu cầu 4
 *   bit 4 = 0 → bỏ yêu cầu 5
 */
pair<int, vector<int>> solveKnapsackBruteForce(ProblemData& data) {
    int C = data.C_max;
    int R = data.R_max;
    int B = data.B_max;
    int N = data.N;
    
    int bestValue = 0;
    long long bestMask = 0;
    long long totalCombinations = 1LL << N;  // 2^N tổ hợp
    
    // Duyệt tất cả 2^N tổ hợp
    for (long long mask = 0; mask < totalCombinations; mask++) {
        int totalCPU = 0;
        int totalRAM = 0;
        int totalBW = 0;
        int totalValue = 0;
        
        // Kiểm tra từng bit trong mask để xác định yêu cầu nào được chọn
        for (int i = 0; i < N; i++) {
            if (mask & (1LL << i)) {
                totalCPU   += data.requests[i].cpu;
                totalRAM   += data.requests[i].ram;
                totalBW    += data.requests[i].bandwidth;
                totalValue += data.requests[i].value;
            }
        }
        
        // Kiểm tra ràng buộc tài nguyên: cả 3 chiều đều phải thỏa mãn
        if (totalCPU <= C && totalRAM <= R && totalBW <= B) {
            // Cập nhật nghiệm tốt nhất nếu tổng giá trị lớn hơn
            if (totalValue > bestValue) {
                bestValue = totalValue;
                bestMask = mask;
            }
        }
    }
    
    // Giải mã bitmask để lấy danh sách các yêu cầu được chọn
    vector<int> selectedItems;
    for (int i = 0; i < N; i++) {
        if (bestMask & (1LL << i)) {
            selectedItems.push_back(data.requests[i].id);
        }
    }
    
    return {bestValue, selectedItems};
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
    
    cout << "\nTotal Combinations Checked: 2^" << data.N << " = " << (1LL << data.N) << endl;
    cout << "Execution Time: " << timeMs << " ms" << endl;
    cout << "Time Complexity: O(2^N * N) = O(2^" << data.N << " * " << data.N 
         << ") = O(" << (1LL << data.N) * data.N << ")" << endl;
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
        
        double timeMs = chrono::duration<double, milli>(end - start).count();
        printResults(file, result.first, result.second, data, timeMs);
    }
    
    cout << "\n========================================" << endl;
    cout << "All test cases completed!" << endl;
    cout << "========================================" << endl;
    
    return 0;
}
