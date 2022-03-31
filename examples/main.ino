// EXAMPLE - Device OS 3.0 and later,  Argon/Boron only
#include "ble_web_json_comm.h"

#define MESSAGE_SEND_TIMEOUT 7000
#define MESSAGE_CHECK_TIMEOUT 50

// the communicatoin object
BLEWebJsonComm bleWeb;

unsigned long messageSendLast;
unsigned long messageCheckLast;

int x=0;

void setup() {
    Serial.begin(9600);
    bleWeb.setup();
    pinMode( D7, OUTPUT);
    digitalWrite( D7, LOW);

    messageSendLast = millis();
    messageCheckLast = millis();
}

void loop() {

    // send a new message (device->browser) every MESSAGE_SEND_TIMEOUT ms
    if ( millis() - messageSendLast  > MESSAGE_SEND_TIMEOUT ) {
        digitalWrite( D7, HIGH);
        char myMessage[64];
        sprintf( myMessage, "{\"increment_value\":%d}", x++  );
	Serial.printlnf("transmitting message (device->browser): %s", myMessage ) ;
        bleWeb.sendMessage( myMessage );
	messageSendLast = millis();
        delay(10);
        digitalWrite( D7, LOW);
    }

    // check for a new message (browswer->device) every MESSAGE_CHECK_TIMEOUT ms
    if ( millis() - messageCheckLast  > MESSAGE_CHECK_TIMEOUT ) {
	char bleReceivedMessage[1024];
	int msgSize = bleWeb.get_message( bleReceivedMessage, 1024);
	if ( msgSize > 0 ) {
		Serial.printlnf("MAIN Received message: %s  mem: %d", bleReceivedMessage, System.freeMemory()  ) ;
	}
	messageCheckLast = millis();
    }

    delay(10);
}

