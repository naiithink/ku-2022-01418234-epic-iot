@ECHO OFF

arduino-cli lib install ArduinoIoTCloud LiquidCrystal_I2C

SET ARDUINO_LIBRARY_ENABLE_UNSAFE_INSTALL=true
arduino-cli lib install --git-url https://github.com/naiithink/musicians-mate.git
SET ARDUINO_LIBRARY_ENABLE_UNSAFE_INSTALL=
