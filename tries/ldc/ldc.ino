/**
 * RS: SDA: 21
 *
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
// LiquidCrystal_I2C lcd(0x27,22,21);
byte Heart[8] = {
0b00000,
0b01010,
0b11111,
0b11111,
0b01110,
0b00100,
0b00000,
0b00000
};

void setup()
{
    Serial.begin(115200);

    // lcd.init();
    lcd.init();
    // lcd.begin(16, 2);
    lcd.begin(16, 2);

	// create a new character
	lcd.createChar(0, Heart);
    lcd.backlight();
    lcd.setCursor(0,0);
    // lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.print("hello, world");
    // lcd.home();
    lcd.write(byte(0));
    // lcd.write(0);
    lcd.setCursor(0,1);
    lcd.print("goodgame");
}

void loop()
{
}
