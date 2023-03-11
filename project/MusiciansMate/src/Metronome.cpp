#include <Arduino.h>
#include "MusiciansMate.h"

#include "Metronome.h"

Metronome::Metronome(uint8_t buzzerPin, unsigned int tickNote, unsigned long duration, uint32_t tempo)
{
    pinMode(buzzerPin, OUTPUT);

    this->_buzzerPin = buzzerPin;
    this->tickNote = tickNote;
    this->duration = duration;
    this->tempo = tempo;
    this->isPlaying = false;
}

void Metronome::setTempo(int newTempo)
{
    this->tempo = newTempo;
}

int Metronome::getTempo()
{
    return this->tempo;
}

// bool Metronome::buttonHandler()
// {
//     short highCount = 3;

//     while (true)
//         if (digitalRead(joyStickButtonPin) == 1)
//             return true;
// }

bool Metronome::start()
{
    if (isPlaying)
    {
        Serial.println("The Metronome is already playing.");
        return false;
    }

    isPlaying = true;

    while (true)
    {
        // if (digitalRead(joyStickButtonPin) == 0)
        //     buttonHandler();

        tone(_buzzerPin, tickNote, duration);
        delay(SEC_IN_MILLI / tempo);
    }
}

bool Metronome::stop()
{
    isPlaying = false;
}
