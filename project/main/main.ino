#include "thingProperties.h"

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

bool cloudSetupDone = false;

int joystickState;
int lastJoystickState = 1;
unsigned long lastJoystickDebounceTime = millis();

int metronomeState = -1;

// pageStage
//      1: Tuner
//     -1: Metronome
int pageState = -1;
int numArrowTuner = 1;

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
    pinMode(BUZZ_PIN, OUTPUT);
    pinMode(JOYSTICK_PIN, INPUT_PULLUP);

    // Debug console
    Serial.begin(115200);
    // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
    delay(1500);

    initProperties();

    // Connect to Arduino IoT Cloud
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);

    cloudSetupDone = true;

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
    if (cloudSetupDone)
        ArduinoCloud.update();

    if (metronome->getIsPlaying() == false && metronomeState > 0 && pageState == -1)
        metronome->start();
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

void inputMonitorTask(void *param)
{
    while (true)
    {
        vTaskDelay(10);

        if (cloudSetupDone)
            ArduinoCloud.update();

        Serial.printf("X: %i\n", analogRead(A0));
        Serial.printf("Y: %i\n", analogRead(A3));
        Serial.printf("B: %i\n", digitalRead(JOYSTICK_PIN));

        if (metronome->getIsPlaying() == true)
        {
            uint16_t joystickX = analogRead(A0);
            uint16_t joystickY = analogRead(A3);

            if (joystickX == 0)
            {
                decreaseMetronomeTempo();
            }
            else if (joystickX >= 4000)
            {
                increaseMetronomeTempo();
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
                numArrowTuner = 1;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("E");
                lcd.setCursor(numArrowTuner, 0);
                lcd.print("<");
                lcd.setCursor(2, 0);
                lcd.print("A");
                lcd.setCursor(4, 0);
                lcd.print("D");
                lcd.setCursor(6, 0);
                lcd.print("G");
                lcd.setCursor(8, 0);
                lcd.print("B");
                lcd.setCursor(10, 0);
                lcd.print("E");
                pageState *= -1;
            }
            else if (joystickX == 0 && pageState == 1)
            {
                numArrowTuner -= 2;
                delay(1000 / 2);
                if (numArrowTuner <= 1)
                {
                    numArrowTuner = 1;
                }
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("E");
                lcd.setCursor(numArrowTuner, 0);
                lcd.print("<");
                lcd.setCursor(2, 0);
                lcd.print("A");
                lcd.setCursor(4, 0);
                lcd.print("D");
                lcd.setCursor(6, 0);
                lcd.print("G");
                lcd.setCursor(8, 0);
                lcd.print("B");
                lcd.setCursor(10, 0);
                lcd.print("E");
            }
            else if (joystickX >= 4000 && pageState == 1)
            {
                numArrowTuner += 2;
                delay(1000 / 2);
                if (numArrowTuner >= 11)
                {
                    numArrowTuner = 11;
                }
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("E");
                lcd.setCursor(numArrowTuner, 0);
                lcd.print("<");
                lcd.setCursor(2, 0);
                lcd.print("A");
                lcd.setCursor(4, 0);
                lcd.print("D");
                lcd.setCursor(6, 0);
                lcd.print("G");
                lcd.setCursor(8, 0);
                lcd.print("B");
                lcd.setCursor(10, 0);
                lcd.print("E");
            }
        }

        if ((millis() - lastJoystickDebounceTime) < DEBOUNCE_DELAY)
            continue;

        joystickState = digitalRead(JOYSTICK_PIN);

        if (joystickState != lastJoystickState)
        {
            lastJoystickDebounceTime = millis();
            lastJoystickState = joystickState;

            if (joystickState == 1 && pageState == -1)
                toggleMetronomeState();
            else if (joystickState == 1 && pageState == 1)
            {
                playTuner();
            }
        }
    }
}

void toggleMetronomeState()
{
    Serial.println(metronomeState);
    metronomeState *= -1;
}

void increaseMetronomeTempo()
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

void decreaseMetronomeTempo()
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

void playTuner()
{
    // ให้เรียกใช้Tuner ตาม numArrowTuner ที่ชี้อยู่
    if (numArrowTuner == 1)
    {
        tuner->playPitch(0);
    }
    else if (numArrowTuner == 3)
    {
        tuner->playPitch(1);
    }
    else if (numArrowTuner == 5)
    {
        tuner->playPitch(2);
    }
    else if (numArrowTuner == 7)
    {
        tuner->playPitch(3);
    }
    else if (numArrowTuner == 9)
    {
        tuner->playPitch(4);
    }
    else if (numArrowTuner == 11)
    {
        tuner->playPitch(5);
    }
}

/*
  Since OkButton is READ_WRITE variable, onOkButtonChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onOkButtonChange()
{
    // Add your code here to act upon OkButton change
    if (pageState == -1)
        toggleMetronomeState();
    else if (pageState == 1)
        playTuner();
}
/*
  Since UpButton is READ_WRITE variable, onUpButtonChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onUpButtonChange()
{
    // Add your code here to act upon UpButton change
}
/*
  Since DownButton is READ_WRITE variable, onDownButtonChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onDownButtonChange()
{
    // Add your code here to act upon DownButton change
}
/*
  Since LeftButton is READ_WRITE variable, onLeftButtonChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onLeftButtonChange()
{
    // Add your code here to act upon LeftButton change
    if (pageState == -1)
        decreaseMetronomeTempo();
}
/*
  Since RightButton is READ_WRITE variable, onRightButtonChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onRightButtonChange()
{
    // Add your code here to act upon RightButton change
    if (pageState == -1)
        increaseMetronomeTempo();
}
