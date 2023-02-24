// Button
const int buttonPin = 17;

// Builtin LED
int builtinLED = 16;

// Button active state
unsigned int buttonState;

// Current LED state
unsigned int currentState = HIGH;

unsigned int countPush = 0;

// Debouncing
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(9600);

  pinMode(builtinLED, OUTPUT);
  pinMode(buttonPin, INPUT);

  Serial.println("hello, world");
}

void loop() {
  int reading = digitalRead(buttonPin);

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        currentState = !currentState;
        countPush++;

        Serial.println(countPush);
      }
    }
  }

  digitalWrite(builtinLED, currentState);

  lastButtonState = reading;
}
