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
extern MenuBackend menu;
//beneath is list of menu items needed to build the menu
//beneath is list of menu items needed to build the menu
extern MenuItem brewtemp;
extern MenuItem steamtemp;
extern MenuItem brewtime;
extern MenuItem preinftime;
extern MenuItem autobrew;
    extern MenuItem autobrew_on;
    extern MenuItem autobrew_off;
extern MenuItem tempoffset;
extern MenuItem reset;

#endif /* end of include guard: MENU_H_ */
