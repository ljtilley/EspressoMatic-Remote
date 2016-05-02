/*
 * Settings.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: luke
 */
#include <EEPROMex.h>
#include <Arduino.h>
#include "Settings.h"

// let's initialize the settings struct with a sensible default

MachineSettings::MachineSettings() {
	// load some sensible defaults into the system
	settings_struct.version[0] = 'l';
	settings_struct.version[1] = 's';
	settings_struct.version[2] = '1';
	settings_struct.brew_temp = 200;
	settings_struct.steam_temp = 235;
	settings_struct.temp_offset = 15;
	// LEAVE THIS ON UNIL DEBUGGING IS FINISHED
	EEPROM.setMaxAllowedWrites(50);
	EEPROM.setMemPool(CONFIG_START,EEPROMSizePro);
	configAddress = EEPROM.getAddress(sizeof(settings_t));
	load();
}

MachineSettings::~MachineSettings() {
	/* nothing to destruct */
}

void MachineSettings::load() {
	EEPROM.readBlock(configAddress, settings_struct);
}

void MachineSettings::update(short brew, short steam, short offset) {
	settings_struct.brew_temp = brew;
	settings_struct.steam_temp = steam;
	settings_struct.temp_offset = offset;
	EEPROM.updateBlock(configAddress, settings_struct);
}

settings_t MachineSettings::get_settings() {
	return settings_struct;
}

void MachineSettings::reset_default() {
	settings_struct.version[0] = 'l';
	settings_struct.version[1] = 's';
	settings_struct.version[2] = '1';
	settings_struct.brew_temp = 200;
	settings_struct.steam_temp = 235;
	settings_struct.temp_offset = 15;
	update(settings_struct.brew_temp, settings_struct.steam_temp, settings_struct.temp_offset);
}
