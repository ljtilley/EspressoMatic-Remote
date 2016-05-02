/* Buttons.h
 * Version 0.1
 * May 1, 2016
 * Luke Tilley
 *
 * Everything to do with handling the Hardware UI for the Remote. In the
 * initial revision of the hardware, this means a standar 16x2 LCD Display
 * and 4 tactile switches.
 *
 * I put this all in one file to make it easier to re-implement the UI
 * by creating standard input/ouput functions
 *
 * This will also make it easier to change out button libraries if I decide to later
 */

#include <Arduino.h>
#include <Button.h>
#include <LiquidCrystal.h>
#include "Status.h"

#ifndef HARDWAREUI_H_
#define HARDWAREUI_H_



// Button Hardware Defines
#define UP_BUTTON 4
#define DWN_BUTTON 5
#define SEL_BUTTON 3
#define PWR_BUTTON 2


// LCD Hardware Defines

#define RSPIN 8
#define ENABLEPIN 7
#define D0 A0
#define D1 A1
#define D2 A2
#define D3 A3

#define LCD_COLS 16
#define LCD_ROWS 2
#define LCD_BACKLIGHT 9

class HardwareUIClass {
public:
    HardwareUIClass();

    //input functions
    void pollButtons();
    bool pwr_isHeld();
    bool nxt_btn_isPressed();
    bool prev_btn_isPressed();
    bool sel_btn_isPressed();

    //output functions
    void displayStatus(bool connected, StatusPacket state);
    void clearDisplay();
    void displayMenuItem();
    void refreshMenuItem();

    void backlightON();
    void backlightOFF();
    void displayOff();
    void displayOn();

private:
    static LiquidCrystal _lcd;

    static Button _up_btn;
    static Button _down_btn;
    static Button _select_btn;

    /* Power Button Debounce Handling */
    int _buttonState;
    int _lastButtonState;

    long _lastDebounceTime;  // the last time the output pin was toggled
    long _debounceDelay;

    void initLCD();
    void initButtons();
};

extern HardwareUIClass HardwareUI;

#endif /* HARDWAREUI_H_ */
