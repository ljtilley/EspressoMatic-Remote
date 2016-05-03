#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

class RadioClass {
public:
    RadioClass();
    void sendSettings();
private:
    static RF24 _radio;
};

extern RadioClass Radio;
