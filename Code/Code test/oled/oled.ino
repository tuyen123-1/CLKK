#include <Arduino.h>
#include <U8g2lib.h>

// Giao tiếp I2C với chân SCL = IO7, SDA = IO8
// Dùng phiên bản giao tiếp I2C phần mềm (software I2C)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(
  U8G2_R0,   // Xoay màn hình (R0 = không xoay)
  /* clock=*/ 7,  // SCL = IO7
  /* data=*/ 8,   // SDA = IO8
  /* reset=*/ U8X8_PIN_NONE // Không dùng chân reset
);

void setup() {
  u8g2.begin();               // Khởi tạo màn hình
  u8g2.enableUTF8Print();     // Cho phép in chuỗi UTF-8 nếu cần

  u8g2.clearBuffer();         // Xóa buffer hiển thị
  u8g2.setFont(u8g2_font_ncenB08_tr); // Chọn font
  u8g2.drawStr(0, 15, "ESP32-C3 + OLED");
  u8g2.drawStr(0, 30, "Test IO7/SCL IO8/SDA");
  u8g2.sendBuffer();          // Gửi dữ liệu ra màn hình
}

void loop() {
  // Không cần làm gì thêm
}
