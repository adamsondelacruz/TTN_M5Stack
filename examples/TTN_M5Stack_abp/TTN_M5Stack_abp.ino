/*
 * this example uses M5Stack with LoRaWan Module
 */
#include <TTN_M5Stack.h>
#include "TTN_CayenneLPP.h"

const char *devAddr = "CHANGE_ME";
const char *nwkSKey = "CHANGE_ME";
const char *appSKey = "CHANGE_ME";

TTN_M5Stack ttn;
TTN_CayenneLPP lpp;

void message(const uint8_t *payload, short size, short rssi)
{
	Serial.println("-- MESSAGE");
	Serial.print("Received " + String(size) + " bytes RSSI= " + String(rssi) + "dB");

	for (int i = 0; i < size; i++)
	{
		Serial.print(" " + String(payload[i],HEX));
	}

	Serial.println();
}

void setup() {
	Serial.begin(115200);
	Serial.println("Starting");
	ttn.begin();
	ttn.onMessage(message);// declare callback function when is downlink from server
	ttn.personalize(devAddr, nwkSKey, appSKey);
	ttn.showStatus();
}

void loop() {
	static float nb = 18.2;
	nb += 0.1;
	lpp.reset();
	lpp.addTemperature(1, nb);
	if (ttn.sendBytes(lpp.getBuffer(), lpp.getSize())) {
		Serial.printf("Temp: %f TTN_CayenneLPP: %d %x %02X%02X\n", nb, lpp.getBuffer()[0], lpp.getBuffer()[1], lpp.getBuffer()[2], lpp.getBuffer()[3]);
	}
	delay(10000);
}
