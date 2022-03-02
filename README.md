# BLE WiFi Setup Manager Library
## About
This library enables a particle Argon and a web browser (Chrome, Android, Opera) to communicate
1. Exposes and advertises a custom BLE service for general Commuincations
2. Provides a simple JSON-based communication interface

## Use
1. Add this library to your project (Name: `ble-web-json-comm`)
2. Implement the provided example code
3. Web-serve 4 files found in doc/website_files

**Please note**: the web example _only_ works on Chrome running on a desktop OS (Windows/OSX/Linux), or on Android devices. iOS does not natively support the Web Bluetooth standard (yet), however, there does exist at least one paid app "Bluefy"

## API
### BLE Services and Characteristcs
The firmware exposes a custom service, which has two characteristics for communication channels. Characteristics are named from the device's perspective, meaning the device sends on `tx`, and receives on `rx`.

| Description         |                                 UUID |
|:--------------------|-------------------------------------:|
|Service              |`b4ad5b8d-d2db-44d6-9d35-5d43b9e5060c`|
|Characteristic: `tx` |`3c673f3a-382a-4835-8433-c1c1b6b65346`|
|Characteristic: `rx` |`226285d5-7a5a-448d-8317-dae1fd2d6c36`|

### Messages
All messages are in JSON format. They start with a value `msg_t` that informs either side what kind of message is being sent.

| `msg_t` | Direction | Description | Example |
|:--------|:------------|:------------|---------:|
| `scan_resp` | Response from device (`rx`) | Contains details about one WiFi access point that has been found during a scan | `{"msg_t":"scan_resp", "ssid":"MySSID", "sec":"WPA2", "ch":11, "rssi":-53}`|
| `scan` | Send to device (`tx`) | Initiates WiFi scan on the Argon | `{"msg_type":"scan"}` |
| `set_creds` | Send to device (`tx`) | Provisions credentials for a given WiFi access point | `{"msg_type":"set_creds","ssid":"MySSID","password":"MyPassword"}` |
