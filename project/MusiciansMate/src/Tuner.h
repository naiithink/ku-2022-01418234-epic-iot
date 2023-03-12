#ifndef TUNER_H
#define TUNER_H

typedef enum
{
    GUITAR = 1
} instrument;


class Tuner
{
public:
    virtual bool playPitch(const int);

protected:
    explicit Tuner(uint8_t,  unsigned long);

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
    bool playPitch(const int);

protected:
    explicit GuitarTuner(uint8_t, unsigned long);
};


#endif /* TUNER_H */
