// Library ที่พวกเราเขียนเอง
#include <MusiciansMate.h>

// จอ character display
#include <LiquidCrystal_I2C.h>

// เก็บเสียง โด เร มี ฟา ...
#include "includes/Pitches.h"

// เก็บตัวแปรที่ใช้กับ Arduino IoT Cloud
#include "includes/thingProperties.h"

// Pin ต่อ buzzer -- ลำโพง
#define BUZZ_PIN 17

// Pin ต่อ xy joystick
#define JOYSTICK_PIN 16

// ความถี่ default ของ Metronome
// หน่วยเป็น Beats Per Minutes -- ครั้งต่อนาที
#define DEFAULT_TEMPO 120

// ความยาวในการเล่นโน้ตแต่ละตัว
// หน่วยเป็น มิลลิวินาที -- milliseconds
// 1000 คือ 1 วินาที
#define NOTE_DURATION (1000 * 1 * 0.18)

// ความยาวในการเล่นเสียง Tuner
// หน่วยเป็น มิลลิวินาที -- milliseconds
// 1000 คือ 1 วินาที
#define TUNER_DURATION (1000)


// ระยะในการ debounce
#define DEBOUNCE_DELAY (unsigned long)50


// สำหรับการทำงานอีก core
// จะมี function ชื่อ backgroundTask() เหมือนกับ loop() อีกอันหนึ่ง
// โดยที่ backgroundTask() จะทำงานคนละ core กับ loop()
// ก็คือจะทำงานไปพร้อมๆ กันแบบหลาย CPU core
TaskHandle_t BackgroundTask;

// initialize object ที่ใช้ติดต่อกับจอ LCD
// 0x27 คือ address ของจอ
// 16 คือ จำนวนตัวอักษรต่อบรรทัด
// 1 คือ จำนวนบรรทัดที่ต้องการแสดงบนจอ -- เราใช้ 1 เพราะไฟไม่พอ
LiquidCrystal_I2C lcd(0x27, 16, 1);


// ใช้จับ status ของ xy joystick
// ค่าที่เป็นไปได้คือ 0 และ 1
//  - 1 คือ ปุ่มไม่ถูกกด
//  - 0 คือ ปุ่มถูกกด
int joystickState;

// ใช้ debounce ปุ่มกดตรงกลาง joystick -- ตัวจับ status การกดปุ่ม
// ค่าต่างๆ เหมือน joystickState
int lastJoystickState = 1;

// ใช้ debounce ปุ่มกดตรงกลาง joystick -- ตัวจับเวลา
unsigned long lastJoystickDebounceTime = millis();


// ตัวจับ status ของ Metronome
// -1 คือ pause
//  1 คือ play
int metronomeState = -1;

// pageStage
//      1: Tuner
//     -1: Metronome
int pageState = -1;

/// poom
// ตัวจับ index ของสาย Guitar
int numArrowTuner = 1;

/// ไม่จำเป็นไม่ต้องอธิบาย -- บอกไปว่าใช้กับ library
// เสียงเล่นตอนเปิดเครื่อง
// แต่ละ {} ย่อยเก็บเสียงโน้ต, ความยาวเสียงที่จะเล่น ตามลำดับ
track jingle[] = {
    { P_G7,     NOTE_E },
    { P_G7,     NOTE_E },
    { P_G7,     NOTE_E },
    { P_DS7,    NOTE_H },
    { P_F7,     NOTE_E },
    { P_F7,     NOTE_E },
    { P_F7,     NOTE_E },
    { P_D7,     NOTE_H },
    { 0, 0 }
};

// initialize Metronome -- เครื่องจับจังหวะดนตรี
Metronome *metronome = new Metronome(BUZZ_PIN, P_F7, NOTE_DURATION, DEFAULT_TEMPO);

// initialize GuitarTuner -- เครื่องตั้งสายกีตาร์
GuitarTuner *tuner = (GuitarTuner *) TunerBuilder::build(BUZZ_PIN, GUITAR, TUNER_DURATION);

// run ครั้งเดียวตอน board เริ่มทำงาน
void setup()
{
    // ตั้ง mode ของ pin ที่ต่อกับลำโพง buzzer
    // เป็น output -- ข้อมูลออก
    pinMode(BUZZ_PIN, OUTPUT);

    // ตั้ง mode ของ jostick
    // เป็น input และ pullup -- ข้อมูลเข้า และ สามารถโยกได้
    pinMode(JOYSTICK_PIN, INPUT_PULLUP);

    // ตัว serial monitor สำหรับ print
    Serial.begin(115200);
    // เอาไว้ป้องกันปัญหาการแย่งใช้ทรัพยากร เลยหน่วงเวลาไว้ให้ serial monitor ตั้งค่าให้เสร็จก่อน
    delay(1500);

    // initialize จอ LCD
    lcd.init();

    // เปิดแสงพื้นหลังจอ LCD
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Musicians' Mate");

    // Jingle
    // เล่นเสียงที่ประกาศไว้ในตัวแปร jingle (ข้างบน)
    playTrack(BUZZ_PIN, jingle, DEFAULT_TEMPO);

    // initialize ค่าต่างๆ บน Arduino IoT Cloud
    initProperties();

    // ให้ board ต่อกับ Arduino IoT Cloud
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);

    // จอง CPU core -- ผูก function backgroundTask กับ core หมายเลข 0, เพราะ core 1 ถูกใช้โดย loop() ไปแล้ว
    xTaskCreatePinnedToCore(
        backgroundTask,
        "backgroundTask",
        2048,
        NULL,
        1,
        &BackgroundTask,
        0
    );

    Serial.println("Done setup");

    // เริ่ม Metronome
    lcd.clear();
    lcd.print("Metronome");
    delay(1000 * 2);

    metronome->start();
    metronomeState = metronome->getIsPlaying() == true ? 1
                                                       : -1;
}

void backgroundTask(void *param)
{
    while (true)
    {
        vTaskDelay(10);

        if (metronome->getIsPlaying() == false && metronomeState > 0 && pageState == -1)
            metronome->start();
    }
}

void loop()
{
    ArduinoCloud.update();

    // Serial.printf("X: %i\n", analogRead(A0));
    // Serial.printf("Y: %i\n", analogRead(A3));
    // Serial.printf("B: %i\n", digitalRead(JOYSTICK_PIN));

    uint16_t joystickX = analogRead(A0);
    uint16_t joystickY = analogRead(A3);

    if (metronome->getIsPlaying() == true)
    {
        if (joystickX == 0)
            decreaseMetronomeTempo();
        else if (joystickX >= 4000)
            increaseMetronomeTempo();
    }
    else
    {
        if (joystickY == 0 && pageState == 1)
            switchToMetronomePage();
        else if (joystickY >= 4000 && pageState == -1)
            switchToGuitarTunerPage();
        else if (joystickX == 0 && pageState == 1)
            shiftTunerLeft();
        else if (joystickX >= 4000 && pageState == 1)
            shiftTunerRight();
    }

    if ((millis() - lastJoystickDebounceTime) >= DEBOUNCE_DELAY)
    {
        joystickState = digitalRead(JOYSTICK_PIN);

        if (joystickState != lastJoystickState)
        {
            lastJoystickDebounceTime = millis();
            lastJoystickState = joystickState;

            if (joystickState == 1 && pageState == -1)
                toggleMetronomeState();
            else if (joystickState == 1 && pageState == 1)
                playTuner();
        }
    }
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
    metronomeState *= -1;
    Serial.printf("Metronome: %s\n", metronomeState > 0 ? "Play"
                                                        : "Pause");
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
    Serial.printf("Current Metronome tempo: %d\n", metronome->getTempo());
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
    Serial.printf("Current Metronome tempo: %d\n", metronome->getTempo());
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

void switchToMetronomePage()
{
    lcd.clear();
    lcd.print("Metronome");
    delay(1000 * 2);

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

void switchToGuitarTunerPage()
{
    lcd.clear();
    lcd.print("Guitar Tuner");
    delay(1000 * 2);

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

void shiftTunerLeft()
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

void shiftTunerRight()
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

/*
  Since OkButton is READ_WRITE variable, onOkButtonChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onOkButtonChange()
{
    // Add your code here to act upon OkButton change
    if (okButton == false)
        return;

    Serial.println("okButton hit");
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
    if (upButton == false)
        return;

    Serial.println("upButton hit");

    if (pageState == 1)
        switchToMetronomePage();
}
/*
  Since DownButton is READ_WRITE variable, onDownButtonChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onDownButtonChange()
{
    // Add your code here to act upon DownButton change
    if (downButton == false)
        return;

    Serial.println("downButton hit");

    if (pageState == -1 && metronome->getIsPlaying() == false)
        switchToGuitarTunerPage();
}
/*
  Since LeftButton is READ_WRITE variable, onLeftButtonChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onLeftButtonChange()
{
    // Add your code here to act upon LeftButton change
    if (leftButton == false)
        return;

    Serial.println("leftButton hit");

    if (pageState == -1 && metronome->getIsPlaying() == true)
        decreaseMetronomeTempo();
    else if (pageState == 1)
        shiftTunerLeft();
}
/*
  Since RightButton is READ_WRITE variable, onRightButtonChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onRightButtonChange()
{
    // Add your code here to act upon RightButton change
    if (rightButton == false)
        return;

    Serial.println("rightButton hit");

    if (pageState == -1 && metronome->getIsPlaying() == true)
        increaseMetronomeTempo();
    else if (pageState == 1)
        shiftTunerRight();
}
