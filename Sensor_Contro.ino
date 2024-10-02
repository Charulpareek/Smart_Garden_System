#include <LiquidCrystal.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(0, 1);
#define DHTPIN 13
#define DHTTYPE DHT11

const int sensorPin = A1;
const int moisturePin = A0;
const int relayPin = 12;

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // RS, EN, D4, D5, D6, D7
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  pinMode(moisturePin, INPUT);
  pinMode(relayPin, OUTPUT);
  lcd.begin(16, 2);
  dht.begin();
}

void loop() {
  int moistureValue = analogRead(moisturePin);
  int relaystatus = digitalRead(relayPin);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  int sensorValue = analogRead(sensorPin);
  float voltage = sensorValue * (5.0 / 1023.0);
  float temperatureC = (voltage - 0.5) * 100.0;

  lcd.setCursor(0, 0);
  lcd.print("SM:");
  lcd.print(moistureValue);
  lcd.print(" WT:");
  lcd.print(temperatureC);
  delay(1000);

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temperature);
  delay(1000);

  if (relaystatus == HIGH) {
    lcd.print("P:ON");
  } else {
    lcd.print("P:OFF");
  }

  lcd.print(relaystatus);
  lcd.print(" H:");
  lcd.print(humidity);

  if (bluetooth.available()) {
    String receivedData = bluetooth.readString();
    if (receivedData == "status") {
      Serial.println(" ");
      Serial.print("Soil Moisture: ");
      Serial.print(moistureValue);
      delay(1000);
      Serial.println(" ");
      Serial.print("Humidity: ");
      Serial.print(humidity);
      delay(1000);
      Serial.println(" ");
      Serial.print("Temperature: ");
      Serial.print(temperature);
      delay(1000);
      Serial.println(" ");
      Serial.print("Water Temperature: ");
      Serial.print(temperatureC);
      delay(1000);
      Serial.println(" ");

      if (relaystatus == HIGH) {
        Serial.print("Pump Status: ON");
      } else {
        Serial.print("Pump Status: OFF");
      }

      Serial.println(" ");
    } else {
      Serial.print("Enter the proper keyword");
    }
  }

  if (moistureValue <= 500) {
    digitalWrite(relayPin, LOW);
  } else {
    digitalWrite(relayPin, HIGH);
  }
}
