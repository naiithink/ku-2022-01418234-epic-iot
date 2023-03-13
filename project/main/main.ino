#include <MusiciansMate.h>

#include "Pitches.h"

#define BUZZ_PIN 17
#define JOYSTICK_PIN 16
#define DEFAULT_TEMPO 120

#define NOTE_DURATION (1000 * 1 * 0.18)
#define TUNER_DURATION (1000)

int musicToolsBuzzerPin = BUZZ_PIN;

int joystickState;

int cnt;

int rnd;

// track jingle[] = {
//     { NOTE_DS7,     NOTE_DURATION },
//     { NOTE_DS7,     NOTE_DURATION },
//     { NOTE_DS7,     NOTE_DURATION },
//     { NOTE_DS7,     NOTE_DURATION },
//     { NOTE_CS7,     NOTE_DURATION },
//     { NOTE_B6,      NOTE_DURATION },
//     { NOTE_B6,      NOTE_DURATION },
//     { 0,            0 }
// };

track jingle[] = {
    { NOTE_G7,      NOTE_DURATION },
    { NOTE_G7,      NOTE_DURATION },
    { NOTE_G7,      NOTE_DURATION },
    { NOTE_DS7,     7 * NOTE_DURATION },
    { NOTE_F7,      NOTE_DURATION },
    { NOTE_F7,      NOTE_DURATION },
    { NOTE_F7,      NOTE_DURATION },
    { NOTE_D7,      7 * NOTE_DURATION },
    { 0,            0 }
};

Metronome *metronome = new Metronome(BUZZ_PIN, NOTE_F7, NOTE_DURATION, DEFAULT_TEMPO);

GuitarTuner *tuner = (GuitarTuner *) TunerBuilder::build(BUZZ_PIN, GUITAR, TUNER_DURATION);

void setup()
{
    Serial.begin(115200);

    pinMode(BUZZ_PIN, OUTPUT);
    // pinMode(JOYSTICK_PIN, INPUT_PULLUP);

    Serial.println("Done setup");

    // Jingle
    playTrack(BUZZ_PIN, jingle, DEFAULT_TEMPO);

    metronome->start();
}

void loop()
{
    if (cnt >= 9 && rnd < 1)
    {
        metronome->setTempo(90);
        cnt = 0;
        metronome->start();
        rnd++;
    }
    else if (rnd < 2)
    {
        int i = 0;

        while (tuner->playPitch(i++));

        rnd++;
    }
    else
    {
        while (true);
    }
}

bool metronomeStopObserver()
{
    if (cnt++ < 10)
        return true;
    else
        return false;
}
