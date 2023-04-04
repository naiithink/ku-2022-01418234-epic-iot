#! /usr/bin/env bash

arduino-cli lib install ArduinoIoTCloud LiquidCrystal_I2C

export ARDUINO_LIBRARY_ENABLE_UNSAFE_INSTALL=true
arduino-cli lib install --git-url https://github.com/naiithink/musicians-mate.git
unset ARDUINO_LIBRARY_ENABLE_UNSAFE_INSTALL
