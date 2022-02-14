#include "ble_web_json_comm.h"

#if PLATFORM_ID != 12 // Argon
    #error "This library only works on the Argon!"
#else

Logger BLEwebJsonCommLog("app.BLEwebJsonCommLog");

static void onDataReceived(const uint8_t* rx_data, size_t len, const BlePeerDevice& peer, void* self) {
    // Is this really the way to do this?
    Log.info("BLE MANAGER: in onDataReceived routing");
    ((BLEWebJsonComm*)self)->queue_msg(rx_data, len);
}

BLEWebJsonComm::BLEWebJsonComm() 
  : config_state(STATE_CONFIG_SETUP),
    next_config_state(STATE_CONFIG_SETUP),
    provisionCb(nullptr)
{}

void BLEWebJsonComm::setup() {
    rxCharacteristic = new BleCharacteristic("rx", BleCharacteristicProperty::NOTIFY, readUUID, serviceUUID);
    txCharacteristic = new BleCharacteristic("tx", BleCharacteristicProperty::WRITE_WO_RSP, writeUUID, serviceUUID, onDataReceived, this);

    BLE.addCharacteristic(*rxCharacteristic);
    BLE.addCharacteristic(*txCharacteristic);

    // Advertise our custom configuration service UUID so the webapp can detect compatible devices
    BleAdvertisingData advData;
    advData.appendServiceUUID(serviceUUID);
    BLE.advertise(&advData);

    BLEwebJsonCommLog.trace("Bluetooth Address: %s", BLE.address().toString().c_str());
    BLE.on();

    // WiFi must be on for this library to work
    //WiFi.on();
}

void BLEWebJsonComm::loop() {

    char tmp_buf[150];  // Need: ~64 chars + SSID length + null terminator

    int len = sprintf(tmp_buf, "{\"msg_t\":\"general_debug\", \"key1\":\"%s\", \"key2\":\"%s\", \"key3\":%d}", 
				"value1", "value2", 3 );

    BLEwebJsonCommLog.trace("Sending message from device to browser %s",tmp_buf);
    rxCharacteristic->setValue((uint8_t*)tmp_buf, len);  // message from device to browser
}

void BLEWebJsonComm::parse_message() {
    // Pull our message off the queue, copy it locally, and free the original message
    // Probbaly not ideal since we don't really need to copy, but since we parse and have conditional
    // actions later, this may prevent accidental memory leaks with the addition of other conditional paths in this code 
    // e.g. returning somewhere down below before the free() call.
    char *msg_buf = device_receive_msg_queue.front();
    BLEwebJsonCommLog.trace("String RX: %s", msg_buf);
    JSONValue outerObj = JSONValue::parseCopy(msg_buf);
    device_receive_msg_queue.pop();
    free(msg_buf);

    // Process our received message
    JSONObjectIterator iter(outerObj);
    while(iter.next()) {
        BLEwebJsonCommLog.info("key=%s value=%s", 
            (const char *) iter.name(), 
            (const char *) iter.value().toString());
    }
}

void BLEWebJsonComm::queue_msg(const uint8_t* rx_data, size_t len) {
    if( len > 0 ) {
        // The underlying BLE lib reuses the receive buffer, and will not terminate it properly for a string
        // Add some manual processing and properly terminate for string parsing
        char *msg_buf = (char*)malloc(len+1);
        memcpy(msg_buf, rx_data, len);
        msg_buf[len] = 0;   // Null-terminate string
        device_receive_msg_queue.push(msg_buf);
        Log.info("BLE MANAGER:  Added message to the queue: %s", msg_buf);
        BLEwebJsonCommLog.trace("Added message to the queue: %s", msg_buf);
        return;
    }
}

#endif  // PLATFORM_ID == 12
