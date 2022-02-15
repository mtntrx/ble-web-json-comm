// EXAMPLE - Device OS 3.0 and later
#include "ble_web_json_comm.h"

#define MESSAGE_SEND_TIMEOUT 7000
#define MESSAGE_CHECK_TIMEOUT 50


//SerialLogHandler logHandler(LOG_LEVEL_ALL, {
 //   {"app", LOG_LEVEL_ERROR},
  //  {"system.ctrl.ble", LOG_LEVEL_ERROR},
   // {"wiring.ble", LOG_LEVEL_ERROR},
//});

BLEWebJsonComm bleWeb;

unsigned long messageSendLast;
unsigned long messageCheckLast;

int x=0;

void setup() {
   // (void)logHandler; // Does nothing, just to eliminate warning for unused variable
    Serial.begin(9600);
    bleWeb.setup();
    pinMode( D7, OUTPUT);
    digitalWrite( D7, HIGH);

    messageSendLast = millis();
    messageCheckLast = millis();
}

void loop() {

    if ( millis() - messageSendLast  > MESSAGE_SEND_TIMEOUT ) {
        char myMessage[64];
        sprintf( myMessage, "{\"rotations\":%d}", x++  );
        bleWeb.sendMessage( myMessage );
	messageSendLast = millis();
    }

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

