# VXL_Lab_3_HCMUT

Bài thực hành **Lab 3: Buttons & Switches** môn **Vi xử lý - Vi điều khiển (HCMUT)** — hệ thống đèn giao thông 2 tuyến đường sử dụng vi điều khiển **STM32F103C6**, 3 nút nhấn và 4 LED 7 đoạn quét.

## Cấu trúc thư mục

- `lab3/`: Project STM32CubeIDE
  - `lab3/Core/Src/main.c`: Khởi tạo hệ thống, GPIO, `TIM2` và vòng lặp chính gọi FSM.
  - `lab3/Core/Src/timer.c`: Ngắt `TIM2` chu kỳ `10 ms` phục vụ đọc nút nhấn, quét LED 7 đoạn (`50 ms`) và tạo các cờ thời gian (`1s`, `500ms`, `2Hz`).
  - `lab3/Core/Src/input_reading.c`: Đọc và chống dội 3 nút nhấn (hỗ trợ phát hiện nhấn giữ `>= 1s`).
  - `lab3/Core/Src/input_processing.c`: Máy trạng thái (FSM) điều khiển đèn giao thông và các chế độ cài đặt thời gian.
  - `lab3/Core/Src/led_display.c`: Điều khiển đèn giao thông và quét 4 LED 7 đoạn.
  - `lab3/Debug/lab3.hex`: File `.hex` biên dịch sẵn để nạp chạy mô phỏng Proteus.
- `Lab3.pdsprj`: File mô phỏng mạch trên Proteus.
- `VXL_VDK_Lab_3_Buttons-Switches.pdf`: Đề bài và tài liệu hướng dẫn Lab 3.

## Phần cứng & Sơ đồ chân (STM32F103C6Ux)

- **Clock & Timer:** HSI 8 MHz; `TIM2` (`Prescaler = 7999`, `Period = 9`) tạo ngắt định kỳ mỗi `10 ms`.
- **Đèn giao thông (`GPIOA`):**
  - Tuyến 1: `PA1` (`RED_1`), `PA2` (`YELLOW_1`), `PA3` (`GREEN_1`)
  - Tuyến 2: `PA4` (`RED_2`), `PA5` (`YELLOW_2`), `PA6` (`GREEN_2`)
- **LED 7 đoạn & chân quét (`GPIOB`, active-low):**
  - Các đoạn `a`–`g`: `PB0`–`PB6` (`SEG_A`–`SEG_G`)
  - Chân chọn LED: `PB7` (`EN0`), `PB8` (`EN1`), `PB9` (`EN2`), `PB10` (`EN3`)
- **Nút nhấn (`GPIOB`, Pull-up, active-low):**
  - `PB12` (`BUTTON_1`): Chuyển chế độ (`Mode 1 -> 2 -> 3 -> 4 -> 1`)
  - `PB13` (`BUTTON_2`): Tăng thời gian (`1`–`99s`; nhấn giữ `>= 1s` tự động tăng mỗi `500 ms`)
  - `PB14` (`BUTTON_3`): Lưu giá trị thời gian đang chỉnh

## Chế độ hoạt động

1. **Mode 1 — Normal:** Chạy chu trình đèn giao thông 2 tuyến (mặc định Đỏ `5s`, Vàng `2s`, Xanh `3s`) và hiển thị đếm ngược trên 4 LED 7 đoạn. Khi vào Mode 1, hệ thống đảm bảo `time_red = time_green + time_yellow`.
2. **Mode 2 — Modify Red:** LED Đỏ nhấp nháy `2 Hz`; LED 7 đoạn hiển thị `02` và giá trị thời gian đang chỉnh.
3. **Mode 3 — Modify Yellow:** LED Vàng nhấp nháy `2 Hz`; LED 7 đoạn hiển thị `03` và giá trị thời gian đang chỉnh.
4. **Mode 4 — Modify Green:** LED Xanh nhấp nháy `2 Hz`; LED 7 đoạn hiển thị `04` và giá trị thời gian đang chỉnh.

## Chạy mô phỏng & Build

- **Mô phỏng nhanh:** Mở `Lab3.pdsprj` bằng Proteus (file chương trình trỏ tới `lab3/Debug/lab3.hex`).
- **Chỉnh sửa & Build:** Mở thư mục `lab3/` trong **STM32CubeIDE**, chỉnh sửa mã nguồn và build lại để cập nhật `lab3/Debug/lab3.hex`.

Bài thực hành **Lab 3: Buttons & Switches** môn **Vi xử lý - Vi điều khiển (HCMUT)** — hệ thống đèn giao thông 2 tuyến đường sử dụng vi điều khiển **STM32F103C6**, 3 nút nhấn và 4 LED 7 đoạn quét.

## Cấu trúc thư mục

- `lab3/`: Project STM32CubeIDE
  - `lab3/Core/Src/main.c`: Khởi tạo hệ thống, GPIO, `TIM2` và vòng lặp chính gọi FSM.
  - `lab3/Core/Src/timer.c`: Ngắt `TIM2` chu kỳ `10 ms` phục vụ đọc nút nhấn, quét LED 7 đoạn (`50 ms`) và tạo các cờ thời gian (`1s`, `500ms`, `2Hz`).
  - `lab3/Core/Src/input_reading.c`: Đọc và chống dội 3 nút nhấn (hỗ trợ phát hiện nhấn giữ `>= 1s`).
  - `lab3/Core/Src/input_processing.c`: Máy trạng thái (FSM) điều khiển đèn giao thông và các chế độ cài đặt thời gian.
  - `lab3/Core/Src/led_display.c`: Điều khiển đèn giao thông và quét 4 LED 7 đoạn.
  - `lab3/Debug/lab3.hex`: File `.hex` biên dịch sẵn để nạp chạy mô phỏng Proteus.
- `Lab3.pdsprj`: File mô phỏng mạch trên Proteus.
- `VXL_VDK_Lab_3_Buttons-Switches.pdf`: Đề bài và tài liệu hướng dẫn Lab 3.

## Phần cứng & Sơ đồ chân (STM32F103C6Ux)

- **Clock & Timer:** HSI 8 MHz; `TIM2` (`Prescaler = 7999`, `Period = 9`) tạo ngắt định kỳ mỗi `10 ms`.
- **Đèn giao thông (`GPIOA`):**
  - Tuyến 1: `PA1` (`RED_1`), `PA2` (`YELLOW_1`), `PA3` (`GREEN_1`)
  - Tuyến 2: `PA4` (`RED_2`), `PA5` (`YELLOW_2`), `PA6` (`GREEN_2`)
- **LED 7 đoạn & chân quét (`GPIOB`, active-low):**
  - Các đoạn `a`–`g`: `PB0`–`PB6` (`SEG_A`–`SEG_G`)
  - Chân chọn LED: `PB7` (`EN0`), `PB8` (`EN1`), `PB9` (`EN2`), `PB10` (`EN3`)
- **Nút nhấn (`GPIOB`, Pull-up, active-low):**
  - `PB12` (`BUTTON_1`): Chuyển chế độ (`Mode 1 -> 2 -> 3 -> 4 -> 1`)
  - `PB13` (`BUTTON_2`): Tăng thời gian (`1`–`99s`; nhấn giữ `>= 1s` tự động tăng mỗi `500 ms`)
  - `PB14` (`BUTTON_3`): Lưu giá trị thời gian đang chỉnh

## Chế độ hoạt động

1. **Mode 1 — Normal:** Chạy chu trình đèn giao thông 2 tuyến (mặc định Đỏ `5s`, Vàng `2s`, Xanh `3s`) và hiển thị đếm ngược trên 4 LED 7 đoạn. Khi vào Mode 1, hệ thống đảm bảo `time_red = time_green + time_yellow`.
2. **Mode 2 — Modify Red:** LED Đỏ nhấp nháy `2 Hz`; LED 7 đoạn hiển thị `02` và giá trị thời gian đang chỉnh.
3. **Mode 3 — Modify Yellow:** LED Vàng nhấp nháy `2 Hz`; LED 7 đoạn hiển thị `03` và giá trị thời gian đang chỉnh.
4. **Mode 4 — Modify Green:** LED Xanh nhấp nháy `2 Hz`; LED 7 đoạn hiển thị `04` và giá trị thời gian đang chỉnh.

## Chạy mô phỏng & Build

- **Mô phỏng nhanh:** Mở `Lab3.pdsprj` bằng Proteus (file chương trình trỏ tới `lab3/Debug/lab3.hex`).
- **Chỉnh sửa & Build:** Mở thư mục `lab3/` trong **STM32CubeIDE**, chỉnh sửa mã nguồn và build lại để cập nhật `lab3/Debug/lab3.hex`.
