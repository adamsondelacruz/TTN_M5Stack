/*
    Name:       M5Stack_TTN_ENV.ino
    Created:	07/02/2020 10:14:10
    Author:     I5-FR\francois
*/
//#include <M5Stack.h>
#include "TTN_M5Stack.h"
#include <TTN_CayenneLPP.h>

char *devEui = "0EC3AC7C32981E5"; //changer avec de devEui TTN
char *appEui = "70B3D57ED0015575"; // changer avec le appEui TTN
char *appKey = "DFF6EC5B347AE5BBE6395B06FCD974A7"; // changer avec le appKey TTN

TTN_M5Stack ttn;
TTN_CayenneLPP lpp;

void message(const uint8_t *payload, short size, short rssi)
{
	Serial.println("-- MESSAGE");
	Serial.printf("Received %d bytes niveau %d\n",size,rssi);
	for (int i = 0; i < size; i++)
	{
		Serial.printf("%02X ",payload[i]);
		//Serial.write(payload[i]);
	}

	Serial.println();
}

void setup() {
	Serial.begin(115200);
	Serial.println("Starting");
	ttn.begin();
	ttn.onMessage(message);// declare callback function when is downlink from server
    Serial.println("joining TTN ");
	Serial.println("devEUI:"+ttn.getDevEui());
	ttn.join(devEui, appEui, appKey);

	ttn.showStatus();
 Serial.println("joined!");
}

void loop() {
	static float nb = 18.2;
	nb += 0.1;
	lpp.reset();
	lpp.addTemperature(1, nb);
	if (ttn.sendBytes(lpp.getBuffer(), lpp.getSize()),1) {
		Serial.printf("Temp: %f TTN_CayenneLPP: %d %x %02X%02X\n", nb, lpp.getBuffer()[0], lpp.getBuffer()[1], lpp.getBuffer()[2], lpp.getBuffer()[3]);
	}
	delay(10000);
}
