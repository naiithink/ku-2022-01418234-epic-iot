#include "pitches.h"

#define BUZZ_PIN 16
#define JOYSTICK_PIN 17

unsigned long tempo = 120;
int joystickState;

void setup() {
    Serial.begin(115200);

    pinMode(BUZZ_PIN, OUTPUT);
    pinMode(JOYSTICK_PIN, INPUT_PULLUP);

    Serial.println("Done setup");

    // Jingle
    tone(BUZZ_PIN, NOTE_DS7, 1000 * 1 * 0.18);
    tone(BUZZ_PIN, NOTE_DS7, 1000 * 1 * 0.18);
    tone(BUZZ_PIN, NOTE_DS7, 1000 * 1 * 0.18);
    tone(BUZZ_PIN, NOTE_DS7, 1000 * 1 * 0.18);
    tone(BUZZ_PIN, NOTE_CS7, 1000 * 1 * 0.18);
    tone(BUZZ_PIN, NOTE_B6, 1000 * 1 * 0.18);
    tone(BUZZ_PIN, NOTE_B6, 1000 * 1 * 0.18);

    noTone(BUZZ_PIN);
} 

void loop() {
    tone(BUZZ_PIN, NOTE_C7, 100);

    delay((1000 * 60) / tempo);
}
