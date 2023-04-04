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


void playTrack(uint8_t buzzerPin, track *piece, uint32_t tempo)
{
    PRINT("Playing a track");

    track *head = piece;

    while (head->note != PIECE_TERMINATOR && head->duration != PIECE_TERMINATOR)
    {
        tone(buzzerPin, head->note, head->duration);
        delay(head->duration);
        head++;
    }

    noTone(buzzerPin);

    PRINT("Done playing track");
}
