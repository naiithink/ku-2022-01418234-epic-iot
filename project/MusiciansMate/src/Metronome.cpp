/*
 * Epic IoT's implementation of Musicians' Mate Arduino Library in C++
 * Copyright (C) 2023 The Epic IoT Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


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
