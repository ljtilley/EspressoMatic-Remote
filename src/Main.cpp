#include <Arduino.h>

/* nrf24l01 library stuff */
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

/* power libraries */
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>

/* Project Files */
#include "Settings.h"
#include "Status.h"

/* Menu Related Files */
#include <MenuBackend.h>
#include "Menu.h"

/* Hardware UI Related Files */
#include <LiquidCrystal.h>
#include <Button.h>
#include "HardwareUI.h"

/* State Machine Related Files */
#include <SM.h>
#include "StateMachine.h"


/* NOTE: Don't forget to explicitly include all dependant libaries to not hide project dependencies in other headers */

/* SETTINGS AND MACHINE STATUS SETUP */
StatusPacket status {0, 0};

/* FUNCTION PROTOTYPES */
/* TODO: Move this to Main.h */
void loadMenuValues();

void itemChangeEvent(MenuItemChangeEvent changed) {
    Serial.print("MenuItemChangeEven Fired\n");
    HardwareUI.refreshMenuItem(Menu.getCurrent());
}

void menuChangeEvent(MenuChangeEvent changed) {
    HardwareUI.displayMenuItem(changed.to);
}

void menuSetup() {
    // set up menu structure
    MenuItem root = Menu.getRoot();
    root.addAfter(brewtemp);
    brewtemp.add(steamtemp);
    // TODO: features to be added later with a second SSR
    //steamtemp.add(brewtime);
    //brewtime.add(preinftime);
    //preinftime.add(autobrew);
    steamtemp.add(tempoffset);
    tempoffset.add(reset);
    brewtemp.addBefore(reset);
    loadMenuValues();
}

void loadMenuValues() {
    // set default values:
    brewtemp.setValue(Settings.getBrewTemp());
    steamtemp.setValue(Settings.getSteamTemp());
    // features to be added later with a second SSR
    //brewtime.setValue(30);
    //preinftime.setValue(5);
    tempoffset.setValue(Settings.getTempOffset());
}

// actual functions
void pwrInterrupt() {}

void sleepNow() {
    HardwareUI.displayOff(); // power down display
    // power down radio
    // radio.stopListening();
    // radio.powerDown();

    // close serial connection

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
    noInterrupts();

    sleep_enable();
    EIFR = bit (INTF0);       // clear flag for interrupt 0
    attachInterrupt(0, pwrInterrupt, FALLING);  // wake up on rising edge
    interrupts();

    Serial.println("Going to sleep");
    Serial.end();

    power_adc_disable();
    power_spi_disable();
    power_timer0_disable();
    power_timer1_disable();
    power_timer2_disable();
    power_twi_disable();

    sleep_cpu();

    // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
    sleep_disable();// first thing after waking from sleep:
    power_all_enable();
    detachInterrupt(0);
    Serial.begin(9600);
    Serial.println("Waking up");
    //radio.powerUp();
    //radio.startListening();
    delay(10);
    HardwareUI.displayOn();
}

void setup() {
    // Set up LCD
    //pinMode(LCD_BACKLIGHT, OUTPUT);
    //digitalWrite(LCD_BACKLIGHT, 0);
    //lcd.begin(LCD_COLS, LCD_ROWS);

    // Initialize Serial
    Serial.begin(9600);
    //lcd.setCursor(0, 0);
    //lcd.print("Hello World!!");

    menuSetup();

    //EEPROM Settings load and setup
    EEPROM.setMaxAllowedWrites(50);
    Serial.println(Settings.getBrewTemp());
    Serial.println(Settings.getSteamTemp());
    Serial.println(Settings.getTempOffset());
}

void loop() {
    if(HardwareUI.pwr_isHeld()) {
        sleepNow();
    }
    HardwareUI.pollButtons();
    EXEC(UserInterface);
}
