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
    lcd.begin(16, 2);

	// create a new character
	lcd.createChar(0, Heart);
    lcd.backlight();
    // create a new character
	// lcd.createChar(0, Heart);
    // lcd.setCursor(0,1);
    // lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.write(byte(0));
    lcd.setCursor(0,0);
    lcd.print("E");
    lcd.setCursor(3,0);
    lcd.print("A");
    lcd.setCursor(6,0);
    lcd.print("D");
    lcd.setCursor(9,0);
    lcd.print("G");
    lcd.setCursor(12,0);
    lcd.print("B");
    lcd.setCursor(15,0);
    lcd.print("E");
        // NOTE_E2,
        // NOTE_A2,
        // NOTE_D3,
        // NOTE_G3,
        // NOTE_B3,
        // NOTE_E4
}

void loop()
{
}
