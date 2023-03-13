#include <Arduino.h>
#include "MusiciansMate.h"

#include "Metronome.h"


Metronome::Metronome(uint8_t buzzerPin, unsigned int tickNote, unsigned long tickDuration, uint32_t tempo)
{
    this->_buzzerPin = buzzerPin;
    this->tickNote = tickNote;
    this->tickDuration = tickDuration;
    this->tempo = tempo;
    this->isPlaying = false;
}


bool Metronome::start()
{
    if (isPlaying == true)
    {
        PRINT("The Metronome is already playing");
        return false;
    }

    isPlaying = true;

    while (isPlaying == true && (isPlaying = metronomeStopObserver()) == true)
    {
        tone(_buzzerPin, tickNote, tickDuration);
        delay(SEC_IN_MILLI / tempo);
    }

    return true;
}


bool Metronome::stop()
{
    isPlaying = false;
}


bool Metronome::getIsPlaying()
{
    return this->isPlaying;
}


int Metronome::getTempo()
{
    return this->tempo;
}


void Metronome::setTempo(int newTempo)
{
    this->tempo = newTempo;
}
