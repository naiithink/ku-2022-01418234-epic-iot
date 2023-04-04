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


#ifndef METRONOME_H
#define METRONOME_H


extern bool metronomeStopObserver();


class Metronome
{
public:
    explicit Metronome(uint8_t, unsigned int, unsigned long, uint32_t);

    bool start();

    bool stop();

    bool getIsPlaying();

    int getTempo();

    void setTempo(int);

private:
    uint8_t _buzzerPin;

    unsigned int tickNote;

    unsigned long tickDuration;

    // The current metronome tempo
    uint32_t tempo;

    // Current metronome state
    // Set either by the external `metronomeStopObserver()' function or by the `Metronome::stop()' method
    bool isPlaying;
};

#endif /* METRONOME_H */
