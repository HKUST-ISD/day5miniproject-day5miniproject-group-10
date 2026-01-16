/* replace ? and add code in ???*/
#include "pitches.h"
#include "Arduino.h"
#include "NewPing.h"

// define ultrasonic pins
#define TRIG_PIN   4       
#define ECHO_PIN   5     

// define buzzer pin ？
#define BUZZER_PIN  35   

//Define Built-in LED pin
#define LED_PIN 2      

// Maximum distance we want to ping for (in centimeters).
#define MAX_DISTANCE 400

// NewPing setup of pins and maximum distance.
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

//*** Define distance as a variable ***
float distance;  

//jingle bells high_tone

int melody_high[] = {
  
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4,
  NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4,
  NOTE_D4, NOTE_G4
};

int noteDurations_high[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8, 2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};
  //jingle bells LOW
int melody_low[] = {

  NOTE_E3, NOTE_E3, NOTE_E3,
  NOTE_E3, NOTE_E3, NOTE_E3,
  NOTE_E3, NOTE_G3, NOTE_C3, NOTE_D3, NOTE_E3,
  NOTE_F3, NOTE_F3, NOTE_F3, NOTE_F3,
  NOTE_F3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3,
  NOTE_E3, NOTE_D3, NOTE_D3, NOTE_E3,
  NOTE_D3, NOTE_G3
};

int noteDurations_low[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8, 2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};


void setup() {

Serial.begin(9600);

pinMode(LED_PIN, OUTPUT);
pinMode(BUZZER_PIN, OUTPUT);
 
  
}

void loop() {
  // Get the distance from the Ultrasonic Sensor
  distance = sonar.ping_cm();  // 0 sometimes means "no echo"

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 10 || distance == 0) {
    // turn on built in LED
    digitalWrite(LED_PIN, HIGH);

  } else if (distance <= 10 && distance >= 5) {
    // turn off built in LED
    digitalWrite(LED_PIN, LOW);

    // play high tone Jingle-Bells
    int notes = min(
      (int)(sizeof(melody_high) / sizeof(melody_high[0])),
      (int)(sizeof(noteDurations_high) / sizeof(noteDurations_high[0]))
    );

    for (int thisNote = 0; thisNote < notes; thisNote++) {
      int noteDuration = 1000 / noteDurations_high[thisNote];
      tone(BUZZER_PIN, melody_high[thisNote], noteDuration);
      delay((int)(noteDuration * 1.30));
      noTone(BUZZER_PIN);
    }

  } else if (distance < 5) {
    // turn off built in LED
    digitalWrite(LED_PIN, LOW);

    // play low tone Jingle-Bells
    int notes = min(
      (int)(sizeof(melody_low) / sizeof(melody_low[0])),
      (int)(sizeof(noteDurations_low) / sizeof(noteDurations_low[0]))
    );

    for (int thisNote = 0; thisNote < notes; thisNote++) {
      int noteDuration = 1000 / noteDurations_low[thisNote];
      tone(BUZZER_PIN, melody_low[thisNote], noteDuration);
      delay((int)(noteDuration * 1.30));
      noTone(BUZZER_PIN);
    }
  }

  delay(200); // small pause so distance reading isn't spammy
}