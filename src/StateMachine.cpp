#include "StateMachine.h"

int menu_timeout = 2000;
unsigned long last_action_time = 0;
int timeOld = 0;
int timeNew = 0;
int timer = 0;

SM UserInterface(DisplayStatus);

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
    	//settings.reset_default();
    	//loadMenuValues();
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
				//settings_struct.brew_temp = (short) Menu.getCurrent().getValue();
				//Serial.println(settings_struct.brew_temp);
			} else if(strcmp(Menu.getCurrent().getName(), "Steam Set Point") == 0) {
				//settings_struct.steam_temp = (short) Menu.getCurrent().getValue();
			} else if(strcmp(Menu.getCurrent().getName(), "Temp Offset") == 0) {
				//settings_struct.temp_offset = (short) Menu.getCurrent().getValue();
			}
			//settings.update(settings_struct.brew_temp, settings_struct.steam_temp, settings_struct.temp_offset);
			HardwareUI.displayMenuItem(Menu.getCurrent());
			UserInterface.Set(DisplayMenu);
		}
    }
}
