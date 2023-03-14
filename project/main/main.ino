#include <MusiciansMate.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BUZZ_PIN 17
#define JOYSTICK_PIN 16
#define DEFAULT_TEMPO 120

#define NOTE_DURATION (1000 * 1 * 0.18)
#define TUNER_DURATION (1000)

#define DEBOUNCE_DELAY (unsigned long)50

TaskHandle_t InputMonitor;
LiquidCrystal_I2C lcd(0x27, 16, 1);

int joystickState;
int lastJoystickState = 1;
unsigned long lastJoystickDebounceTime = millis();

int metronomeState = -1;
int pageState = -1;

track jingle[] = {
    {NOTE_G7, NOTE_DURATION},
    {NOTE_G7, NOTE_DURATION},
    {NOTE_G7, NOTE_DURATION},
    {NOTE_DS7, 7 * NOTE_DURATION},
    {NOTE_F7, NOTE_DURATION},
    {NOTE_F7, NOTE_DURATION},
    {NOTE_F7, NOTE_DURATION},
    {NOTE_D7, 7 * NOTE_DURATION},
    {0, 0}};

Metronome *metronome = new Metronome(BUZZ_PIN, NOTE_F7, NOTE_DURATION, DEFAULT_TEMPO);

GuitarTuner *tuner = (GuitarTuner *)TunerBuilder::build(BUZZ_PIN, GUITAR, TUNER_DURATION);

void setup()
{
    Serial.begin(115200);

    pinMode(BUZZ_PIN, OUTPUT);
    pinMode(JOYSTICK_PIN, INPUT_PULLUP);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Musicians' Mate");

    // Jingle
    playTrack(BUZZ_PIN, jingle, DEFAULT_TEMPO);

    xTaskCreatePinnedToCore(
        inputMonitorTask,
        "inputMonitor",
        2048,
        NULL,
        1,
        &InputMonitor,
        0);

    Serial.println("Done setup");

    metronome->start();
    metronomeState = metronome->getIsPlaying() == true ? 1
                                                       : -1;
}

void loop()
{
    if (metronome->getIsPlaying() == false && metronomeState > 0 && pageState < 0)
        metronome->start();
    // else if(pageState > 0){
    // }
}

bool metronomeStopObserver()
{
    if (metronomeState > 0)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("[");
        lcd.setCursor(1, 0);
        lcd.print(metronome->getTempo());
        lcd.setCursor(4, 0);
        lcd.print("]");
        lcd.setCursor(6, 0);
        lcd.print("PLAYING");
        return true;
    }
    else
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("[");
        lcd.setCursor(1, 0);
        lcd.print(metronome->getTempo());
        lcd.setCursor(4, 0);
        lcd.print("]");
        lcd.setCursor(6, 0);
        lcd.print("PAUSED");
        return false;
    }
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
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("[");
                lcd.setCursor(1, 0);
                lcd.print(metronome->getTempo());
                lcd.setCursor(4, 0);
                lcd.print("]");
                lcd.setCursor(6, 0);
                lcd.print("PLAYING");
                delay(543);
                Serial.printf("Metronome tempo --: %d\n", metronome->getTempo());
            }
            else if (joystickX >= 4000)
            {
                metronome->setTempo(metronome->getTempo() + 1);
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("[");
                lcd.setCursor(1, 0);
                lcd.print(metronome->getTempo());
                lcd.setCursor(4, 0);
                lcd.print("]");
                lcd.setCursor(6, 0);
                lcd.print("PLAYING");
                delay(543);
                Serial.printf("Metronome tempo ++: %d\n", metronome->getTempo());
            }
        }
        else
        {
            uint16_t joystickX = analogRead(A0);
            uint16_t joystickY = analogRead(A3);

            if (joystickY == 0 && pageState == 1)
            {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("[");
                lcd.setCursor(1, 0);
                lcd.print(metronome->getTempo());
                lcd.setCursor(4, 0);
                lcd.print("]");
                lcd.setCursor(6, 0);
                lcd.print("PAUSED");
                pageState *= -1;
            }
            else if (joystickY >= 4000 && pageState == -1)
            {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("E");
                lcd.setCursor(3, 0);
                lcd.print("A");
                lcd.setCursor(6, 0);
                lcd.print("D");
                lcd.setCursor(9, 0);
                lcd.print("G");
                lcd.setCursor(12, 0);
                lcd.print("B");
                lcd.setCursor(15, 0);
                lcd.print("E");
                pageState *= -1;
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
