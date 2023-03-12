#ifndef METRONOME_H
#define METRONOME_H


extern bool metronomeStopObserver();


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

    unsigned long tickDuration;

    // The current metronome tempo
    uint32_t tempo;

    // Current metronome state
    // Set either by the external `metronomeStopObserver()' function or by the `Metronome::stop()' method
    bool isPlaying;
};

#endif /* METRONOME_H */
