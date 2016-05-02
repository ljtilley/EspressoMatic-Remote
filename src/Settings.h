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

#define DEFAULT_BREW_TEMP 205;
#define DEFAULT_STEAM_TEMP 225;
#define DEFAULT_BREW_TIME 30;
#define DEFAULT_PREINF_TIME 5;
#define DEFAULT_AUTO_BREW true;
#define DEFAULT_TEMP_OFFSET 15;

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

	short getBrewTemp();
	void setBrewTemp(short brew_temp);
	short getSteamTemp();
	void setSteamTemp(short steam_temp);
	short getTempOffset();
	void setTempOffset(short temp_offset);

	void update(short brew, short steam, short offset);
	settings_t get_settings();
	void reset_default();

private:
	settings_t settings_struct;
	int configAddress;
};

extern MachineSettings Settings;

#endif /* SETTINGS_H_ */
