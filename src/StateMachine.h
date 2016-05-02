/* StateMachine.h
 * Version 0.1
 * By: Luke Tilley
 * May 1, 2016
 *
 * This file contains the main state machine for the remote
 * The state machine is used to manage the state of the machine and transitioning from
 * displaying the status, moving through the menu, editing settings, etc
 */

#include <Arduino.h>
#include <SM.h>
#include "HardwareUI.h" // We need to interact with the UI (LCD and Buttons)
#include "Menu.h" // We also need to work with the menu system

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

/* MENU SYSTEM VARIABLES SETUP */


// Set up state machines prototypes
State DisplayStatus();
State DisplayMenu();
State EditMenu();

extern SM UserInterface;

#endif /* STATEMACHINE_H_ */
