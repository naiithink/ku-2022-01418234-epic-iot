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


#ifndef MUSICIANS_MATE_H
#define MUSICIANS_MATE_H

#include <stdint.h>

#include "Metronome.h"
#include "Pitches.h"
#include "Tuner.h"


#define SEC_IN_MILLI (1000 * 60)
#define PRINT(MSG) (Serial.printf("%s: %s\n", __func__, MSG))

#define PIECE_TERMINATOR (int) 0


typedef struct track
{
    int note;
    unsigned long duration;
}
track;


void playTrack(uint8_t, track *, uint32_t);

#endif /* MUSICIANS_MATE_H */
