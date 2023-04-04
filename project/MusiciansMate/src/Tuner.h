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


#ifndef TUNER_H
#define TUNER_H

typedef enum
{
    GUITAR = 1
}
instrument;


class Tuner
{
public:
    virtual bool playPitch(const int);

protected:
    explicit Tuner(uint8_t, unsigned long);

    uint8_t _buzzerPin;

    instrument type;

    int pitchCount;

    unsigned long playDuration;

    int *pitches;
};


class TunerBuilder
{
public:
    static Tuner *build(uint8_t, instrument, unsigned long);

private:
    TunerBuilder();
};


class GuitarTuner : protected Tuner
{
public:
    explicit GuitarTuner(uint8_t, unsigned long);

    bool playPitch(const int);
};


#endif /* TUNER_H */
