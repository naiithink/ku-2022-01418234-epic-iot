#define JOYSTICK_PIN 17

int value;

void setup()
{
    Serial.begin(115200);
    pinMode(JOYSTICK_PIN, INPUT_PULLUP);

    Serial.println("Done setup");
}

void loop()
{
    value = analogRead(A0);
    Serial.print("X:");
    Serial.print(value, DEC);

    value = analogRead(A3);
    Serial.print(" | Y:");
    Serial.print(value, DEC);

    value = digitalRead(JOYSTICK_PIN);
    Serial.print(" | Button:");
    Serial.println(value, DEC);

    delay(100);
}
