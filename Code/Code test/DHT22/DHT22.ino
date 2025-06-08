#include <DHT.h>

#define DHTPIN 6
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("⚠️ Không đọc được cảm biến DHT22!");
  } else {
    Serial.print("Nhiệt độ: ");
    Serial.print(t);
    Serial.print(" °C\t");
    Serial.print("Độ ẩm: ");
    Serial.print(h);
    Serial.println(" %");
  }

  delay(2000);
}
