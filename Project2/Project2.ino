#include <Arduino.h>
#include <DHT.h>
#include "NewPing.h"
#include <ESP32Servo.h>
#include "pitches.h"

// ===== Pin definitions (adjust if your lab wiring differs) =====
#define LED_PIN     2
#define DHT_PIN     7
#define TRIG_PIN    4   
#define ECHO_PIN    5
#define BUZZER_PIN  35
#define SERVO_PIN   40   

// ===== DHT setup =====
#define DHT_TYPE DHT11
DHT dht11(DHT_PIN, DHT_TYPE);

// ===== Ultrasonic setup =====
#define MAX_DISTANCE 400
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// ===== Servo setup =====
Servo servo;

// ===== Simple helper for buzzer beep =====
void beepOnce(int freq, int onMs, int offMs) {
  tone(BUZZER_PIN, freq, onMs);
  delay(onMs + offMs);
  noTone(BUZZER_PIN);
}

void setup() {
  Serial.begin(9600);

  dht11.begin();

  servo.attach(SERVO_PIN);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  float humi  = dht11.readHumidity();
  float tempC = dht11.readTemperature();
  float tempF = dht11.readTemperature(true);

  if (isnan(tempC) || isnan(tempF) || isnan(humi)) {
    Serial.println("Failed to read from DHT11 sensor!");
    delay(2000);
    return;
  }

  // === SAFE: tempC <= 30 ===
  if (tempC <= 30) {
    // servo motor stop
    servo.write(90);              // neutral/stop for continuous servo (adjust if needed)
    // Serial Monitor show Safe
    Serial.println("Safe");
    // buzzer no sound
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW);
  }
  // === CAUTION: tempC > 30 AND humi >= 30 ===
  else if (humi >= 30) {
    servo.write(90);              
    Serial.println("Caution");

    // buzzer continuously alarm
    tone(BUZZER_PIN, NOTE_A4);    

    // report heat source distance from Ultrasonic sensor
    float distance = sonar.ping_cm();
    Serial.print("Heat source distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    digitalWrite(LED_PIN, HIGH);
  }
  // === DANGEROUS: tempC > 30 AND humi < 30 ===
  else { // humi < 30
    // servo motor run
    servo.write(0);               // run (for continuous servo; may need 180 depending direction)
    // Serial Monitor show Dangerous
    Serial.println("Dangerous");

    // buzzer beep
    beepOnce(NOTE_C5, 150, 100);

    digitalWrite(LED_PIN, HIGH);
  }

  delay(2000);
}
