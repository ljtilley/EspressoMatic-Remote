#include "Status.h"
#include "Radio.h"

StatusClass::StatusClass() {

}

bool StatusClass::isConnected() {
    return true;
    /* code */
}

void StatusClass::refresh() {
    _status = Radio.getStatus();

}

int StatusClass::getTemp() {
    return _status.temp;
}

short StatusClass::getState() {
    return _status.state;
    // when the radio is implemented this section should go something like this:
    // if (last update received time < 2 seconds ago) {
    //   return _status.state;
    // }
    // else {
    //   return -1;
    // }
}

StatusPacket StatusClass::getStatusPacket() {
    return _status;
}

StatusClass Status;
