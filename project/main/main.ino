#include <MusiciansMate.h>

#define BUZZ_PIN 17
#define JOYSTICK_PIN 16
#define DEFAULT_TEMPO 120

#define NOTE_DURATION (1000 * 1 * 0.18)
#define TUNER_DURATION (1000)

#define DEBOUNCE_DELAY (unsigned long) 50

TaskHandle_t InputMonitor;

int joystickState;
int lastJoystickState = 1;
unsigned long lastJoystickDebounceTime = millis();

int metronomeState = -1;

track jingle[] = {
    { NOTE_G7,      NOTE_DURATION },
    { NOTE_G7,      NOTE_DURATION },
    { NOTE_G7,      NOTE_DURATION },
    { NOTE_DS7,     7 * NOTE_DURATION },
    { NOTE_F7,      NOTE_DURATION },
    { NOTE_F7,      NOTE_DURATION },
    { NOTE_F7,      NOTE_DURATION },
    { NOTE_D7,      7 * NOTE_DURATION },
    { 0,            0 }
};

Metronome *metronome = new Metronome(BUZZ_PIN, NOTE_F7, NOTE_DURATION, DEFAULT_TEMPO);

GuitarTuner *tuner = (GuitarTuner *) TunerBuilder::build(BUZZ_PIN, GUITAR, TUNER_DURATION);

void setup()
{
    Serial.begin(115200);

    pinMode(BUZZ_PIN, OUTPUT);
    pinMode(JOYSTICK_PIN, INPUT_PULLUP);

    // Jingle
    playTrack(BUZZ_PIN, jingle, DEFAULT_TEMPO);

    xTaskCreatePinnedToCore(
        inputMonitorTask,
        "inputMonitor",
        2048,
        NULL,
        1,
        &InputMonitor,
        0
    );

    Serial.println("Done setup");

    metronome->start();
    metronomeState = metronome->getIsPlaying() == true  ? 1
                                                        : -1;
}

void loop()
{
    if (metronome->getIsPlaying() == false && metronomeState > 0)
        metronome->start();
}

bool metronomeStopObserver()
{
    if (metronomeState > 0)
        return true;
    else
        return false;
}

void toggleMetronomeState()
{
    Serial.println(metronomeState);
    metronomeState *= -1;
}

void inputMonitorTask(void *param)
{
    while (true)
    {
        vTaskDelay(10);

        Serial.printf("X: %i\n", analogRead(A0));
        Serial.printf("Y: %i\n", analogRead(A3));
        Serial.printf("B: %i\n", digitalRead(JOYSTICK_PIN));

        if (metronome->getIsPlaying() == true)
        {
            uint16_t joystickX = analogRead(A0);
            uint16_t joystickY = analogRead(A3);

            if (joystickX == 0)
            {
                metronome->setTempo(metronome->getTempo() - 1);
                delay(200);
                Serial.printf("Metronome tempo --: %d\n", metronome->getTempo());
            }
            else if (joystickX >= 4000)
            {
                metronome->setTempo(metronome->getTempo() + 1);
                delay(200);
                Serial.printf("Metronome tempo ++: %d\n", metronome->getTempo());
            }
        }

        if ((millis() - lastJoystickDebounceTime) < DEBOUNCE_DELAY)
            continue;

        joystickState = digitalRead(JOYSTICK_PIN);

        if (joystickState != lastJoystickState)
        {
            lastJoystickDebounceTime = millis();
            lastJoystickState = joystickState;

            if (joystickState == 1)
                toggleMetronomeState();
        }
    }
}
