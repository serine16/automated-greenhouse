#include <Arduino.h>
#include <DHT.h>

#define DHTPIN A5
#define DHTTYPE DHT11

#define ventilateurpin 9

#define trigPin 6
#define echoPin 7
#define buzzerPin 8

DHT dht(DHTPIN, DHTTYPE);

const float hauteur_totale = 15.0;
const float profondeur_alarme = 3.0;

int IN1 = 2;
int IN2 = 10;
int IN3 = 4;
int IN4 = 5;

float sensor1Value = 0;
float sensor2Value = 0;
float sensor3Value = 0;
float sensor4Value = 0;

float temp, hum;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(ventilateurpin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);

  delay(500);
}

void writeDataToSerial() {
  Serial.print(temp);
  Serial.print(",");
  Serial.print(hum);
  Serial.print(",");
  Serial.print(profondeur_eau);
  Serial.print(",");
  Serial.print(sensor1Value);
  Serial.print(",");
  Serial.print(sensor2Value);
  Serial.print(",");
  Serial.print(sensor3Value);
  Serial.print(",");
  Serial.println(sensor4Value);
}

void loop() {
  temp = dht.readTemperature();
  hum = dht.readHumidity();

  // Check and control fan based on temperature
  if (temp > 21) {
    analogWrite(ventilateurpin, 0);
  } else {
    analogWrite(ventilateurpin, 255);
  }

  // Ultrasonic sensor for water level measurement
  long duration;
  float distance, profondeur_eau;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  profondeur_eau = hauteur_totale - distance;

  // Check water level and activate buzzer if too low
  if (profondeur_eau < profondeur_alarme) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  // Read and control the status of each sensor
  sensor1Value = analogRead(A0);
  sensor2Value = analogRead(A1);
  sensor3Value = analogRead(A2);
  sensor4Value = analogRead(A3);

  writeDataToSerial(); // Write data to serial port

  // Control the outputs based on sensor readings
  digitalWrite(IN1, sensor1Value > 450 ? LOW : HIGH);
  digitalWrite(IN2, sensor2Value > 450 ? LOW : HIGH);
  digitalWrite(IN3, sensor3Value > 450 ? LOW : HIGH);
  digitalWrite(IN4, sensor4Value > 450 ? LOW : HIGH);

  delay(5000); // Delay before next iteration
}