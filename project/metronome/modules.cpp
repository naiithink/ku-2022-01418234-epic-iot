#include "modules.h"

Metronome::Metronome(uint8_t pin, unsigned int note, unsigned long duration, int tempo)
{
    this->pin = pin;
    this->note = note;
    this->duration = duration;
    this->tempo = tempo;
}

void Metronome::setTempo(int newTempo)
{
    this->tempo = newTempo;
}

int Metronome::getTempo()
{
    return this->tempo;
}

void Metronome::play()
{
    tone(pin, note, duration);
    delay(SEC_IN_MILLI / tempo);
}
