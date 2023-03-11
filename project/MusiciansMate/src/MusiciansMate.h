#ifndef MUSICIANS_MATE_H
#define MUSICIANS_MATE_H

#include <stdint.h>

#include "Metronome.h"
#include "Tuner.h"

#define SEC_IN_MILLI (1000 * 60)

typedef struct track
{
    int note;
    unsigned long duration;
}
track;

void playTrack(uint8_t, track *, uint32_t);

#endif /* MUSICIANS_MATE_H */
