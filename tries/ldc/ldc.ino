/**
 * RS: SDA: 21
 *
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
// LiquidCrystal_I2C lcd(0x27,22,21);

void setup()
{
    Serial.begin(115200);

    // lcd.init();
    lcd.init();
    // lcd.begin(16, 2);
    // lcd.begin(16,2);
    lcd.backlight();
    lcd.setCursor(0,0);
    // lcd.clear();
    // lcd.setCursor(0, 0);
    lcd.print("hello, world");
    lcd.setCursor(0,1);
    lcd.print("goodgame");
}

void loop()
{
}
