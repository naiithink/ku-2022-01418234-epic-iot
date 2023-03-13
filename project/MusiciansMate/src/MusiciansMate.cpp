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
