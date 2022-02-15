#include "ble_web_json_comm.h"

Logger bleLog("app.bleLog");

static void onDataReceived(const uint8_t* rx_data, size_t len, const BlePeerDevice& peer, void* self) {
    // Is this really the way to do this?
    //bleLog.info("in onDataReceived routing with %s", rx_data);
    ((BLEWebJsonComm*)self)->queue_msg(rx_data, len);
}

void BLEWebJsonComm::setup() {
    rxCharacteristic = new BleCharacteristic("rx", BleCharacteristicProperty::NOTIFY, readUUID, serviceUUID);
    txCharacteristic = new BleCharacteristic("tx", BleCharacteristicProperty::WRITE_WO_RSP, writeUUID, serviceUUID, onDataReceived, this);

    BLE.addCharacteristic(*rxCharacteristic);
    BLE.addCharacteristic(*txCharacteristic);

    BleAdvertisingData advData; // Advertise service UUID so webapp can detect
    advData.appendServiceUUID(serviceUUID);
    BLE.advertise(&advData);

    bleLog.trace("Setup:  Bluetooth Address: %s", BLE.address().toString().c_str());
    BLE.on();

}

void BLEWebJsonComm::sendMessage(char* message) {
    char tmp_buf[1024];
    int len = sprintf(tmp_buf, "%s", message );
    bleLog.trace("sendMessage:  Sending message from device to browser %s",tmp_buf);
    rxCharacteristic->setValue((uint8_t*)tmp_buf, len);  // message from device to browser
}

int BLEWebJsonComm::get_message( char* msgBuf, size_t bufSize ) {
    bleLog.trace("get message called");
    bleLog.trace("queue size: %d", device_receive_msg_queue.size() );

    if ( device_receive_msg_queue.empty() ) {
        msgBuf[0] = '\0';
	return 0;
    }

    strncpy( msgBuf, device_receive_msg_queue.front(), bufSize );
    char *memHog = device_receive_msg_queue.front();
    device_receive_msg_queue.pop();
    free( memHog);
    return 1;
}

void BLEWebJsonComm::queue_msg(const uint8_t* rx_data, size_t len) {
        //bleLog.trace("queue message: %s", rx_data);
    if( len > 0 ) {
        // The underlying BLE lib reuses the receive buffer, 
        // and will not terminate it properly for a string
        // Adding manual processing, properly terminating string
        char *msg_buf = (char*)malloc(len+1);
        memcpy(msg_buf, rx_data, len);
        msg_buf[len] = 0;   // Null-terminate string
        device_receive_msg_queue.push(msg_buf);
        Log.info("BLE MANAGER:  Added message to the queue: %s", msg_buf);
        bleLog.trace("queued message: %s", msg_buf);
        bleLog.trace("queue size: %d", device_receive_msg_queue.size() );
        return;
    }
}
