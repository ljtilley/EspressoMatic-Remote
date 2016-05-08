#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

#include "Radio.h"
#include "Settings.h"
#include "Status.h"
#include "printf.h" //needed for RF24 debug messages

RadioClass::RadioClass() {
    printf_begin();
    Serial.println("We got this far");

}

void RadioClass::radioInit() {
    _radio.begin();
    _radio.setRetries(15,15);
    _radio.setPayloadSize(sizeof(_settings));
    _radio.openReadingPipe(1, read_pipe);
    _radio.openWritingPipe(write_pipe);
    //_radio.startListening();
    _radio.printDetails();
}
bool RadioClass::sendSettings() {
    _settings = Settings.get_settings();
    //_radio.stopListening();
    Serial.println("trying to send the settings");
    bool success = _radio.write(&_settings, sizeof(_settings));
    if(success) {
        return true;
    }
    else { return false; }
    //_radio.startListening();
}

RadioClass Radio;
