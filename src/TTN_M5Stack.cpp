// 
// 
// 

#include "TTN_M5Stack.h"

TaskHandle_t TTN_M5Stack::TTN_task_Handle = NULL;
bool TTN_M5Stack::_joined = false;
TTN_M5Stack::TTN_M5Stack()
{
	LoRaWanClass::init();
}

void TTN_M5Stack::begin()
{
	lora.init();
	SerialLoRa.setTimeout(1000);
	while (SerialLoRa.available())SerialLoRa.read();
	SerialLoRa.print("AT\r\n");
	Serial.readString();


}

bool TTN_M5Stack::saveKeys()
{
	return false;
}

bool TTN_M5Stack::restoreKeys(bool silent)
{
	return false;
}

bool TTN_M5Stack::haveKeys()
{
	return false;
}

size_t TTN_M5Stack::getAppEui(char * buffer, size_t size)
{
	return size_t();
}

String TTN_M5Stack::getAppEui()
{
	while (SerialLoRa.available())SerialLoRa.read();
	SerialLoRa.printf("AT+ID=AppEui\r\n");
	String response = SerialLoRa.readString();
	response.replace(":", "");
	response.replace("+ID AppEui, ", "");
	return response;
}

size_t TTN_M5Stack::getHardwareEui(char * buffer, size_t size)
{
	char buf[17];

	getAppEui().toCharArray(buf, 16);
	
	return hexStrToBin(buf, buffer, 8);
}

size_t TTN_M5Stack::getDevEui(char * buffer, size_t size, bool hardwareEui)
{
	char buf[32];
	String deveui = getDevEui();
	//Serial.println("deveui:" + deveui);
	deveui.toCharArray(buf,deveui.length()+1);
	//Serial.println("buf");
	//for (size_t i = 0; i < 16; i++)
	//{
	//	Serial.write(buf[i]); Serial.printf(",");
	//}
	//Serial.println("\nbuffer");
	hexStrToBin(buf,buffer, size);
	
	return size;

}

String TTN_M5Stack::getDevEui(bool hardwareEui)
{
	while (SerialLoRa.available())SerialLoRa.read();
	if (hardwareEui)
	{
		char hexbuf[17] = { 0 };
		uint8_t mac[6];

		esp_err_t err = esp_efuse_mac_get_default(mac);
		binToHexStr(mac, 6, hexbuf);
		for (size_t i = 0; i < 6; i++)
		{
			hexbuf[15 - i] = hexbuf[11 - i];
		}
		hexbuf[9] = 'E';
		hexbuf[8] = 'F';
		hexbuf[7] = 'F';
		hexbuf[6] = 'F';
		return (String)hexbuf;
	}
	else 
	{
		SerialLoRa.printf("AT+ID=DevEui\r\n");
		String response = SerialLoRa.readString();
		response.replace(":", "");
		response.replace("+ID DevEui, ", "");
		response.replace("\r\n", "");
		return response;
	}
}

bool TTN_M5Stack::provision(const char * appEui,  const char * appKey)
{
	char devEui[17]={ 0 };
	getDevEui(true).toCharArray(devEui, 17);
	provision((const char*)devEui,appEui, appKey);
}

bool TTN_M5Stack::provision(const char * devEui,  const char * appEui,  const char * appKey)
{
	lora.setId(NULL, (char*)devEui, (char*)appEui);
	// setKey(char *NwkSKey, char *AppSKey, char *AppKey);
	lora.setKey(NULL, NULL, (char*)appKey);
	lora.setDeciveMode(LWOTAA);
	lora.setDataRate(DR0, EU868);// DR0 : SF12
	lora.setChannel(0, 868.1);
	lora.setChannel(1, 868.3);
	lora.setChannel(2, 868.5);

	lora.setReceiceWindowFirst(0, 868.1);
	lora.setReceiceWindowSecond(869.525, DR3);
	lora.setPower(14);
	lora.setPort(1);
	return true;
}

bool TTN_M5Stack::provisionABP(const char * devAddr, const char * nwkSKey, const char * appSKey)
{
	lora.setKey((char*)nwkSKey, (char*)appSKey, NULL);
	lora.setId((char*)devAddr, NULL, NULL);
	lora.setDeciveMode(LWABP);
	lora.setDataRate(DR0, EU868);
	lora.setChannel(0, 867.7);
	lora.setChannel(1, 867.9);
	lora.setChannel(2, 868.8);
	lora.setReceiceWindowFirst(0, 867.7);
	lora.setReceiceWindowSecond(869.5, DR3);
	lora.setDutyCycle(false);
	lora.setPower(14);
	lora.setJoinDutyCycle(false);
}

bool TTN_M5Stack::join()
{
	_joined = false;
	//xTaskCreatePinnedToCore(loopJoin, "ttnTask", 2048, (void *)1, (5 | portPRIVILEGE_BIT), &TTN_task_Handle, 0);
	while (!lora.setOTAAJoin(JOIN));
		_joined = true;
		return true;	
}

bool TTN_M5Stack::join(const char * app_eui, const char * app_key, int8_t retries, uint32_t retryDelay)
{
	return false;
}

bool TTN_M5Stack::join( const char * dev_eui,  const char * app_eui,  const char * app_key, int8_t retries, uint32_t retryDelay)
{
	while (SerialLoRa.available())SerialLoRa.read();
	String deveui = getDevEui();
	String appeui = getAppEui();
	//Serial.println(dev_eui);
	//Serial.println(deveui);
	//Serial.println(app_eui);
	//Serial.println(appeui);
	//Serial.println(deveui.indexOf(dev_eui));
	//Serial.println(appeui.indexOf(app_eui));
	if (deveui.indexOf(dev_eui)<0 || appeui.indexOf(app_eui)<0){
		provision(dev_eui, app_eui, app_key);
		//Serial.println("provisionning!");
	}
	
	//provision(dev_eui, app_eui, app_key);

	join();
}

bool TTN_M5Stack::personalize(const char * devAddr, const char * nwkSKey, const char * appSKey)
{
	provisionABP(devAddr, nwkSKey, appSKey);
}

bool TTN_M5Stack::personalize()
{
	lora.setJoinDutyCycle(false);
}

void TTN_M5Stack::showStatus()
{
	while (SerialLoRa.available())SerialLoRa.read();
	SerialLoRa.printf("AT+ID\r\n");
	SerialUSB.print(SerialLoRa.readString());
	SerialUSB.print(SerialLoRa.readString());
	SerialUSB.print(SerialLoRa.readString());
}

bool TTN_M5Stack::joined()
{
	//if (_joined)
	//{
	//	Serial.println("_joined !");
	//	//Serial.println(TTN_task_Handle);
	//	if (TTN_task_Handle != NULL)
	//	{
	//		vTaskDelete(TTN_task_Handle);
	//		Serial.println("stop tache !");
	//	}
	//}
	return _joined;
}

bool TTN_M5Stack::sendBytes(uint8_t * payload, size_t length, uint8_t port, uint8_t confirm)
{
	bool reussi = false;
	if (confirm)
	{
		if (lora.transferPacketWithConfirmed(payload, length, 10)) reussi= true;
	}
	else {
		if (lora.transferPacket(payload, length, 10)) reussi= true;
	}
	if (reussi)
	{
		short length;
		short rssi;
		char buffer[256];
		length = lora.receivePacket(buffer, 256, &rssi);
		if (length)
		{
			messageCallback((const uint8_t*)buffer, length, rssi);
		}

	}
	return reussi;
}

void TTN_M5Stack::onMessage(void(*cb)(const uint8_t *payload, short size, short rssi))
{
	messageCallback = cb;
	/*short rssi;
	char buffer[256];
	short  size=lora.receivePacket(buffer, sizeof(_buffer), &rssi);
	cb((const uint8_t*) buffer, size, &rssi);*/
}

bool TTN_M5Stack::hardReset()
{

	SerialLoRa.print("AT+FDEFAULT\r\n");
	//SerialLoRa.print("AT+HELP\r\n");
#ifdef _DEBUG_SERIAL_
	lora.loraDebugPrint(10);
	return true;
#else
	if (SerialLoRa.readString().indexOf("OK")>0)
	{
		return true;
	}
	
#endif // _DEBUG_SERIAL_

	
	return false;
}

short TTN_M5Stack::readBuffer(char * buffer, short length, unsigned char timeout)
{
	short i = 0;
	unsigned long timerStart, timerEnd;

	timerStart = millis();

	while (1)
	{
		if (i < length)
		{
			while (SerialLoRa.available())
			{
				char c = SerialLoRa.read();
				buffer[i++] = c;
			}
		}

		timerEnd = millis();
		if (timerEnd - timerStart > 1000 * timeout)break;
	}

	return i;
}

bool TTN_M5Stack::hexStrToBin( char * hex, char * buf, int len)
{
	const char* ptr = hex;
	for (int i = 0; i < len; i++)
	{
		int val = hexTupleToByte(ptr);
		if (val < 0)
			return false;
		buf[i] = val;
		ptr += 2;
	}
	return true;
}

int TTN_M5Stack::hexTupleToByte(const char * hex)
{
	int nibble1 = hexDigitToVal(hex[0]);
	if (nibble1 < 0)
		return -1;
	int nibble2 = hexDigitToVal(hex[1]);
	if (nibble2 < 0)
		return -1;
	return (nibble1 << 4) | nibble2;
}

int TTN_M5Stack::hexDigitToVal(char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	if (ch >= 'A' && ch <= 'F')
		return ch + 10 - 'A';
	if (ch >= 'a' && ch <= 'f')
		return ch + 10 - 'a';
	return -1;
}

void TTN_M5Stack::binToHexStr(const uint8_t * buf, int len, char * hex)
{
	for (int i = 0; i < len; i++)
	{
		uint8_t b = buf[i];
		*hex = valToHexDigit((b & 0xf0) >> 4);
		hex++;
		*hex = valToHexDigit(b & 0x0f);
		hex++;
	}
}

char TTN_M5Stack::valToHexDigit(int val)
{
	return "0123456789ABCDEF"[val];
}

void TTN_M5Stack::swapBytes(uint8_t * buf, int len)
{
	uint8_t* p1 = buf;
	uint8_t* p2 = buf + len - 1;
	while (p1 < p2)
	{
		uint8_t t = *p1;
		*p1 = *p2;
		*p2 = t;
		p1++;
		p2--;
	}
}

bool TTN_M5Stack::isAllZeros(const uint8_t * buf, int len)
{
	for (int i = 0; i < len; i++)
		if (buf[i] != 0)
			return false;
	return true;
}

void TTN_M5Stack::loopJoin(void * parameter)
{
	for (;; )
	{	
		if( lora.setOTAAJoin(JOIN)) _joined=true;
		
	}
}
