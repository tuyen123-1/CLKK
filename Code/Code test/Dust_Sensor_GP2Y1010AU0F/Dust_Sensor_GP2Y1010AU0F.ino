//https://howmuchsnow.com/arduino/airquality/
//PM2.5
#define measurePin 4
int ledPower = 5;
int samplingTime = 280;
int deltaTime = 40; // 280 + 40 = 320 us
int sleepTime = 9680; // 10000 - 320 = 9680 us
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPower, OUTPUT);
}
void loop() {
  digitalWrite(ledPower, LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin);
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH);
  delayMicroseconds(sleepTime);
  calcVoltage = voMeasured * 5.0 / 4095.0;
  dustDensity = 170 * calcVoltage - 0.1;
  Serial.print(dustDensity);  // unit: ug/m3
  Serial.println(" ug/m3 ");
  delay(1000);
}