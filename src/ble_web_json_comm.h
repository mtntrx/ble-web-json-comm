#include "Particle.h"
#include "ble_web_json_constants.h"
#include <queue>

class BLEWebJsonComm {
    public:
        void setup();
	void sendMessage(char* message);
        void queue_msg(const uint8_t* rx_data, size_t len);
        int get_message( char* msgBuf, size_t bufSize );
    private:
        std::queue<char*> device_receive_msg_queue;
        BleCharacteristic *rxCharacteristic;
        BleCharacteristic *txCharacteristic;
};
