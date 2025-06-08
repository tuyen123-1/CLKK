#include <WiFi.h>
#include <WiFiManager.h>  // Thư viện WiFiManager
#include "time.h"

// Thông tin máy chủ NTP và múi giờ
const char* ntpServer = "pool.ntp.org";         // Máy chủ NTP chung
const long  gmtOffset_sec = 7 * 3600;           // Múi giờ GMT+7 (Việt Nam)
const int   daylightOffset_sec = 0;             // Không dùng giờ mùa hè ở VN

void setup() {
  Serial.begin(115200);

  WiFiManager wm;

  // Bắt đầu cấu hình, nếu chưa có thông tin WiFi thì mở AP cho người dùng cấu hình
  bool res = wm.autoConnect("ESP32_Setup");

  if (!res) {
    Serial.println("Kết nối thất bại, khởi động lại...");
    ESP.restart();
  } else {
    Serial.println("Đã kết nối WiFi!");
    Serial.println(WiFi.localIP());
  }
  // Đồng bộ thời gian từ NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Đang lấy thời gian từ NTP...");
}

void loop() {
  // xử lý chính ở đây
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println(timeinfo.tm_min);
  } else {
    Serial.println("Lỗi lấy thời gian.");
  }
  delay(1000); // in thời gian mỗi giây
}
