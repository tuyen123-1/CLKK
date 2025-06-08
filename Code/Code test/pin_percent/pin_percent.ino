const int PIN_BATTERY = 3;              // GPIO3
const float ADC_MAX = 4095.0;           // 12-bit ADC
const float VREF = 3.3;                 // Điện áp tham chiếu của ESP32 (thường là 3.3V)
const float VOLTAGE_DIVIDER_RATIO = 2.0; // Nếu dùng cầu chia áp 100k/100k

// Ngưỡng điện áp pin Lithium
const float MIN_VOLTAGE = 3.0;          // Pin gần cạn
const float MAX_VOLTAGE = 4.2;          // Pin đầy

void setup() {
  Serial.begin(115200);
  analogReadResolution(12); // Đảm bảo ADC 12-bit (0 - 4095)
}

void loop() {
  int adcValue = analogRead(PIN_BATTERY);
  float voltage = (adcValue / ADC_MAX) * VREF * VOLTAGE_DIVIDER_RATIO;

  // Giới hạn giá trị trong khoảng đo thực tế
  voltage = constrain(voltage, MIN_VOLTAGE, MAX_VOLTAGE);

  // Tính phần trăm pin (xấp xỉ)
  int batteryPercent = map(voltage * 100, MIN_VOLTAGE * 100, MAX_VOLTAGE * 100, 0, 100);
  batteryPercent = constrain(batteryPercent, 0, 100);

  Serial.print("ADC: ");
  Serial.print(adcValue);
  Serial.print("  | Voltage: ");
  Serial.print(voltage, 2);
  Serial.print(" V  | Battery: ");
  Serial.print(batteryPercent);
  Serial.println(" %");

  delay(2000);
}
