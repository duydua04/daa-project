/**
 * Multi-dimensional Knapsack Problem Solver
 * Using Greedy Algorithm (Tham Lam)
 * 
 * Problem: Resource Management in Cloud Computing
 * - Maximize total value from selected requests
 * - Constraints: CPU, RAM, Bandwidth limits
 * 
 * Các bước của thuật toán:
 *   Tại mỗi bước, chọn yêu cầu có "đơn giá" cao nhất (value / weighted_resource)
 *   rồi kiểm tra tính khả thi (feasibility) trước khi đưa vào lời giải.
 * 
 *   Score = v_i / (α*c_i + β*r_i + γ*b_i)
 *   Trong đó α, β, γ là hệ số phản ánh mức độ quan trọng của từng tài nguyên.
 * 
 * 5 thành phần cốt lõi:
 *   1. Tập ứng viên (Candidate): tất cả N yêu cầu chưa được chọn
 *   2. Hàm lựa chọn (BestSelect): chọn yêu cầu có score cao nhất
 *   3. Hàm khả thi (Acceptable): kiểm tra tổng CPU, RAM, BW không vượt giới hạn
 *   4. Hàm mục tiêu: tổng giá trị các yêu cầu được chấp nhận
 *   5. Hàm đánh giá (IsSolution): khi không còn yêu cầu nào khả thi
 * 
 * Time Complexity: O(N log N) — do sắp xếp
 * Space Complexity: O(N)
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <iomanip>
// #include <windows.h>

using namespace std;

struct Request {
    int id;
    int cpu;
    int ram;
    int bandwidth;
    int value;
    double score; // đơn giá (value / weighted_resource)
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
        cerr << "Error: Không thể mở file " << filename << endl;
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
            sscanf(content.c_str(), "C_max = %d", &data.C_max);
        } else if (content.find("R_max") != string::npos) {
            sscanf(content.c_str(), "R_max = %d", &data.R_max);
        } else if (content.find("B_max") != string::npos) {
            sscanf(content.c_str(), "B_max = %d", &data.B_max);
        } else {
            Request req;
            if (sscanf(content.c_str(), "%d %d %d %d %d",
                       &req.id, &req.cpu, &req.ram, &req.bandwidth, &req.value) == 5) {
                req.score = 0.0; // sẽ tính sau
                data.requests.push_back(req);
            }
        }
    }

    data.N = data.requests.size();
    file.close();
    return data;
}

// ============================================================
// Tính đơn giá (Greedy score) cho từng yêu cầu
//
// score_i = v_i / (α*c_i + β*r_i + γ*b_i)
// Ta chuẩn hóa mỗi tài nguyên theo giới hạn tối đa của hệ thống
// để đảm bảo các chiều có trọng số đồng đều.
//
//   α = β = γ = 1/3  (trọng số bằng nhau)
//   resource_normalized = (c_i/C_max + r_i/R_max + b_i/B_max) / 3
//   score_i = v_i / resource_normalized
//           = 3 * v_i / (c_i/C_max + r_i/R_max + b_i/B_max)
// ============================================================

void computeScores(ProblemData& data) {
    for (auto& req : data.requests) {
        double norm_cpu = (data.C_max > 0) ? (double)req.cpu / data.C_max : 0.0;
        double norm_ram = (data.R_max > 0) ? (double)req.ram / data.R_max : 0.0;
        double norm_bw  = (data.B_max > 0) ? (double)req.bandwidth / data.B_max : 0.0;
        double weighted_resource = norm_cpu + norm_ram + norm_bw;

        if (weighted_resource > 0.0)
            req.score = (double)req.value / weighted_resource;
        else
            req.score = (req.value > 0) ? 1e18 : 0.0;
    }
}

// ============================================================
// Thuật toán Tham Lam — Greedy(A)
//
// Greedy(A):
//   S = ∅
//   while (A ≠ ∅ and !IsSolution(S))
//     x = BestSelect(A)        // chọn ứng viên tốt nhất
//     A = A \ {x}
//     if (Acceptable(S, x))    // kiểm tra khả thi
//       Integrate(S, x)        // tích hợp vào lời giải
//   return S
// ============================================================

SolveResult solveKnapsackGreedy(ProblemData& data) {
    // B1 — Tính đơn giá cho từng yêu cầu
    computeScores(data);

    // B2 — Sắp xếp ứng viên theo đơn giá giảm dần (BestSelect)
    vector<Request> candidates = data.requests;
    sort(candidates.begin(), candidates.end(),
         [](const Request& a, const Request& b) {
             return a.score > b.score;
         });

    // B3 — Khởi tạo lời giải
    int usedCPU = 0, usedRAM = 0, usedBW = 0;
    int totalValue = 0;
    vector<int> selectedIds;
    
    size_t memoryBytes = sizeof(ProblemData) + estimateRequestVectorMemory(data.requests);

    // B4 — Vòng lặp Greedy
    for (const auto& req : candidates) {
        // Hàm khả thi (Acceptable): kiểm tra ràng buộc 3 chiều
        bool feasible = (usedCPU + req.cpu       <= data.C_max) &&
                        (usedRAM + req.ram        <= data.R_max) &&
                        (usedBW  + req.bandwidth  <= data.B_max);

        if (feasible) {
            // Hàm Integrate: đưa yêu cầu vào lời giải
            usedCPU    += req.cpu;
            usedRAM    += req.ram;
            usedBW     += req.bandwidth;
            totalValue += req.value;
            selectedIds.push_back(req.id);
        }
        // Nếu không khả thi → bỏ qua, xét ứng viên tiếp theo
    }

    memoryBytes += estimateRequestVectorMemory(candidates);
    memoryBytes += estimateIntVectorMemory(selectedIds);
    return {totalValue, selectedIds, memoryBytes};
}


void printResults(const string& filename, const SolveResult& result,
                  const ProblemData& data, double timeMs) {
    cout << "\n========================================" << endl;
    cout << "Results for: " << filename << endl;
    cout << "========================================" << endl;

    // Tính tài nguyên đã dùng
    int totalCPU = 0, totalRAM = 0, totalBW = 0;
    for (int id : result.selectedItems) {
        for (const auto& req : data.requests) {
            if (req.id == id) {
                totalCPU += req.cpu;
                totalRAM += req.ram;
                totalBW  += req.bandwidth;
                break;
            }
        }
    }

    cout << "Maximum Value    : " << result.maxValue << endl;
    cout << "Selected Items (" << result.selectedItems.size() << " items): ";
    for (int id : result.selectedItems) cout << id << " ";
    cout << endl;

    cout << "\nResource Usage:" << endl;
    if (data.C_max > 0)
        cout << "  CPU      : " << totalCPU << " / " << data.C_max
             << " (" << fixed << setprecision(1)
             << (100.0 * totalCPU / data.C_max) << "%)" << endl;
    if (data.R_max > 0)
        cout << "  RAM      : " << totalRAM << " / " << data.R_max
             << " (" << (100.0 * totalRAM / data.R_max) << "%)" << endl;
    if (data.B_max > 0)
        cout << "  Bandwidth: " << totalBW << " / " << data.B_max
             << " (" << (100.0 * totalBW  / data.B_max) << "%)" << endl;
    cout << "Memory Usage    : " << result.memoryBytes << " bytes";
    if (result.memoryBytes >= 1024) {
        cout << " (" << fixed << setprecision(2)
             << result.memoryBytes / 1024.0 << " KB)";
    }
    cout << endl;
    cout << "\nExecution Time   : " << timeMs << " us" << endl;
    cout << "Time Complexity  : O(N log N) = O(" << data.N
         << " * log " << data.N << ")" << endl;

    // In bảng đơn giá để minh hoạ tiêu chí tham lam
    cout << "\n--- Greedy Score (top 5 candidates) ---" << endl;
    cout << left << setw(6)  << "ID"
         << setw(8)  << "CPU"
         << setw(8)  << "RAM"
         << setw(8)  << "BW"
         << setw(8)  << "Value"
         << setw(12) << "Score" << endl;
    cout << string(50, '-') << endl;

    // Tạo bản sao đã sắp xếp để in
    vector<Request> sorted = data.requests;
    computeScores(const_cast<ProblemData&>(data));
    sort(sorted.begin(), sorted.end(),
         [](const Request& a, const Request& b) { return a.score > b.score; });

    int printCount = min((int)sorted.size(), 5);
    for (int i = 0; i < printCount; i++) {
        const auto& r = sorted[i];
        bool selected = find(result.selectedItems.begin(), result.selectedItems.end(), r.id)
                        != result.selectedItems.end();
        cout << left << setw(6)  << r.id
             << setw(8)  << r.cpu
             << setw(8)  << r.ram
             << setw(8)  << r.bandwidth
             << setw(8)  << r.value
             << setw(12) << fixed << setprecision(4) << r.score
             << (selected ? " << SELECTED" : "")
             << endl;
    }
}

int main() {
    // SetConsoleOutputCP(65001);

    vector<string> files = {
        "data/knapsack_data_n5.txt",
        "data/knapsack_data_n10.txt",
        "data/knapsack_data_n20.txt",
        "data/knapsack_data_n30.txt",
        "data/knapsack_data_n40.txt", // Cảnh báo: N=40 sẽ rất lâu
        "data/knapsack_data_n50.txt"  // Cảnh báo: N=50 sẽ cực kỳ lâu
    };

    cout << "============================================" << endl;
    cout << "Multi-dimensional Knapsack Problem Solver"  << endl;
    cout << "Greedy Algorithm (Thuật Toán Tham Lam)"     << endl;
    cout << "============================================" << endl;

    for (const string& file : files) {
        ProblemData data = readInputFile(file);

        if (data.N == 0) {
            cout << "\n[SKIP] Không đọc được file: " << file << endl;
            continue;
        }

        auto start  = chrono::high_resolution_clock::now();
        auto result = solveKnapsackGreedy(data);
        auto end    = chrono::high_resolution_clock::now();

        double timeMs = chrono::duration<double, micro>(end - start).count();
        printResults(file, result, data, timeMs);
    }

    cout << "\n========================================" << endl;
    cout << "All test cases completed!" << endl;
    cout << "========================================" << endl;

    return 0;
}