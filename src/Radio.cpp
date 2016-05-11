#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

#include "Radio.h"
#include "Settings.h"
#include "Status.h"
#include "printf.h" //needed for RF24 debug messages

RadioClass::RadioClass() : _radio(RADIO_CE, RADIO_CS) {
    printf_begin();
    Serial.println("We got this far");

}

void RadioClass::radioInit() {
    _radio.begin();
    _radio.setDataRate(RF24_250KBPS);
    _radio.setRetries(15,15);
    _radio.setPayloadSize(sizeof(StatusPacket));
    _radio.openReadingPipe(1, read_pipe);
    _radio.openWritingPipe(write_pipe);
    _radio.startListening(); //set radio to start listening by default
    _radio.printDetails();
}
bool RadioClass::sendSettings() {
    _settings = Settings.get_settings();
    //_radio.stopListening();
    Serial.println("trying to send the settings");
    _radio.stopListening();
    _radio.setPayloadSize(sizeof(_settings));
    bool success = _radio.write(&_settings, sizeof(_settings));
    _radio.setPayloadSize(sizeof(StatusPacket));
    _radio.startListening();
    if(success) {
        return true;
    }
    else { return false; }
}

StatusPacket RadioClass::getStatus() {
    StatusPacket status = Status.getStatusPacket();

    if(_radio.available()) {
        Serial.println("We're receiving some stuff, hold on.");
        while(_radio.available()) {
            _radio.read(&status, sizeof(status));
        }
        _last_connection = millis();
    }
    else {
        if(millis() - _last_connection >= 1500) {
            Serial.println("we've been disconnected");
            status.state = DISCONNECTED;
        }
    }
    return status;
}

RadioClass Radio;
