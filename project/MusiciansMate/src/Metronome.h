#ifndef METRONOME_H
#define METRONOME_H

class Metronome
{
public:
    explicit Metronome(uint8_t, unsigned int, unsigned long, uint32_t);

    bool start();

    bool stop();

    void setTempo(int);

    int getTempo();

private:
    bool buttonHandler();

    uint8_t _buzzerPin;
    unsigned int tickNote;
    unsigned long duration;
    uint32_t tempo;
    bool isPlaying;
    uint8_t joyStickButtonPin;
    uint8_t joyStickXPin;
    uint8_t joyStickYPin;
};

#endif /* METRONOME_H */
