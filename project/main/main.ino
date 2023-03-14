#include <MusiciansMate.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPLAllvhdfB"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "3_95z6wnvK7gr9qP2j72ZGALA3osxOTt"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define BUZZ_PIN 17
#define JOYSTICK_PIN 16
#define DEFAULT_TEMPO 120

#define NOTE_DURATION (1000 * 1 * 0.18)
#define TUNER_DURATION (1000)

#define DEBOUNCE_DELAY (unsigned long)50

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "TEE";
char pass[] = "Tee00000";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
    // Set incoming value from pin V0 to a variable
    int value = param.asInt();

    // Update state
    Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
    // Change Web Link Button message to "Congratulations!"
    Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
    Blynk.setProperty(V3, "onImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
    Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
    // You can send any value at any time.
    // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V2, millis() / 1000);
}

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
    // Debug console
    Serial.begin(115200);

    // You can also specify server:
    // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
    // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

    // Setup a function to be called every second
    timer.setInterval(1000L, myTimerEvent);

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
                lcd.setCursor(2, 0);
                lcd.print("A");
                lcd.setCursor(4, 0);
                lcd.print("D");
                lcd.setCursor(6, 0);
                lcd.print("G");
                lcd.setCursor(8, 0);
                lcd.print("B");
                lcd.setCursor(9, 0);
                lcd.print("E");
                pageState *= -1;
                stageJuner();
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

void stageJuner()
{
    while (true)
    {
        uint16_t joystickX = analogRead(A0);

        if(joystickX )

        // lcd.clear();
        // lcd.setCursor(0, 0);
        // lcd.print("E");
        // lcd.setCursor(2, 0);
        // lcd.print("A");
        // lcd.setCursor(4, 0);
        // lcd.print("D");
        // lcd.setCursor(6, 0);
        // lcd.print("G");
        // lcd.setCursor(8, 0);
        // lcd.print("B");
        // lcd.setCursor(9, 0);
        // lcd.print("E");

        if ((millis() - lastJoystickDebounceTime) < DEBOUNCE_DELAY)
            continue;

        joystickState = digitalRead(JOYSTICK_PIN);

        if (joystickState != lastJoystickState)
        {
            lastJoystickDebounceTime = millis();
            lastJoystickState = joystickState;

            if (joystickState == 1)
                break;
        }
    }
}