#ifndef MODULES_H
#define MODULES_H

#define SEC_IN_MILLI (1000 * 60)

class Metronome
{
public:
    explicit Metronome(uint8_t, unsigned int, unsigned long, int);

    void play();

    void setTempo(int);

    int getTempo();

private:
    uint8_t pin;

    unsigned int note;

    unsigned long duration;

    int tempo;
};

#endif /* MODULES_H */
