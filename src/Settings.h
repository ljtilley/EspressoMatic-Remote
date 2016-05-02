/**
* @Author: Luke Tilley <luke>
* @Date:   30-04-2016
* @Project: EspressoMatic Remote
* @Last modified by:   luke
* @Last modified time: 01-05-2016
*
* Load and save settings from EEPROM
* Functions relating to sending/receiving settings data from the
* base unit will be contained in Radio.h
*/


#include <EEPROMex.h>
#include <Arduino.h>

#ifndef SETTINGS_H_
#define SETTINGS_H_

// ID of the settings block
#define CONFIG_VERSION "ls1"

// Tell it where to store your config data in EEPROM
#define CONFIG_START 32

struct settings_t {
	short brew_temp;
	short steam_temp;
	short temp_offset;
	char version[3];
};

class MachineSettings {
public:
	MachineSettings();
	~MachineSettings();
	void load();
	void update(short brew, short steam, short offset);
	settings_t get_settings();
	void reset_default();

private:
	settings_t settings_struct;
	int configAddress;
};



#endif /* SETTINGS_H_ */
