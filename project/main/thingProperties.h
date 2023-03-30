#ifndef THING_PROPERTIES_H
#define THING_PROPERTIES_H

// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

#define SECRET_SSID "NAI"
#define SECRET_OPTIONAL_PASS "epic_iot"
#define SECRET_DEVICE_KEY "6FPDENGCBMMUORIP7CP3"

const char DEVICE_LOGIN_NAME[] = "142e4ea7-e882-49a9-b778-5966fa88eea6";

const char SSID[] = SECRET_SSID;             // Network SSID (name)
const char PASS[] = SECRET_OPTIONAL_PASS;    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[] = SECRET_DEVICE_KEY; // Secret device password

void onDownButtonChange();
void onLeftButtonChange();
void onOkButtonChange();
void onRightButtonChange();
void onUpButtonChange();

bool downButton;
bool leftButton;
bool okButton;
bool rightButton;
bool upButton;

void initProperties()
{

    ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
    ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
    ArduinoCloud.addProperty(downButton, READWRITE, ON_CHANGE, onDownButtonChange);
    ArduinoCloud.addProperty(leftButton, READWRITE, ON_CHANGE, onLeftButtonChange);
    ArduinoCloud.addProperty(okButton, READWRITE, ON_CHANGE, onOkButtonChange);
    ArduinoCloud.addProperty(rightButton, READWRITE, ON_CHANGE, onRightButtonChange);
    ArduinoCloud.addProperty(upButton, READWRITE, ON_CHANGE, onUpButtonChange);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

#endif /* THING_PROPERTIES_H */
