#include <DHT.h>
#include <WiFi.h>
#include <WiFiManager.h>  // Thư viện WiFiManager
#include "time.h"
#include <U8g2lib.h>

class Sensor {
private:
  uint8_t samplingTime = 280;
  uint8_t deltaTime = 40;    // 280 + 40 = 320 us
  uint8_t sleepTime = 9680;  // 10000 - 320 = 9680 us
  float voMeasured = 0;
  float calcVoltage = 0;
  uint8_t vo_pin;
  uint8_t led_pin;
  uint8_t dht_pin;
  uint8_t dht_type;
  DHT dht;
  static const uint8_t DUST_SAMPLE_COUNT = 10;   // Số mẫu để lọc trung bình
  float dustSamples[DUST_SAMPLE_COUNT] = { 0 };  // Khởi tạo mảng mẫu
  uint8_t sampleIndex = 0;
  bool samplesFilled = false;
  uint8_t warmupCount = 10;  // Bỏ qua 10 mẫu đầu
public:
  float temperature = 0;
  float humidity = 0;
  float dustDensity = 0;
  Sensor(uint8_t vo_pin, uint8_t led_pin, uint8_t dht_pin, uint8_t dht_type)
    : vo_pin(vo_pin), led_pin(led_pin), dht(dht_pin, dht_type) {}
  void init() {
    pinMode(led_pin, OUTPUT);
    dht.begin();
  }

  void read() {
    // Đọc cảm biến bụi
    digitalWrite(led_pin, LOW);
    delayMicroseconds(samplingTime);
    voMeasured = analogRead(vo_pin);
    delayMicroseconds(deltaTime);
    digitalWrite(led_pin, HIGH);
    delayMicroseconds(sleepTime);

    // Chuyển đổi sang điện áp
    calcVoltage = voMeasured * (3.3 / 4096.0);

    // Tính mật độ bụi thô
    float rawDustDensity = 170 * calcVoltage - 0.1;

    // Bỏ qua vài mẫu đầu để tránh nhiễu
    if (warmupCount > 0) {
      warmupCount--;
      return;
    }

    // Cắt ngưỡng âm về 0
    if (rawDustDensity < 0) rawDustDensity = 0;

    // Lọc trung bình
    dustSamples[sampleIndex++] = rawDustDensity;
    if (sampleIndex >= DUST_SAMPLE_COUNT) {
      sampleIndex = 0;
      samplesFilled = true;
    }

    float sum = 0;
    uint8_t count = samplesFilled ? DUST_SAMPLE_COUNT : sampleIndex;
    for (uint8_t i = 0; i < count; i++) {
      sum += dustSamples[i];
    }
    dustDensity = sum / count;

    // Đọc nhiệt độ và độ ẩm từ DHT
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    temperature = isnan(t) ? 0 : t;
    humidity = isnan(h) ? 0 : h;
  }
};

class Battery {
private:
  uint8_t battery_pin;
  const float ADC_MAX = 4095.0;             // 12-bit ADC
  const float VREF = 3.3;                   // Điện áp tham chiếu của ESP32
  const float VOLTAGE_DIVIDER_RATIO = 2.0;  // chia áp 100k/100k
  const float MIN_VOL = 3.3;                // Pin gần cạn
  const float MAX_VOL = 4.2;                // Pin đầy
public:
  uint8_t bat_percent = 0;
  float bat_vol = 0;
  uint8_t bat_level = 0;
  Battery(int pin)
    : battery_pin(pin) {}
  void init() {
    analogReadResolution(12);  // Đảm bảo ADC 12-bit (0 - 4095)
  }
  uint8_t getBatPercent() {
    int adcValue = analogRead(battery_pin);
    bat_vol = (adcValue / ADC_MAX) * VREF * VOLTAGE_DIVIDER_RATIO;
    bat_vol = bat_vol < MIN_VOL ? MIN_VOL : (bat_vol > MAX_VOL ? MAX_VOL : bat_vol);
    bat_percent = ((bat_vol - MIN_VOL) * 100) / (MAX_VOL - MIN_VOL);
    return bat_percent;
  }
  uint8_t batPercentToLevel(){
    return (getBatPercent() <= 5) ? 0 : constrain((getBatPercent() - 6) / 19 + 1, 1, 5);
  }
};

// Thông tin máy chủ NTP và múi giờ
const char* ntpServer = "pool.ntp.org";  // Máy chủ NTP chung
const long gmtOffset_sec = 7 * 3600;     // Múi giờ GMT+7 (Việt Nam)
const int daylightOffset_sec = 0;        // vì ở VN luôn là GMT+7

class Time {
private:
  struct tm timeinfo;
public:
  uint8_t day = 0;
  uint8_t month = 0;
  uint16_t year = 0;
  uint8_t hour = 0;
  uint8_t minute = 0;
  uint8_t second = 0;
  uint8_t num_day = 0;
public:
  bool err;
  void init() {
    WiFiManager wm;

    // Bắt đầu cấu hình, nếu chưa có thông tin WiFi thì mở AP cho người dùng cấu hình
    err = wm.autoConnect("ESP32_Setup");

    if (!err) {
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
  void getTime() {
    if (getLocalTime(&timeinfo)) {
      this->day = timeinfo.tm_mday;
      this->month = timeinfo.tm_mon + 1;
      this->year = timeinfo.tm_year + 1900;
      this->hour = timeinfo.tm_hour;
      this->minute = timeinfo.tm_min;
      this->second = timeinfo.tm_sec;
      this->num_day = timeinfo.tm_wday;
    } else {
      Serial.println("Không lấy được thời gian!");
    }
  }
  String getTimeStr(bool option = 0) {
    getTime();
    String d_m_y = String(day) + '/' + String(month) + '/' + String(year);
    String h_m_c = String(hour) + ':' + String(minute) + ':' + String(second);
    String result = option ? d_m_y : h_m_c;
    return result;
  }
};
