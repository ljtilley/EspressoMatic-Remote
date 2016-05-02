/**
* @Author: Luke Tilley <luke>
* @Date:   01-05-2016
* @Project: EspressoMatic Remote
* @Last modified by:   luke
* @Last modified time: 01-05-2016
*
* All the functions related to setting up the menu
*/

#include <Arduino.h>
#include <MenuBackend.h>


#ifndef MENU_H_
#define MENU_H_

void menuChangeEvent(MenuChangeEvent);
void itemChangeEvent(MenuItemChangeEvent);

/* MENU ITEM STRUCTURE SETUP */
MenuBackend menu = MenuBackend(NULL, menuChangeEvent, itemChangeEvent);
//beneath is list of menu items needed to build the menu
MenuItem brewtemp = MenuItem(menu, "Brew Set Point", 1, 'B');
MenuItem steamtemp = MenuItem(menu, "Steam Set Point", 1, 'S');
MenuItem brewtime = MenuItem(menu, "Brew Time", 1);
MenuItem preinftime = MenuItem(menu, "Preinfuse Time", 1);
MenuItem autobrew = MenuItem(menu, "Auto Brew Enable", 1);
    MenuItem autobrew_on = MenuItem(menu, "On", 2);
    MenuItem autobrew_off = MenuItem(menu, "Off", 2);
MenuItem tempoffset = MenuItem(menu, "Temp Offset", 1, 'O');
MenuItem reset = MenuItem(menu, "Reset to Default", 1, 'R');

#endif /* end of include guard: MENU_H_ */
