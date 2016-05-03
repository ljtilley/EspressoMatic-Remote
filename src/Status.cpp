#include "Status.h"

StatusClass::StatusClass() {

}

bool StatusClass::isConnected() {
    return true;
    /* code */
}

int StatusClass::getTemp() {
    return 99;
}

short StatusClass::getState() {
    return BREW_HEAT;
    // when the radio is implemented this section should go something like this:
    // if (last update received time < 2 seconds ago) {
    //   return _status.state;
    // }
    // else {
    //   return -1;
    // }
}

StatusClass Status;
