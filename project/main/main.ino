#include <MusiciansMate.h>

#include "Pitches.h"

#define BUZZ_PIN 17
#define JOYSTICK_PIN 16
#define DEFAULT_TEMPO 120

#define NOTE_DURATION (1000 * 1 * 0.18)

int musicToolsBuzzerPin = BUZZ_PIN;

int joystickState;

track jingle[] = {
    { NOTE_DS7,     NOTE_DURATION },
    { NOTE_DS7,     NOTE_DURATION },
    { NOTE_DS7,     NOTE_DURATION },
    { NOTE_DS7,     NOTE_DURATION },
    { NOTE_CS7,     NOTE_DURATION },
    { NOTE_B6,      NOTE_DURATION },
    { NOTE_B6,      NOTE_DURATION },
    { 0,            0 }
};

void setup()
{
    Serial.begin(115200);

    pinMode(BUZZ_PIN, OUTPUT);
    // pinMode(JOYSTICK_PIN, INPUT_PULLUP);

    Serial.println("Done setup");

    // Jingle
    playTrack(BUZZ_PIN, jingle, DEFAULT_TEMPO);

    Serial.println(xPortGetCoreID());
}

void loop()
{
    // tone(BUZZ_PIN, NOTE_C7, 100);

    // delay((1000 * 60) / tempo);
}
