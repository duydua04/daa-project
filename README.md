# 1. Nêu bài toán tối ưu quản lý tài nguyên trong điện toán đám mây

Điện toán đám mây là mô hình cung cấp tài nguyên tính toán như CPU, bộ nhớ RAM, dung lượng lưu trữ, băng thông mạng, máy ảo, container và các dịch vụ phần mềm thông qua Internet. Thay vì phải đầu tư hệ thống máy chủ vật lý riêng, người dùng có thể thuê tài nguyên từ nhà cung cấp dịch vụ đám mây theo nhu cầu sử dụng. Đặc điểm quan trọng của mô hình này là tài nguyên được cấp phát linh hoạt, có thể mở rộng hoặc thu hồi tùy theo tải công việc, đồng thời chi phí thường được tính dựa trên lượng tài nguyên thực tế mà người dùng sử dụng.

Trong môi trường điện toán đám mây, nhiều người dùng hoặc nhiều ứng dụng có thể đồng thời gửi yêu cầu sử dụng tài nguyên đến hệ thống. Mỗi yêu cầu có thể tương ứng với việc khởi tạo một máy ảo, một container, một dịch vụ xử lý dữ liệu, một tác vụ tính toán hoặc một ứng dụng cần được triển khai. Mỗi yêu cầu này thường cần một lượng tài nguyên nhất định, ví dụ số lõi CPU, dung lượng RAM và băng thông mạng. Tuy nhiên, tài nguyên của hệ thống máy chủ là hữu hạn. Do đó, hệ thống không thể luôn chấp nhận tất cả các yêu cầu nếu tổng nhu cầu tài nguyên vượt quá khả năng cung cấp.

Vì vậy, bài toán đặt ra là: với một tập các yêu cầu tài nguyên từ người dùng và một hệ thống đám mây có giới hạn về CPU, RAM và băng thông, cần lựa chọn những yêu cầu nào nên được chấp nhận để tối đa hóa tổng giá trị thu được, đồng thời đảm bảo tổng tài nguyên đã cấp phát không vượt quá giới hạn của hệ thống.

Bài toán này có thể được mô hình hóa như một biến thể của bài toán __Cái túi đa chiều__, hay __Multi-dimensional Knapsack Problem - MKP__. Trong bài toán cái túi truyền thống, ta cần chọn các đồ vật đưa vào túi sao cho tổng giá trị là lớn nhất nhưng tổng trọng lượng không vượt quá sức chứa của túi. Đối với bài toán quản lý tài nguyên trong điện toán đám mây, “cái túi” chính là hệ thống máy chủ hoặc cụm máy chủ đám mây, còn “trọng lượng” không chỉ là một đại lượng duy nhất mà gồm nhiều loại tài nguyên khác nhau, cụ thể là CPU, RAM và băng thông.

Giả sử hệ thống có giới hạn tài nguyên tối đa như sau:

- $C_{max}$: tổng dung lượng CPU tối đa của hệ thống.
- $R_{max}$: tổng dung lượng RAM tối đa của hệ thống.
- $B_{max}$: tổng băng thông tối đa của hệ thống.

Có tập gồm $N$ yêu cầu cần được xem xét. Mỗi yêu cầu $i$, với $1 \leq i \leq N$, có các thông tin:

- $c_i$: lượng CPU mà yêu cầu $i$ cần sử dụng.
- $r_i$: lượng RAM mà yêu cầu $i$ cần sử dụng.
- $b_i$: lượng băng thông mà yêu cầu $i$ cần sử dụng.
- $v_i$: giá trị, lợi ích, doanh thu hoặc mức độ ưu tiên thu được nếu yêu cầu $i$ được phục vụ.

Ta định nghĩa biến quyết định:

$$x_i \in {0,1}$$

Trong đó:

- $x_i = 1$: yêu cầu $i$ được chấp nhận và được cấp phát tài nguyên.
- $x_i = 0$: yêu cầu $i$ bị từ chối hoặc chưa được phục vụ tại thời điểm hiện tại.

Mục tiêu của bài toán là tối đa hóa tổng giá trị của các yêu cầu được chấp nhận:

$$\max \sum_{i=1}^{N} v_i x_i$$

Với các ràng buộc tài nguyên:

$$\sum_{i=1}^{N} c_i x_i \leq C_{max}$$

$$\sum_{i=1}^{N} r_i x_i \leq R_{max}$$

$$\sum_{i=1}^{N} b_i x_i \leq B_{max}$$

Các ràng buộc trên đảm bảo rằng tổng lượng CPU, RAM và băng thông của các yêu cầu được chấp nhận không vượt quá khả năng cung cấp của hệ thống. Đây là điểm cốt lõi của bài toán tối ưu quản lý tài nguyên trong điện toán đám mây: không chỉ cần chọn các yêu cầu có giá trị cao, mà còn phải xét đến việc chúng tiêu thụ tài nguyên như thế nào trên nhiều chiều ràng buộc khác nhau.

Ví dụ, một yêu cầu có giá trị cao nhưng tiêu thụ quá nhiều CPU hoặc băng thông có thể khiến hệ thống không còn đủ tài nguyên để phục vụ các yêu cầu khác. Ngược lại, một số yêu cầu có giá trị riêng lẻ thấp hơn nhưng tiêu thụ ít tài nguyên có thể kết hợp lại để tạo ra tổng giá trị lớn hơn. Vì vậy, việc lựa chọn tối ưu không thể chỉ dựa vào giá trị của từng yêu cầu, mà cần xem xét đồng thời giữa giá trị mang lại và lượng tài nguyên mà yêu cầu đó tiêu thụ.

Bài toán này có ý nghĩa thực tiễn rất lớn trong các hệ thống đám mây. Nếu hệ thống lựa chọn tài nguyên không hợp lý, nhà cung cấp dịch vụ có thể bị lãng phí tài nguyên, giảm hiệu suất, tăng chi phí vận hành hoặc vi phạm các cam kết chất lượng dịch vụ. Ngược lại, nếu việc phân bổ tài nguyên được tối ưu, hệ thống có thể phục vụ được nhiều yêu cầu quan trọng hơn, tăng doanh thu, cải thiện hiệu năng và sử dụng hiệu quả hạ tầng hiện có.

---

# 2. Các thách thức của bài toán

* **Tài nguyên hệ thống có giới hạn và nhiều ràng buộc đồng thời:** Hệ thống phải quản lý và thỏa mãn cùng lúc CPU, RAM và băng thông.
* **Khó khăn trong việc lựa chọn yêu cầu tối ưu:** Việc chọn yêu cầu có giá trị cao nhất chưa chắc đem lại tổng giá trị tối ưu do chúng tiêu hao nhiều tài nguyên.
* **Bài toán thuộc nhóm khó về mặt tính toán (NP-hard):** Số lượng tổ hợp tăng lũy thừa $2^N$ theo số lượng yêu cầu $N$, gây khó khăn cho việc tính toán nhanh.
* **Nhu cầu tài nguyên thay đổi liên tục:** Tải của hệ thống biến động liên tục theo thời gian thực.
* **Sự khác nhau về đặc điểm của các yêu cầu:** Các loại máy ảo/container có yêu cầu tài nguyên không đồng nhất.
* **Đảm bảo chất lượng dịch vụ (SLA):** Tránh việc cấp phát vượt ngưỡng làm suy giảm hiệu năng hệ thống.
* **Cân bằng giữa hiệu năng và chi phí:** Hạn chế lãng phí tài nguyên nhàn rỗi trong khi vẫn đảm bảo hiệu suất hoạt động.

---

# 3. Hướng giải pháp cho bài toán

* **Mô hình hóa bài toán dưới dạng cái túi đa chiều (MKP):** Phát biểu toán học để tìm vector quyết định tối ưu hóa giá trị trong các ràng buộc tài nguyên.
* **Sử dụng thuật toán vét cạn cho bộ dữ liệu nhỏ:** Duyệt qua toàn bộ tổ hợp con của tập yêu cầu để đảm bảo tìm nghiệm tối ưu tuyệt đối.
* **Sử dụng quy hoạch động khi tài nguyên có miền giá trị vừa phải:** Giải quyết bài toán bằng bảng phương án trạng thái tối ưu hóa bộ nhớ qua hashmap.
* **Sử dụng thuật toán nhánh cận để giảm không gian tìm kiếm:** Xây dựng cây tìm kiếm và cắt tỉa các nhánh không triển vọng.
* **Sử dụng thuật toán tham lam để tìm nghiệm nhanh:** Định nghĩa điểm số ưu tiên ($score_i$) dựa trên tài nguyên chuẩn hóa để sắp xếp và chọn nhanh ứng viên.
* **Kết hợp giám sát tài nguyên theo thời gian thực:** Theo dõi các mức tải thực tế để cập nhật giới hạn tài nguyên khả dụng.
* **Tự động mở rộng và thu hồi tài nguyên (Auto-scaling):** Tăng/giảm node vật lý dựa trên nhu cầu của các yêu cầu.
* **Đánh giá nghiệm dựa trên nhiều tiêu chí:** Xem xét đa chiều (tổng giá trị, độ tải tài nguyên, thời gian chạy) để so sánh các giải thuật.


---

# 4. Hướng dẫn cài đặt môi trường

## 4.1. Yêu cầu hệ thống

| Thành phân | Yêu cầu tối thiểu | Ghi chú |
|---|---|---|
| **Hệ điều hành** | Windows, Linux hoặc macOS | Windows cần cài MSYS2 để có môi trường tương thích |
| **Trình biên dịch C++** | `g++` hỗ trợ chuẩn **C++17** | GCC ≥ 7.0 hoặc Clang ≥ 5.0 |
| **GNU Make** | `make` ≥ 3.81 | Windows: đi kèm MSYS2; Linux/macOS: có sẵn |
| **Python 3** | Python ≥ 3.6 | Chỉ cần nếu muốn sinh file CSV thống kê |
| **Git** | Bất kỳ phiên bản nào | Tùy chọn — có thể nhận dự án qua file ZIP |

## 4.2. Cài đặt trên Windows

Trên Windows, cần cài **MSYS2** để có `g++`, `make` và shell bash tương thích với Makefile của dự án.

### Bước 1: Cài đặt MSYS2

1. Tải MSYS2 từ trang chủ: [https://www.msys2.org](https://www.msys2.org)
2. Chạy file cài đặt và cài vào đường dẫn mặc định (`C:\msys64`).
3. Sau khi cài xong, mở **MSYS2 UCRT64** (hoặc **MSYS2 MINGW64**) từ Start Menu.

### Bước 2: Cài đặt g++, make và git

Trong cửa sổ MSYS2 terminal, chạy:

```bash
# Cập nhật hệ thống gói
pacman -Syu

# Cài đặt g++, make và git
pacman -S --noconfirm mingw-w64-ucrt-x86_64-gcc make git

# Cài đặt Python 3 (tùy chọn, nếu muốn sinh CSV)
pacman -S --noconfirm mingw-w64-ucrt-x86_64-python
```

### Bước 3: Sử dụng

> **Quan trọng:** Luôn mở **MSYS2 UCRT64** terminal để chạy các lệnh `make`, `g++` trong dự án. Không sử dụng CMD hoặc PowerShell vì chúng không tương thích với Makefile.

**Cách thay thế (không cần MSYS2):** Nếu không muốn cài MSYS2, có thể biên dịch thủ công từng file bằng `g++` (xem [Mục 5.4](#54-chạy-từng-phương-pháp) — phần biên dịch thủ công).

## 4.3. Cài đặt trên Ubuntu / Debian

```bash
# Cập nhật danh sách gói
sudo apt update

# Cài đặt g++, make và git
sudo apt install -y build-essential git

# Cài đặt Python 3 (nếu chưa có)
sudo apt install -y python3
```

## 4.4. Cài đặt trên macOS

```bash
# Cài đặt Xcode Command Line Tools (bao gồm clang++ và make)
xcode-select --install

# Cài đặt Python 3 qua Homebrew (nếu chưa có)
brew install python3
```

## 4.5. Lấy mã nguồn dự án

### Cách 1: Clone từ GitHub (nếu có Git)

```bash
git clone https://github.com/duydua04/daa-project.git
cd daa-project
```

### Cách 2: Tải file ZIP và giải nén

Nếu bạn nhận dự án qua file ZIP:

1. Giải nén file ZIP vào một thư mục
2. Mở terminal (MSYS2 trên Windows, Terminal trên Linux/macOS).
3. Di chuyển vào thư mục dự án:

```bash
# Thay đường dẫn phù hợp với vị trí bạn giải nén
cd /đường/dẫn/tới/daa-project
```

> **Trên Windows (MSYS2):** Đường dẫn `C:\Users\TenBan\Desktop\daa-project` tương đương `/c/Users/TenBan/Desktop/daa-project` trong MSYS2 terminal.

## 4.6. Kiểm tra môi trường

Chạy các lệnh sau để đảm bảo môi trường đã sẵn sàng:

```bash
# Kiểm tra trình biên dịch C++
g++ --version

# Kiểm tra Make
make --version

# Kiểm tra Python 3 (tùy chọn)
python3 --version
```

> **Trên Windows (MSYS2):** Nếu `python3` không hoạt động, thử dùng `python --version` thay thế.

---

# 5. Hướng dẫn sử dụng

## 5.1. Cấu trúc dự án

```
daa-project/
├── Makefile                         # Tự động biên dịch và chạy
├── generate_metrics_csv.py          # Script sinh CSV thống kê hiệu năng
├── data/                            # Bộ dữ liệu đầu vào
│   ├── knapsack_data_n5.txt
│   ├── knapsack_data_n10.txt
│   ├── knapsack_data_n20.txt
│   ├── knapsack_data_n30.txt
│   ├── knapsack_data_n40.txt
│   └── knapsack_data_n50.txt
├── brute-force/                     # Phương pháp vét cạn
│   └── cloud_computing.cpp
├── dynamic-programing/              # Phương pháp quy hoạch động
│   └── cloud_computing.cpp
├── greedy/                          # Phương pháp tham lam
│   └── cloud_computing.cpp
├── visualization/                   # Biểu đồ so sánh hiệu năng (PNG)
├── statistics/                      # File CSV thống kê (tự động tạo khi chạy `make csv`)
└── docs/                            # Tài liệu bổ sung
```

## 5.2. Định dạng dữ liệu đầu vào

Mỗi file dữ liệu trong thư mục `data/` có định dạng:

```text
# Multi-dimensional Knapsack Problem Dataset
# Number of requests (N): 5

# Server capacity limits
C_max = 16
R_max = 32
B_max = 100

# Request data: ID, CPU (c_i), RAM (r_i), Bandwidth (b_i), Value (v_i)
1 2 4 10 50
2 4 8 20 80
3 3 6 15 65
4 5 10 25 100
5 2 4 12 45
```

- Dòng bắt đầu bằng `#` là comment, sẽ bị bỏ qua.
- `C_max`, `R_max`, `B_max` là giới hạn tài nguyên của hệ thống.
- Mỗi dòng dữ liệu yêu cầu gồm 5 cột: `ID  CPU  RAM  Bandwidth  Value`.

## 5.3. Biên dịch dự án

### Cách 1: Dùng Make (khuyến nghị)

```bash
# Biên dịch tất cả các phương pháp
make all
```

Lệnh này sẽ biên dịch cả 3 file source và tạo ra các binary tương ứng:

| Binary được tạo | Phương pháp |
|---|---|
| `brute-force/cloud_computing` | Vét cạn (Brute Force) |
| `dynamic-programing/cloud_computing` | Quy hoạch động (Dynamic Programming) |
| `greedy/cloud_computing` | Tham lam (Greedy) |

Xóa các file đã biên dịch:

```bash
make clean
```

### Cách 2: Biên dịch thủ công bằng g++ (không cần Make)

Nếu không có `make` (ví dụ: sử dụng CMD hoặc PowerShell trên Windows), có thể biên dịch trực tiếp:

```bash
# Biên dịch phương pháp Brute Force
g++ -std=c++17 -O2 -Wall -o brute-force/cloud_computing brute-force/cloud_computing.cpp

# Biên dịch phương pháp Dynamic Programming
g++ -std=c++17 -O2 -Wall -o dynamic-programing/cloud_computing dynamic-programing/cloud_computing.cpp

# Biên dịch phương pháp Greedy
g++ -std=c++17 -O2 -Wall -o greedy/cloud_computing greedy/cloud_computing.cpp
```

> **Trên Windows CMD/PowerShell:** Thay `/` bằng `\` trong đường dẫn, ví dụ:
> ```cmd
> g++ -std=c++17 -O2 -Wall -o brute-force\cloud_computing.exe brute-force\cloud_computing.cpp
> ```

## 5.4. Chạy từng phương pháp

### 5.4.1. Phương pháp Vét cạn (Brute Force)

```bash
# Dùng make
make run-brute-force/cloud_computing

# Chạy thủ công 
./brute-force/cloud_computing          # Linux / macOS / MSYS2
brute-force\cloud_computing.exe        # Windows CMD / PowerShell
```

### 5.4.2. Phương pháp Quy hoạch động (Dynamic Programming)

```bash
# Dùng make
make run-dynamic-programing/cloud_computing

# Chạy thủ công (sau khi đã biên dịch)
./dynamic-programing/cloud_computing   # Linux / macOS / MSYS2
dynamic-programing\cloud_computing.exe # Windows CMD / PowerShell
```

### 5.4.3. Phương pháp Tham lam (Greedy)

```bash
# Dùng make
make run-greedy/cloud_computing

# Chạy thủ công
./greedy/cloud_computing               # Linux / macOS / MSYS2
greedy\cloud_computing.exe             # Windows CMD / PowerShell
```


## 5.5. Chạy tất cả các phương pháp

```bash
make run-all
```

Lệnh này sẽ biên dịch và chạy lần lượt cả 3 phương pháp trên tất cả bộ dữ liệu. Kết quả mỗi phương pháp bao gồm:

- **Maximum Value**: tổng giá trị tối đa đạt được.
- **Selected Items**: danh sách các yêu cầu được chọn.
- **Resource Usage**: mức sử dụng CPU / RAM / Bandwidth (và tỷ lệ %).
- **Memory Usage**: bộ nhớ ước tính (bytes).
- **Execution Time**: thời gian chạy (microseconds).

## 5.6. Sinh file CSV thống kê

```bash
make csv
```

Lệnh này sẽ:
1. Biên dịch tất cả các phương pháp.
2. Chạy script `generate_metrics_csv.py` để thực thi từng phương pháp.
3. Xuất các file CSV vào thư mục `statistics/`:

| File CSV | Nội dung |
|---|---|
| `maximum_value.csv` | So sánh giá trị tối đa đạt được |
| `memory_usage.csv` | So sánh mức sử dụng bộ nhớ |
| `execution_time.csv` | So sánh thời gian thực thi |
| `cpu_usage.csv` | So sánh mức sử dụng CPU |
| `ram_usage.csv` | So sánh mức sử dụng RAM |
| `bandwidth_usage.csv` | So sánh mức sử dụng băng thông |

Mỗi file CSV có cấu trúc: cột đầu là kích thước $N$, các cột tiếp theo là giá trị tương ứng của mỗi phương pháp (Greedy, Dynamic Programming, Brute Force).


---

