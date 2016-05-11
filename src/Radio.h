#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

#include "Settings.h"
#include "Status.h"

#define RADIO_CE 6
#define RADIO_CS 10

const uint64_t read_pipe = 0xF0F0F0F0D2LL;
const uint64_t write_pipe = 0xF0F0F0F0E1LL;

class RadioClass {
public:
    RadioClass();
    void radioInit();
    bool sendSettings();
    StatusPacket getStatus();
private:
    RF24 _radio;
    settings_t _settings;
    long _last_connection;
};

extern RadioClass Radio;
