# API Reference

The `TTN_M5Stack` class enables ESP32 devices with supported LoRaWAN modules to communicate via The Things Network. 
Needed hardware : M5Stack (M5Stack Core ESP2, M5Stack Fire)

## Class: `TTN_M5Stack`

Include and instantiate the TTN_M5Stack class. The constructor initialize the library with the Streams it should communicate with. 

```c
#include <TTN_M5Stack.h>

TTN_M5Stack ttn;
```

## Method: `getAppEui`

Gets the provisioned AppEUI. The AppEUI is set using `provision()` or `join()`.

```c
String getAppEui();
```
return AppEui as String
## Method: `getDevEui`
Gets the provisioned DevEUI. The DevEUI is set using `provision()` or `join()`.

```c
String getDevEui(bool hardwareEui=false);
```
return DevEUI as String

- `bool hardwareEui=false`: if true get DevEUI from Mac Address.


## Method: `showStatus`

Writes information about the device and LoRa module to `Serial` .

```c
void showStatus();
```

Will write something like:

```bash
---------------Status--------------
+ID: DevAddr, 26:01:1B:24
+ID: DevEui, 00:EC:3A:C7:C3:29:81:E5
+ID: AppEui, 70:B3:D5:7E:D0:01:55:75
-----------------------------------
```

## Method: `onMessage`

Sets a function which will be called to process incoming messages. You'll want to do this in your `setup()` function and then define a `void (*cb)(const byte* payload, size_t length, port_t port)` function somewhere else in your sketch.

```c
void onMessage(void (*cb)(const uint8_t *payload, short size, short rssi));
```

- `const uint8_t* payload`: Bytes received.
- `short size`: Number of bytes.
- `short rssi`: the rssi in dB.

See the [ttn-otaa](https://github.com/rgot-org/TheThingsNetwork_esp32/blob/master/examples/ttn-otaa/ttn-otaa.ino) example.

## Method: `join`

Activate the device via OTAA (default).

```c
bool join();
bool join(const char *app_eui, const char *app_key);
bool join(const char *dev_eui, const char *app_eui, const char *app_key);
```

- `const char *app_eui`: Application EUI the device is registered to.
- `const char *app_key`: Application Key assigned to the device.
- `const char *dev_eui`: Device EUI 

Returns `true` or `false` depending on whether it received confirmation that the activation was successful before the maximum number of attempts.

Call the method without the first two arguments if the device's LoRa module is provisioned or comes with NVS stored values. See `provision`, `saveKeys` and `restoreKeys`

## Method: `personalize`

Activate the device via ABP.

```c
bool personalize(const char *devAddr, const char *nwkSKey, const char *appSKey);
bool personalize();
```

- `const char *devAddr`: Device Address assigned to the device.
- `const char *nwkSKey`: Network Session Key assigned to the device for identification.
- `const char *appSKey`: Application Session Key assigned to the device for encryption.

Returns `true` or `false` depending on whether the activation was successful.

Call the method with no arguments if the device's LoRa module is provisioned or comes with NVS stored values. See `provisionABP`

See the [ttn_abp](https://github.com/rgot-org/TheThingsNetwork_esp32/tree/master/examples/ttn_abp) example.

## Method: `sendBytes`

Send a message to the application using raw bytes.

```c
bool sendBytes(uint8_t *payload, size_t length, uint8_t port = 1, uint8_t confirm = 0);
```

- `uint8_t *payload `: Bytes to send.
- `size_t length`: The number of bytes. Use `sizeof(payload)` to get it.
- `uint8_t port = 1`: The port to address. Defaults to `1`.
- `uint8_t confirm = false`: Whether to ask for confirmation. Defaults to `false`

## Method: `provision`

Sets the informations needed to activate the device via OTAA, without actually activating. Call join() without the first 2 arguments to activate.

```c
bool provision(const char *appEui, const char *appKey);
bool provision(const char *devEui, const char *appEui, const char *appKey);
```

- `const char *appEui`: Application Identifier for the device.
- `const char *appKey`: Application Key assigned to the device.
- `const char *devEui`: Device EUI.
## Method: `provisionABP`

Sets the informations needed to activate the device via ABP, without actually activating. call `personalize()` without arguments to activate.
```c
bool provisionABP(const char *devAddr, const char *nwkSKey, const char *appSKey);
```
- `const char *devAddr`: Device Address.
- `const char *nwkSKey`: Network Session Key.
- `const char *appSKey`: Application Session Key.




