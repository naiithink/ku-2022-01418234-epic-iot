#include <Arduino.h>

#include "MusiciansMate.h"

#define PIECE_TERMINATOR (int) 0

void playTrack(uint8_t buzzerPin, track *piece, uint32_t tempo)
{
    Serial.println("Playing a track");

    track *head = piece;

    while (head->note != PIECE_TERMINATOR && head->duration != PIECE_TERMINATOR)
    {
        tone(buzzerPin, head->note, head->duration);
        head++;
    }

    noTone(buzzerPin);
}
