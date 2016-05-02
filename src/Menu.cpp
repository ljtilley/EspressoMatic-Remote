#include <Arduino.h>
#include <MenuBackend.h>
#include "Menu.h"

MenuBackend Menu = MenuBackend(NULL, menuChangeEvent, itemChangeEvent);
//beneath is list of menu items needed to build the menu
MenuItem brewtemp = MenuItem(Menu, "Brew Set Point", 1, 'B');
MenuItem steamtemp = MenuItem(Menu, "Steam Set Point", 1, 'S');
MenuItem brewtime = MenuItem(Menu, "Brew Time", 1);
MenuItem preinftime = MenuItem(Menu, "Preinfuse Time", 1);
MenuItem autobrew = MenuItem(Menu, "Auto Brew Enable", 1);
    MenuItem autobrew_on = MenuItem(Menu, "On", 2);
    MenuItem autobrew_off = MenuItem(Menu, "Off", 2);
MenuItem tempoffset = MenuItem(Menu, "Temp Offset", 1, 'O');
MenuItem reset = MenuItem(Menu, "Reset to Default", 1, 'R');
