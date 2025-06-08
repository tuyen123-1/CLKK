#include "data.h"
#include <U8g2lib.h>

#define LENGTH 128
#define WIDTH 64

#define battery_pin 3
#define vo_pin 4
#define led_pin 5
#define dht_pin 6
#define SCL_PIN 7
#define SDA_PIN 8
#define buzzer_pin 18

U8G2_SSD1306_128X64_NONAME_F_SW_I2C display(U8G2_R0, SCL_PIN, SDA_PIN, U8X8_PIN_NONE);
Sensor sensor(vo_pin, led_pin, dht_pin, DHT22);
Battery battery(battery_pin);
Time t;

const char* dayOfWeek[7] = { "Sun", "Mo", "Tue", "Wed", "Thu", "Fri", "Sat" };

void drawPin(uint8_t option, uint8_t x, uint8_t y);
void drawWifi(uint8_t x, uint8_t y);
void drawStr(uint8_t x, uint8_t y, const String& content);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  sensor.init();
  battery.init();
  t.init();
  display.begin();
  display.setFont(u8g2_font_ncenB08_tr);  // Chọn font: wid 7 hei 10
  display.setFontPosTop();
  display.setFontDirection(0);
}

uint8_t pos_y = 0;
uint8_t pos_y_temp = 27;
uint8_t step = 12;
unsigned long lastReadTime = 0;       // Lưu thời điểm lần đọc trước
const unsigned long interval = 2000;  // Khoảng thời gian giữa các lần đọc (2 giây)

void loop() {

  unsigned long currentTime = millis();
  static String tmp = "", hum = "", dust = "";

  if (currentTime - lastReadTime >= interval) {
    lastReadTime = currentTime;
    sensor.read();  // đọc dữ liệu cảm biến
    tmp = "Temp: " + String(sensor.temperature) + " oC";
    hum = "Hum: " + String(sensor.humidity) + " %";
    dust = "Dust: " + String(sensor.dustDensity) + " ug/m3";
  }
  String hour = t.getTimeStr();
  String date = t.getTimeStr(1);
  String dayOfWeekStr = dayOfWeek[t.num_day];
  String bat_per = String(battery.getBatPercent()) + "%";
  String bat_voltage = String(battery.bat_vol) + "V";

  display.clearBuffer();

  display.setFont(u8g2_font_ncenB08_tr);
  drawStr(0, 0, dayOfWeekStr);
  drawStr(30, pos_y, date);
  drawStr(0, pos_y_temp, tmp);
  drawStr(0, pos_y_temp + step, hum);
  drawStr(0, pos_y_temp + step * 2, dust);

  display.setFont(u8g2_font_ncenB10_tr);
  drawStr(10, pos_y + step, hour);

  display.setFont(u8g2_font_t0_11_me);
  drawStr(100, 12, bat_per);
  drawStr(95, 24, bat_voltage);
  drawPin(battery.batPercentToLevel(), 128, 0);
  drawWifi(110,64);

  display.sendBuffer();

  Serial.print(dayOfWeekStr + " | ");
  Serial.print(date + " | ");
  Serial.print(hour + " | ");
  Serial.print(tmp + " | ");
  Serial.print(hum + " | ");
  Serial.println(dust);
  Serial.print(bat_per + " | ");
  Serial.println(bat_voltage);
}

void drawPin(uint8_t option, uint8_t x, uint8_t y) {
	uint16_t code = 0;
	switch (option) {
		case 0: code = 0x0030; break;
		case 1: code = 0x0031; break;
		case 2: code = 0x0032; break;
		case 3: code = 0x0033; break;
		case 4: code = 0x0034; break;
		case 5: code = 0x0035; break;
	}		
	display.setFont(u8g2_font_battery19_tn);
	display.setFontDirection(1);
	display.drawGlyph(x, y, code);
  display.setFontDirection(0);
}

void drawWifi(uint8_t x, uint8_t y){
  display.setFont(u8g2_font_open_iconic_all_2x_t);
	display.setFontDirection(3);
	display.drawGlyph(x, y, 0x00f7);
  display.setFontDirection(0);
}

void drawStr(uint8_t x, uint8_t y, const String& content){
  display.drawStr(x,y,content.c_str());
}