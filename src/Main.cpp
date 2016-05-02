#include <Arduino.h>
#include "HardwareDefines.h"

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


/* LCD SETUP */
LiquidCrystal lcd(RSPIN, ENABLEPIN, D0, D1, D2, D3);



// set default settings values:
int brew_temp = 205;
int steam_temp = 225;
int brew_time = 30;
int preinf_time = 5;
bool auto_brew = true;
int temp_offset = 15;

/* SETTINGS AND MACHINE STATUS SETUP */
StatusPacket status {0, 0};

settings_t settings_struct;
MachineSettings settings;






/* FUNCTION PROTOTYPES */
/* TODO: Move this to Main.h */
void sleepNow();
void pwrInterrupt();
void updateDisplay(bool connected, StatusPacket state);


void displayMenuValue();
void loadMenuValues();



// State Machine Functions
State DisplayStatus() {
    /* begin testing section (haven't got the radio working yet)*/
    timeOld = timeNew;
    timeNew = millis();
    timer += abs(timeOld - timeNew);

    if(timer > 2000) {
       HardwareUI.displayStatus(); // this fakes having a radio connection
       timer=0;
    }
    /* end test section */
	// if ( !radio.available() ) {
	// 	updateDisplay(false, status);
	// }
	// else {
	// 	uint8_t len = radio.getDynamicPayloadSize();
	// 	radio.read(&status, len);
	// 	updateDisplay(true, status);
	// }

    // move to DisplayMenu State if any button is pressed
    if(HardwareUI.nxt_btn_isPressed() || HardwareUI.prev_btn_isPressed() || HardwareUI.sel_btn_isPressed()) {
        last_action_time = millis();
        Menu.select(brewtemp);
        HardwareUI.displayMenuItem(Menu.getCurrent());
        Serial.println("Moving to DisplayMenu State");
        UserInterface.Set(DisplayMenu);
    }
}

State DisplayMenu() {
	if (abs(millis() - last_action_time) >= 3000 ) {
	    	Serial.print("Changing state to DisplayStatus");
	        UserInterface.Set(DisplayStatus);
	}
    if(HardwareUI.nxt_btn_isPressed()) {
        last_action_time = millis();
        Serial.println("Moving menu up");
        Menu.moveDown();
    }
    else if (HardwareUI.prev_btn_isPressed()) {
        last_action_time = millis();
        Serial.println("Moving menu down");
        Menu.moveUp();
    }

    // Check for transition cases. Either a timeout of 3 seconds of no activity (back to displaystatus) or click on the encoder to transition to edit state.

    else if (HardwareUI.sel_btn_isPressed()) {
        UserInterface.Set(EditMenu);
    }
}

State EditMenu() {
    if(Menu.getCurrent().getShortkey() == 'R') {
    	settings.reset_default();
    	loadMenuValues();
    	UserInterface.Set(DisplayMenu);
    }
    else {
		if (HardwareUI.nxt_btn_isPressed()) {
			Menu.getCurrent().increment(1);

		}
		else if (HardwareUI.prev_btn_isPressed()) {
			Menu.getCurrent().decrement(1);
		}
		// Check for transition to next state (encoder clicked)
		if (HardwareUI.sel_btn_isPressed()) {
			if(strcmp(Menu.getCurrent().getName(), "Brew Set Point") == 0) {
				Serial.println("updating Brew Set Point");
				settings_struct.brew_temp = (short) Menu.getCurrent().getValue();
				Serial.println(settings_struct.brew_temp);
			} else if(strcmp(Menu.getCurrent().getName(), "Steam Set Point") == 0) {
				settings_struct.steam_temp = (short) Menu.getCurrent().getValue();
			} else if(strcmp(Menu.getCurrent().getName(), "Temp Offset") == 0) {
				settings_struct.temp_offset = (short) Menu.getCurrent().getValue();
			}
			settings.update(settings_struct.brew_temp, settings_struct.steam_temp, settings_struct.temp_offset);
			HardwareUI.displayMenuItem(Menu.getCurrent());
			UserInterface.Set(DisplayMenu);
		}
    }
}

void itemChangeEvent(MenuItemChangeEvent changed) {
    Serial.print("MenuItemChangeEven Fired\n");
    lcd.clear();
    lcd.print(Menu.getCurrent().getName());
    lcd.setCursor(0, 1);
    lcd.print(String(Menu.getCurrent().getValue())+(char)223+"F");
}

void menuChangeEvent(MenuChangeEvent changed) {

    HardwareUI.displayMenuItem(changed.to);
}



void menuSetup()
{
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
    settings_struct = settings.get_settings();
    brewtemp.setValue(settings_struct.brew_temp);
    steamtemp.setValue(settings_struct.steam_temp);
    // features to be added later with a second SSR
    //brewtime.setValue(30);
    //preinftime.setValue(5);
    tempoffset.setValue(settings_struct.temp_offset);
}

// actual functions

void updateDisplay(bool connected, StatusPacket state) {
	lcd.setCursor(0,0);
	if(connected == false) {
		lcd.print("Waiting for Connection...");
	}
	else {
		lcd.print("Boiler temp: ");
		lcd.print(status.temp);
		//Serial.println(temp);
		lcd.setCursor(0,1);
		String displaytemp = "Set temp: " + (String) settings_struct.brew_temp + "F";
		lcd.print(displaytemp);
	}
    //lcd.clear();

    //int temp = (int) random(195, 205);

}

void pwrInterrupt() {}

void sleepNow()
{
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
    lcd.begin(LCD_COLS, LCD_ROWS);

    // Initialize Serial
    Serial.begin(9600);
    lcd.setCursor(0, 0);
    lcd.print("Hello World!!");

    menuSetup();



    //EEPROM Settings load and setup
    EEPROM.setMaxAllowedWrites(50);
    settings.load();
    settings_struct = settings.get_settings();
    Serial.println(settings_struct.brew_temp);
    Serial.println(settings_struct.steam_temp);
    Serial.println(settings_struct.temp_offset);
}

void loop() {
    if(HardwareUI.pwr_isHeld()) {
        sleepNow();
    }
    HardwareUI.pollButtons();
    EXEC(UserInterface);
    // read_buttons();
    // if(up_btn.wasPressed()) {
    //     Serial.println("Up Pressed.");
    // }
    // if(down_btn.wasPressed()) {
    //     Serial.println("Down Pressed.");
    // }
    // if(select_btn.wasPressed()) {
    //     Serial.println("Select Pressed.");
    // }
}
