#include <Arduino.h>
#include <Button.h>
#include <LiquidCrystal.h>
#include <MenuBackend.h>
#include "Menu.h"
#include "Status.h"
#include "Settings.h"
#include "HardwareUI.h"


LiquidCrystal HardwareUIClass::_lcd(RSPIN, ENABLEPIN, D0, D1, D2, D3);
Button HardwareUIClass::_up_btn = Button(UP_BUTTON, true, true, 25);
Button HardwareUIClass::_down_btn = Button(DWN_BUTTON, true, true, 25);
Button HardwareUIClass::_select_btn = Button(SEL_BUTTON, true, true, 25);

int _buttonState = 0;
int _lastButtonState = 0;

long _lastDebounceTime = 0;  // the last time the output pin was toggled
long _debounceDelay = 50;

HardwareUIClass::HardwareUIClass() {
    initLCD();
    initButtons();
}

void HardwareUIClass::initLCD() {
    pinMode(LCD_BACKLIGHT, OUTPUT);
    _lcd.begin(LCD_COLS, LCD_ROWS);
}

void HardwareUIClass::initButtons() {
    pinMode(PWR_BUTTON, INPUT_PULLUP);
    pinMode(UP_BUTTON, INPUT_PULLUP);
    pinMode(DWN_BUTTON, INPUT_PULLUP);
    pinMode(SEL_BUTTON, INPUT_PULLUP);
}

void HardwareUIClass::pollButtons() {
    _up_btn.read();
    _down_btn.read();
    _select_btn.read();
}

bool HardwareUIClass::pwr_isHeld() {
    int reading = digitalRead(PWR_BUTTON);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH),  and you've waited
    // long enough since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != _lastButtonState) {
        // reset the debouncing timer
        _lastDebounceTime = millis();
    }

    if ((millis() - _lastDebounceTime) > 300) {
        // whatever the reading is at, it's been there for longer
        // than the debounce delay, so take it as the actual current state:

        // if the button state has changed:
        if (reading != _buttonState) {
            _buttonState = reading;

            // only toggle the LED if the new button state is HIGH
            if (_buttonState == LOW) {
                Serial.println("attaching interrupt");
                return true;
            }
        }
    }
    _lastButtonState = reading;
    return false;
}

bool HardwareUIClass::nxt_btn_isPressed(){
    return _up_btn.wasPressed();
}

bool HardwareUIClass::prev_btn_isPressed(){
    return _down_btn.wasPressed();
}

bool HardwareUIClass::sel_btn_isPressed(){
    return _select_btn.wasPressed();
}


void HardwareUIClass::backlightON() {
    for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 1) {
        // sets the value (range from 0 to 255):
        analogWrite(LCD_BACKLIGHT, fadeValue);
        // wait for 1 milliseconds to see the dimming effect
        delay(1);
    }
}

void HardwareUIClass::backlightOFF() {
    for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 1) {
        // sets the value (range from 0 to 255):
        analogWrite(LCD_BACKLIGHT, fadeValue);
        // wait for 1 milliseconds to see the dimming effect
        delay(1);
    }
}

void HardwareUIClass::displayOff() {
    _lcd.clear();
    _lcd.noDisplay();
    backlightOFF();
}

void HardwareUIClass::displayOn() {
    _lcd.display();
    backlightON();
}

void HardwareUIClass::displayStatus() {
    _lcd.setCursor(0,0);
    // if(connected == false) {
    //     _lcd.print("Waiting for Connection...");
    // }
    // else {
        _lcd.print("Boiler temp: ");
        _lcd.print(Status.getTemp());
        //Serial.println(temp);
        _lcd.setCursor(0,1);
        String displaytemp = "Set temp: " + (String) 99 + (char)223 + "F"; // (String) 99 should be changed to something that actually comes from the settings once the getters/setters are implemented
        _lcd.print(displaytemp);
    // }
}

void HardwareUIClass::displayMenuItem(MenuItem item) {
    Serial.println("Trying to update lcd display with the proper menu");
    _lcd.clear();
    _lcd.print(item.getName());
    _lcd.setCursor(0, 1);
    if (item.isEqual(brewtemp) || item.isEqual(steamtemp) || item.isEqual(tempoffset)) {
        _lcd.print(String(item.getValue())+(char)223+"F");
    }
}

void HardwareUIClass::refreshMenuItem(MenuItem item) {
    _lcd.setCursor(0, 1);
    _lcd.print("                ");
    _lcd.setCursor(0, 1);
    _lcd.print(String(item.getValue())+(char)223+"F");
}

HardwareUIClass HardwareUI;
