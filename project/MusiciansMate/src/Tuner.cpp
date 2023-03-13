#include <Arduino.h>
#include "MusiciansMate.h"
#include "Pitches.h"

#include "Tuner.h"


Tuner::Tuner(uint8_t buzzerPin, unsigned long playDuration)
{
    this->_buzzerPin = buzzerPin;
    this->playDuration = playDuration;
}


GuitarTuner::GuitarTuner(uint8_t _buzzerPin, unsigned long playDuration)
: Tuner::Tuner(_buzzerPin, playDuration)
{
    // pinMode(_buzzerPin, OUTPUT);

    this->_buzzerPin = _buzzerPin;
    this->type = GUITAR;
    this->pitchCount = 6;
    this->playDuration = playDuration;
    this->pitches = new int[pitchCount] {
        // NOTE_E2,
        // NOTE_A2,
        // NOTE_D3,
        // NOTE_G3,
        // NOTE_B3,
        // NOTE_E4
        NOTE_E5,
        NOTE_A5,
        NOTE_D6,
        NOTE_G6,
        NOTE_B6,
        NOTE_E7
    };
}


bool GuitarTuner::playPitch(const int index)
{
    if (index < 0 || index > pitchCount)
    {
        PRINT("Pitch index out of range");
        return false;
    }

    tone(_buzzerPin, pitches[index], playDuration);
    delay(playDuration);
    noTone(_buzzerPin);

    return true;
}

Tuner *TunerBuilder::build(uint8_t buzzerPin, instrument type, unsigned long playDuration)
{
    Tuner *tuner;

    switch (type)
    {
        case GUITAR:
            tuner = (Tuner *) new GuitarTuner(buzzerPin, playDuration);
            break;
        default:
            PRINT("Unknown instrument");
    }

    return tuner;
}
