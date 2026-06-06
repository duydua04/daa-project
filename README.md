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

## 2.1. Tài nguyên hệ thống có giới hạn và nhiều ràng buộc đồng thời

Thách thức đầu tiên của bài toán là hệ thống phải quản lý nhiều loại tài nguyên cùng lúc. Trong trường hợp này, các ràng buộc chính gồm CPU, RAM và băng thông. Một yêu cầu chỉ được chấp nhận khi hệ thống còn đủ cả ba loại tài nguyên này. Nếu chỉ còn đủ CPU và RAM nhưng không đủ băng thông, yêu cầu vẫn không thể được phục vụ.

Điều này làm cho bài toán phức tạp hơn nhiều so với bài toán tối ưu một chiều. Trong thực tế, một số ứng dụng có thể tiêu thụ nhiều CPU nhưng ít băng thông, ví dụ các tác vụ tính toán khoa học. Một số ứng dụng khác có thể cần nhiều băng thông nhưng không cần quá nhiều CPU, ví dụ truyền phát video hoặc hệ thống xử lý dữ liệu thời gian thực. Vì vậy, hệ thống cần đánh giá yêu cầu trên nhiều khía cạnh thay vì chỉ xét một loại tài nguyên duy nhất.

## 2.2. Khó khăn trong việc lựa chọn yêu cầu tối ưu

Không phải yêu cầu nào có giá trị cao nhất cũng nên được ưu tiên chọn. Một yêu cầu có giá trị lớn nhưng tiêu tốn quá nhiều CPU, RAM hoặc băng thông có thể làm giảm khả năng phục vụ các yêu cầu khác. Ngược lại, nhiều yêu cầu nhỏ có thể đem lại tổng giá trị cao hơn nếu chúng sử dụng tài nguyên hiệu quả hơn.

Do đó, hệ thống cần giải quyết bài toán lựa chọn tổ hợp. Với $N$ yêu cầu, mỗi yêu cầu có hai trạng thái là chọn hoặc không chọn, số phương án có thể lên tới $2^N$. Khi số lượng yêu cầu tăng, không gian tìm kiếm tăng rất nhanh, khiến việc tìm nghiệm tối ưu trở nên khó khăn nếu chỉ sử dụng phương pháp duyệt vét cạn.

## 2.3. Bài toán thuộc nhóm khó về mặt tính toán

Bài toán cái túi đa chiều là một bài toán tối ưu tổ hợp khó. Khi số lượng yêu cầu và số lượng ràng buộc tăng lên, thời gian tính toán để tìm nghiệm tối ưu cũng tăng mạnh. Trong môi trường điện toán đám mây thực tế, số lượng yêu cầu có thể rất lớn và thay đổi liên tục theo thời gian, do đó hệ thống không chỉ cần tìm nghiệm tốt mà còn phải đưa ra quyết định nhanh.

Nếu thuật toán tối ưu mất quá nhiều thời gian, hệ thống có thể không phản ứng kịp với các yêu cầu mới. Điều này đặc biệt nghiêm trọng trong các hệ thống yêu cầu thời gian thực hoặc gần thời gian thực, nơi quyết định cấp phát tài nguyên cần được thực hiện nhanh chóng.

## 2.4. Nhu cầu tài nguyên thay đổi liên tục

Trong điện toán đám mây, nhu cầu sử dụng tài nguyên không cố định. Lưu lượng truy cập, số lượng người dùng, số lượng tác vụ và khối lượng dữ liệu có thể thay đổi theo giờ, theo ngày hoặc theo các sự kiện đặc biệt. Chẳng hạn, một hệ thống thương mại điện tử có thể cần nhiều tài nguyên hơn trong các đợt khuyến mãi, trong khi một hệ thống học trực tuyến có thể tăng tải vào giờ học cao điểm.

Sự biến động này làm cho việc lập kế hoạch và tối ưu phân bổ tài nguyên trở nên khó khăn. Nếu cấp phát thiếu tài nguyên, hệ thống có thể bị quá tải, làm tăng thời gian phản hồi hoặc gây gián đoạn dịch vụ. Nếu cấp phát quá nhiều tài nguyên, hệ thống sẽ lãng phí và làm tăng chi phí vận hành.

## 2.5. Sự khác nhau về đặc điểm của các yêu cầu

Các yêu cầu trong hệ thống đám mây có thể rất khác nhau về nhu cầu tài nguyên và mức độ ưu tiên. Một yêu cầu có thể là máy ảo phục vụ cơ sở dữ liệu cần nhiều RAM, trong khi yêu cầu khác là container xử lý tính toán cần nhiều CPU. Một dịch vụ truyền thông hoặc streaming lại có thể cần nhiều băng thông mạng.

Sự không đồng nhất này khiến việc so sánh và lựa chọn yêu cầu trở nên khó khăn. Hệ thống cần có tiêu chí đánh giá phù hợp, chẳng hạn giá trị trên mỗi đơn vị tài nguyên, mức độ ưu tiên, doanh thu, thời hạn xử lý hoặc mức độ ảnh hưởng đến chất lượng dịch vụ.

## 2.6. Đảm bảo chất lượng dịch vụ

Trong điện toán đám mây, nhà cung cấp thường phải đảm bảo các cam kết chất lượng dịch vụ như thời gian phản hồi, độ sẵn sàng, độ ổn định và thông lượng xử lý. Nếu việc phân bổ tài nguyên không hợp lý, hệ thống có thể chấp nhận quá nhiều yêu cầu vượt quá khả năng xử lý, dẫn đến giảm hiệu năng và vi phạm SLA.

Vì vậy, bài toán tối ưu không chỉ nhằm tối đa hóa giá trị thu được, mà còn phải đảm bảo các yêu cầu được chấp nhận có thể được phục vụ ổn định trong giới hạn tài nguyên hiện có.

## 2.7. Cân bằng giữa hiệu năng và chi phí

Một trong những mục tiêu quan trọng của quản lý tài nguyên đám mây là cân bằng giữa hiệu năng và chi phí. Nếu hệ thống sử dụng quá ít tài nguyên, hiệu năng có thể bị suy giảm. Nếu duy trì quá nhiều tài nguyên dự phòng, chi phí vận hành sẽ tăng.

Trong bài toán tối ưu với ràng buộc CPU, RAM và băng thông, hệ thống cần tìm phương án phân bổ sao cho tài nguyên được sử dụng hiệu quả nhất. Điều này có nghĩa là hạn chế tài nguyên nhàn rỗi, tránh quá tải và ưu tiên các yêu cầu đem lại giá trị cao so với lượng tài nguyên tiêu thụ.

---

# 3. Hướng giải pháp cho bài toán

## 3.1. Mô hình hóa bài toán dưới dạng cái túi đa chiều

Hướng giải pháp đầu tiên là mô hình hóa bài toán quản lý tài nguyên thành bài toán tối ưu tổ hợp dạng cái túi đa chiều. Mỗi yêu cầu VM hoặc container được xem như một vật phẩm cần lựa chọn. Các tài nguyên CPU, RAM và băng thông được xem như các chiều ràng buộc của cái túi. Giá trị $v_i$ biểu diễn lợi ích khi chấp nhận yêu cầu.

Cách mô hình hóa này giúp bài toán trở nên rõ ràng về mặt toán học. Hệ thống cần tìm vector quyết định:

$$X = (x_1, x_2, ..., x_N)$$

sao cho tổng giá trị:

$$\sum_{i=1}^{N} v_i x_i$$

là lớn nhất, đồng thời không vi phạm các ràng buộc:

$$\sum_{i=1}^{N} c_i x_i \leq C_{max}$$

$$\sum_{i=1}^{N} r_i x_i \leq R_{max}$$

$$\sum_{i=1}^{N} b_i x_i \leq B_{max}$$

Nhờ mô hình này, ta có thể áp dụng các thuật toán tối ưu hóa đã biết để giải quyết bài toán.

## 3.2. Sử dụng thuật toán vét cạn cho bộ dữ liệu nhỏ

Với số lượng yêu cầu nhỏ, có thể sử dụng phương pháp vét cạn để kiểm tra toàn bộ các tổ hợp chọn hoặc không chọn. Phương pháp này đảm bảo tìm được nghiệm tối ưu tuyệt đối vì mọi khả năng đều được xét.

Tuy nhiên, độ phức tạp của phương pháp vét cạn là $O(2^N)$, nên chỉ phù hợp với các bộ dữ liệu nhỏ như $N=5$ hoặc $N=10$. Khi số lượng yêu cầu tăng lên, số tổ hợp cần kiểm tra trở nên rất lớn, khiến phương pháp này không còn hiệu quả trong thực tế.

## 3.3. Sử dụng quy hoạch động khi tài nguyên có miền giá trị vừa phải

Quy hoạch động là một hướng giải phù hợp khi các giới hạn tài nguyên như $C_{max}$, $R_{max}$ và $B_{max}$ không quá lớn. Ý tưởng là xây dựng bảng trạng thái theo số lượng yêu cầu đã xét và lượng tài nguyên đã sử dụng. Mỗi trạng thái lưu giá trị tốt nhất có thể đạt được.

Đối với bài toán có ba ràng buộc CPU, RAM và băng thông, trạng thái có thể được biểu diễn dưới dạng:

$$DP[i][c][r][b]$$

Trong đó, $i$ là số yêu cầu đầu tiên được xét, còn $c$, $r$, $b$ lần lượt là lượng CPU, RAM và băng thông đã sử dụng hoặc còn lại. Công thức chuyển trạng thái sẽ xét hai khả năng: không chọn yêu cầu hiện tại hoặc chọn yêu cầu hiện tại nếu còn đủ tài nguyên.

Ưu điểm của quy hoạch động là có thể tìm nghiệm tối ưu. Tuy nhiên, nhược điểm là bộ nhớ và thời gian tính toán tăng nhanh theo tích của các giới hạn tài nguyên:

$$O(N \times C_{max} \times R_{max} \times B_{max})$$

Do đó, phương pháp này phù hợp với các bài toán có kích thước vừa phải hoặc dữ liệu đã được rời rạc hóa hợp lý.

## 3.4. Sử dụng thuật toán nhánh cận để giảm không gian tìm kiếm

Thuật toán nhánh cận có thể được dùng để tìm nghiệm tối ưu nhưng tránh phải duyệt toàn bộ không gian nghiệm. Ý tưởng là xây dựng cây tìm kiếm, trong đó mỗi mức tương ứng với quyết định chọn hoặc không chọn một yêu cầu. Tại mỗi nhánh, thuật toán tính cận trên của giá trị có thể đạt được. Nếu cận trên của một nhánh không tốt hơn nghiệm hiện tại, nhánh đó sẽ bị loại bỏ.

Phương pháp này giúp giảm đáng kể số lượng phương án cần xét so với vét cạn, đặc biệt khi có chiến lược sắp xếp yêu cầu tốt, chẳng hạn sắp xếp theo tỷ lệ giá trị trên tài nguyên tiêu thụ. Tuy nhiên, trong trường hợp xấu nhất, nhánh cận vẫn có thể có độ phức tạp lớn.

## 3.5. Sử dụng thuật toán tham lam để tìm nghiệm nhanh

Trong các hệ thống cần ra quyết định nhanh, thuật toán tham lam là một lựa chọn thực tế. Thuật toán có thể sắp xếp các yêu cầu theo một tiêu chí ưu tiên, sau đó lần lượt chọn các yêu cầu nếu hệ thống còn đủ CPU, RAM và băng thông.

Một tiêu chí tham lam có thể là:

$$score_i = \frac{v_i}{\alpha c_i + \beta r_i + \gamma b_i}$$

Trong đó $\alpha$, $\beta$, $\gamma$ là các hệ số phản ánh mức độ quan trọng hoặc mức độ khan hiếm của từng loại tài nguyên. Yêu cầu có điểm số cao hơn sẽ được ưu tiên chọn trước.

Ưu điểm của phương pháp tham lam là đơn giản, tốc độ nhanh và dễ triển khai. Nhược điểm là không đảm bảo luôn tìm được nghiệm tối ưu toàn cục, vì quyết định tốt tại từng bước chưa chắc dẫn đến phương án tốt nhất tổng thể.

## 3.6. Kết hợp giám sát tài nguyên theo thời gian thực

Để bài toán tối ưu có ý nghĩa trong môi trường đám mây thực tế, hệ thống cần có cơ chế giám sát tài nguyên theo thời gian thực. Các thông tin cần theo dõi gồm:

- Mức sử dụng CPU hiện tại.
- Mức sử dụng RAM hiện tại.
- Lưu lượng băng thông đang dùng.
- Số lượng VM/container đang chạy.
- Thời gian phản hồi của dịch vụ.
- Số lượng yêu cầu đang chờ xử lý.

Dữ liệu giám sát giúp hệ thống xác định chính xác giá trị hiện tại của $C_{max}$, $R_{max}$ và $B_{max}$ còn khả dụng. Từ đó, thuật toán tối ưu có thể đưa ra quyết định cấp phát phù hợp hơn.

## 3.7. Tự động mở rộng và thu hồi tài nguyên

Bên cạnh việc lựa chọn yêu cầu trong giới hạn tài nguyên hiện có, hệ thống có thể kết hợp cơ chế tự động mở rộng. Khi nhu cầu tăng cao và nhiều yêu cầu có giá trị lớn không thể được phục vụ, hệ thống có thể mở rộng thêm máy ảo, container hoặc node mới để tăng $C_{max}$, $R_{max}$ và $B_{max}$.

Ngược lại, khi tải giảm, hệ thống có thể thu hồi các tài nguyên dư thừa để tiết kiệm chi phí. Cơ chế này giúp hệ thống duy trì sự cân bằng giữa hiệu năng và chi phí vận hành.

## 3.8. Đánh giá nghiệm dựa trên nhiều tiêu chí

Một phương án phân bổ tài nguyên tốt không chỉ là phương án có tổng giá trị lớn nhất, mà còn cần được đánh giá theo nhiều tiêu chí khác như:

- Tổng giá trị thu được.
- Tỷ lệ sử dụng CPU.
- Tỷ lệ sử dụng RAM.
- Tỷ lệ sử dụng băng thông.
- Số lượng yêu cầu được chấp nhận.
- Lượng tài nguyên còn dư.
- Mức độ cân bằng giữa các loại tài nguyên.
- Thời gian chạy của thuật toán.

Việc đánh giá đa tiêu chí giúp so sánh các thuật toán khác nhau và lựa chọn phương pháp phù hợp với từng tình huống thực tế.

---

# 4. Kết luận

_(Nội dung kết luận sẽ được bổ sung)_

---

# 5. Hướng dẫn cài đặt môi trường

## 5.1. Yêu cầu hệ thống

| Thành phần | Yêu cầu tối thiểu | Ghi chú |
|---|---|---|
| **Hệ điều hành** | Windows, Linux hoặc macOS | Windows cần cài MSYS2 để có môi trường tương thích |
| **Trình biên dịch C++** | `g++` hỗ trợ chuẩn **C++17** | GCC ≥ 7.0 hoặc Clang ≥ 5.0 |
| **GNU Make** | `make` ≥ 3.81 | Windows: đi kèm MSYS2; Linux/macOS: có sẵn |
| **Python 3** | Python ≥ 3.6 | Chỉ cần nếu muốn sinh file CSV thống kê |
| **Git** | Bất kỳ phiên bản nào | Tùy chọn — có thể nhận dự án qua file ZIP |

## 5.2. Cài đặt trên Windows

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

**Cách thay thế (không cần MSYS2):** Nếu không muốn cài MSYS2, có thể biên dịch thủ công từng file bằng `g++` (xem [Mục 6.4](#64-chạy-từng-phương-pháp) — phần biên dịch thủ công).

## 5.3. Cài đặt trên Ubuntu / Debian

```bash
# Cập nhật danh sách gói
sudo apt update

# Cài đặt g++, make và git
sudo apt install -y build-essential git

# Cài đặt Python 3 (nếu chưa có)
sudo apt install -y python3
```

## 5.4. Cài đặt trên macOS

```bash
# Cài đặt Xcode Command Line Tools (bao gồm clang++ và make)
xcode-select --install

# Cài đặt Python 3 qua Homebrew (nếu chưa có)
brew install python3
```

## 5.5. Lấy mã nguồn dự án

### Cách 1: Clone từ GitHub (nếu có Git)

```bash
git clone https://github.com/duydua04/daa-project.git
cd daa-project
```

### Cách 2: Tải file ZIP và giải nén

Nếu bạn nhận dự án qua file ZIP (không cần cài Git):

1. Giải nén file ZIP vào một thư mục tùy ý.
2. Mở terminal (MSYS2 trên Windows, Terminal trên Linux/macOS).
3. Di chuyển vào thư mục dự án:

```bash
# Thay đường dẫn phù hợp với vị trí bạn giải nén
cd /đường/dẫn/tới/daa-project
```

> **Trên Windows (MSYS2):** Đường dẫn `C:\Users\TenBan\Desktop\daa-project` tương đương `/c/Users/TenBan/Desktop/daa-project` trong MSYS2 terminal.

## 5.6. Kiểm tra môi trường

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

# 6. Hướng dẫn sử dụng

## 6.1. Cấu trúc dự án

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

## 6.2. Định dạng dữ liệu đầu vào

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

## 6.3. Biên dịch dự án

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

## 6.4. Chạy từng phương pháp

### 6.4.1. Phương pháp Vét cạn (Brute Force)

```bash
# Dùng make
make run-brute-force/cloud_computing

# Chạy thủ công 
./brute-force/cloud_computing          # Linux / macOS / MSYS2
brute-force\cloud_computing.exe        # Windows CMD / PowerShell
```

### 6.4.2. Phương pháp Quy hoạch động (Dynamic Programming)

```bash
# Dùng make
make run-dynamic-programing/cloud_computing

# Chạy thủ công (sau khi đã biên dịch)
./dynamic-programing/cloud_computing   # Linux / macOS / MSYS2
dynamic-programing\cloud_computing.exe # Windows CMD / PowerShell
```

### 6.4.3. Phương pháp Tham lam (Greedy)

```bash
# Dùng make
make run-greedy/cloud_computing

# Chạy thủ công
./greedy/cloud_computing               # Linux / macOS / MSYS2
greedy\cloud_computing.exe             # Windows CMD / PowerShell
```


## 6.5. Chạy tất cả các phương pháp

```bash
make run-all
```

Lệnh này sẽ biên dịch và chạy lần lượt cả 3 phương pháp trên tất cả bộ dữ liệu. Kết quả mỗi phương pháp bao gồm:

- **Maximum Value**: tổng giá trị tối đa đạt được.
- **Selected Items**: danh sách các yêu cầu được chọn.
- **Resource Usage**: mức sử dụng CPU / RAM / Bandwidth (và tỷ lệ %).
- **Memory Usage**: bộ nhớ ước tính (bytes).
- **Execution Time**: thời gian chạy (microseconds).

## 6.6. Sinh file CSV thống kê

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

