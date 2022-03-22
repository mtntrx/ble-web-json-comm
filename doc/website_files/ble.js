
        var encoder = new TextEncoder();
        var particle_ble_device = null;    // BLE connection instance
        const uuid_wifi_config_service = 'b4ad5b8d-d2db-44d6-9d35-5d43b9e5060c';
        const uuid_wifi_config_tx      = '3c673f3a-382a-4835-8433-c1c1b6b65346';
        const uuid_wifi_config_rx      = '226285d5-7a5a-448d-8317-dae1fd2d6c36';

        var messageNumber = 0;

	function setUpRx() {
            if (particle_ble_device) {

                // Set up notifications for responses on the RX channel
                particle_ble_device.gatt.getPrimaryService(uuid_wifi_config_service)
                .then(service => {
                    service.getCharacteristic(uuid_wifi_config_rx)
                    .then(characteristic => {
                        characteristic.startNotifications();
                        return characteristic;
                    })
                    .then(characteristic => {
			///SET UP LISTENER HERE
                        characteristic.addEventListener('characteristicvaluechanged', rxDataHandler); 
                    })
                    .catch(error => {
                        Logger.log('Argh in setUpRx1! ' + error);
                    });
                    return service;
                })
                .then(service => { return service.getCharacteristic(uuid_wifi_config_tx) })
                .then(characteristic => {
                    Logger.log('Initiate WiFi Scan');
                    var scan_msg = { msg_type: "Device set up for Rx" }
                    var message_str = JSON.stringify(scan_msg);
                    var bytes = encoder.encode(message_str);
                    Logger.log('TX: ' + message_str);
                    return characteristic.writeValue(bytes);
                })
                .catch(error => {
                    Logger.log('Argh in setupRX2! ' + error);
                })
            } else {
                Logger.log('Not connected to Argon!');
            }
        }

	function sendMessageToDevice( txMessage ) {
             Logger.log(`Send message to device: ${txMessage}`);
            if (particle_ble_device) {

                particle_ble_device.gatt.getPrimaryService(uuid_wifi_config_service)
                .then(service => {
                    return service.getCharacteristic(uuid_wifi_config_tx)
                })
                .then(characteristic => {
                    //Logger.log('Transmitting message: ${txMessage}');
                    //var message_str = JSON.stringify(txMessage);
                    Logger.log('TX: ' + txMessage);
                    var bytes = encoder.encode(txMessage);
                    return characteristic.writeValue(bytes);
                })

            } else {
                Logger.log('Cant sendMessageToDevice; Not connected to Argon!');
            }
        }


        function onConnectButtonClick() {
            Logger.log('Requesting Bluetooth Device...');
            navigator.bluetooth.requestDevice({
                filters: [{
                    services: [uuid_wifi_config_service], // Only show devices that have the WiFi config service enabled
                }],
            })
            .then(device => {
                // Connect to the selected device
                Logger.log('Connecting to ' + device.name + '...');
                device.addEventListener('gattserverdisconnected', onDisconnected);
                particle_ble_device = device;
                return device.gatt.connect();
            })
            .then(server => {
                // Enable additional UI elements
                Logger.log('Connected!');
                Logger.log('Setting up Rx');
		setUpRx();
                document.getElementById('connect_btn').value = "Disconnect from Argon";
            })
            .catch(error => {
                Logger.log('Argh on connect button click! ' + error);
                particle_ble_device = null;
            });
        }

        function onDisconnectButtonClick() {
            if (particle_ble_device) {
                Logger.log('Disconnecting...');
                particle_ble_device.gatt.disconnect();
            }
        }

        // Handler for when data is received from the BLE device
        function rxDataHandler(event) {
            var decoder = new TextDecoder("utf-8");
            var result_str = decoder.decode(event.target.value);
            try {
                var obj = JSON.parse(result_str);
                if (obj) {
                        Logger.log(`RX: ${result_str}`);
                    parseAndDisplay(result_str); 
                } else {
                        Logger.log("JSON obj empty?");
		}

            } catch (error) {
                Logger.log('Error parsing JSON object of rx Data : ' + error);
            }
        }

        function onDisconnected(event) {
            const device = event.target;
            Logger.log(`Device ${device.name} is disconnected.`);
            particle_ble_device = null;
            resetUI();
        }

        function resetUI() {
            document.getElementById('connect_btn').value = "Connect to Argon";
        }

	document.addEventListener("DOMContentLoaded", function(event) {
                // wait for the document to load before adding handlers to buttons

		// Connect to Argon button handler
		document.querySelector('#form_connect').addEventListener('submit', function(event) {
		    event.stopPropagation();
		    event.preventDefault();

		    if (navigator.bluetooth) {
			if (particle_ble_device) {
			    // Disconnect
			    onDisconnectButtonClick();
			} else {
			    // Connect
			    Logger.clearLog();
			    onConnectButtonClick();
			}
		    } else {
			Logger.log('Error: Web Bluetooth API is not available. ' +
				   'Please make sure the "Experimental Web Platform features" flag is enabled.');
		    }
		});

		// send message to argon handler button handler
		document.querySelector('#form_sendMessage').addEventListener('submit', function(event) {
		    event.stopPropagation();
		    event.preventDefault();
		    try {
			let messageToSend = document.getElementById('messageText').value;
			sendMessageToDevice( messageToSend  );
			messageNumber++;
		    } catch (error) {
			Logger.log('Failed to send message ' + error);
		    }
		});

	});

