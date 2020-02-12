// TTN_M5Stack.h

#ifndef _TTN_M5STACK_h
#define _TTN_M5STACK_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "LoRaWan.h"
#include "nvs_flash.h"
class TTN_M5Stack : public LoRaWanClass
{
protected:


public:
	TTN_M5Stack();
	void begin();
	bool saveKeys();
	bool restoreKeys(bool silent = true);
	bool haveKeys();
	//bool decodeKeys(const char *dev_eui, const char *app_eui, const char *app_key);
	//bool fromMAC(const char *app_eui, const char *app_key);	
	size_t getAppEui(char *buffer, size_t size);
	String getAppEui();
	size_t getHardwareEui(char *buffer, size_t size);
	size_t getDevEui(char *buffer, size_t size, bool hardwareEui = false);
	String getDevEui(bool hardwareEui = false);
	bool provision(char *appEui, char *appKey);
	bool provision(char *devEui, char *appEui, char *appKey);
	bool provisionABP(const char *devAddr, const char *nwkSKey, const char *appSKey);
	bool join();
	bool join(const char *app_eui, const char *app_key, int8_t retries = -1, uint32_t retryDelay = 10000);
	bool join(char *dev_eui, char *app_eui, char *app_key, int8_t retries = -1, uint32_t retryDelay = 10000);
	bool personalize(const char *devAddr, const char *nwkSKey, const char *appSKey);
	bool personalize();
	void showStatus();
	bool joined();
	bool sendBytes(uint8_t *payload, size_t length, uint8_t port = 1, uint8_t confirm = 0);
	void onMessage(void(*cb)(const uint8_t *payload, short size, short rssi));
	bool hardReset();
private:
	short readBuffer(char* buffer, short length, unsigned char timeout = DEFAULT_TIMEOUT);
	static bool _joined ;
	bool hexStrToBin(char *hex, char *buf, int len);
	int hexTupleToByte(const char *hex);
	int hexDigitToVal(char ch);
	void binToHexStr(const uint8_t* buf, int len, char* hex);
	char valToHexDigit(int val);
	void swapBytes(uint8_t* buf, int len);
	bool isAllZeros(const uint8_t* buf, int len);
	char _buffer[256];
	void(*messageCallback)(const uint8_t *payload, short size, short rssi);
	static void loopJoin(void * parameter);
	static TaskHandle_t TTN_task_Handle;

};



#endif

