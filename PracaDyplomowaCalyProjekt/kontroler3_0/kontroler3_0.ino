/*
 Name:		kontroler3_0.ino
 Created:	30.12.2022 08:02:13
 Author:	Przemyslaw Zapart
*/

#include <Wire.h>
#include <DS1307.h>
#include "Time.h"
#include <EEPROM.h>
#include "src/AnalogSensor.h"
#include "src/DigitalSensor.h"
#include "src/Global.h"

#define SYSTEM_PRESURE 0
#define OIL_PRESURE 1
#define ENGINE_TEMPERATURE 2
#define FUEL_LEVEL 3
#define FUEL_VALVE_CLOSE 4
#define FUEL_TANK_LEAK 5
#define FLOW_RAW_WATER 6
#define THERMOSTAT 7
#define CHARGER_1 8
#define CHARGER_2 9
#define STARTER_1 10
#define STARTER_2 11
#define STARTER_1 12
#define STARTER_2 13

//#define START_BATTERY_1 
//#define 
//#define 
//#define 
//#define 


TimeClass time;

DigitalSensorClass digitalSensorArray[] = { {32},{33},{34},{35},{36},{37},{38},{39},{40},{41},{42},{43},{44},{45},{ 46 }, {47} };//8,9,10,11,12,13,22,23,24,25,26,27,28,29,30,31
AnalogSensorClass analogSensorArray[] = { {A4}, {A4}, {A4}, {A4} };

//uint8_t digitalOut[] = { 8,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47 };
uint8_t digitalOutArray[] = { 8,9,10,11,12,13,22,23,24,25,26,27,28,29,30,31 };
const char* softwareVersion = "1.002";

uint8_t muxPinPosition[] = { 4,5,6,7 };
uint8_t muxAnalogPin = A4;

unsigned long timer = 0; 
//----------
bool keyOffState = false;
bool keyHandState = false;
bool keyAutoState = false;

bool keyStart = false;
bool keyStop = false;
bool keyReset = false;

//--------------
unsigned int digitalInputState = 0;
unsigned int digitalOutputState = 0;
unsigned int engineState = 0;
unsigned int rpm = 0;

//-------------------

bool resetFlag = false;
bool resetState = false;
bool startReset = false;
unsigned long resetTimer = 0;

//--------------

bool runningStatus = false;
bool startingStatus = false;
bool starter1Status = false;
bool starter2Status = false;
bool startingErrorStatus = false;
bool engineStoppingStatus = false;

bool firstStart = true;

//serial event 2
void serialEvent2();
void checkIncomingData(char* data);
bool checkSum(char* data);
void SetNweValue(char* data);
void getValue(char* data);
void checkPanelKeyStatus(char* data);
void showKeyStatus();
void sendAllSettingsToPanel();
void setSerialNumber(char* serialNumber);
void getSerialNumber(char* serialNumber);
void getVersionSoftware(char* versionSoftware);


//send data
void sendDataToPanel();
void makeStringToPanel(char* data);
void getTime(char* data);
void addRpm(char* data);
void addIntValueToString(char* data, int value);
void makeAnalogStringToPanel(char* data);
void makeCheckSum(char* data);

//sensor state
void getStnsorState();
void checkReset();
void setBit(unsigned int& value, int position);
void unSetBit(unsigned int& value, int position);
void setPinPositionMultiplexer(int position);


void SendToSd(uint8_t position);
void SendToSd(const char* data);

void setup() {
	Serial.begin(19200);
	Serial2.begin(19200);
	time.init();
	delay(500);
	setDefaultValues();	//read names from eeprom
	Serial.println("witaj");
	//sd settings 
	//time settings
	//multiplexer settings
	for (size_t i = 0; i < 4; i++)	
	{
		pinMode(muxPinPosition[i], OUTPUT);
		digitalWrite(muxPinPosition[i], LOW);
	}
	//set digital pins output
	for (size_t i = 0; i < sizeof(digitalOutArray) / sizeof(digitalOutArray[0]); i++)
	{
		pinMode(digitalOutArray[i], OUTPUT);
		digitalWrite(digitalOutArray[i], LOW);
	}
	//get all values from eeprom digital
	for (size_t i = 0; i < sizeof(digitalSensorArray) / sizeof(digitalSensorArray[0]); i++)
	{
		digitalSensorArray[i].getValuesFromEeprom();
	}
	//get all values from eeprom analog
	for (size_t i = 0; i < sizeof(analogSensorArray) / sizeof(analogSensorArray[0]); i++)
	{
		analogSensorArray[i].getValuesFromEeprom();
	}



	//show all values delete this later
	/*
	for (size_t i = 0; i < sizeof(digitalSensorArray) / sizeof(digitalSensorArray[0]); i++)
	{
		digitalSensorArray[i].show();
	}

	
	for (size_t i = 0; i < sizeof(analogSensorArray) / sizeof(analogSensorArray[0]); i++)
	{
		setPinPosition(i);
		analogSensorArray[i].measurement();
		Serial.print("anlog value = ");
		Serial.println(analogSensorArray[i].getValue());
	}
	for (size_t i = 0; i < sizeof(analogSensorArray) / sizeof(analogSensorArray[0]); i++)
	{
		analogSensorArray[i].show();
	}
	*/
	getStnsorState();
	SendToSd("Start");
}


void loop() {
  //serialEvent2();
	
	if (millis() - timer > 1000) { // check all data 
		getStnsorState();
		sendDataToPanel();
		timer = millis();		
		blink();
	}
	if (millis() - resetTimer > 5000 && startReset) {//??
		startReset = false;
		startingErrorStatus = false;
		//digitalWrite(digitalOut[2], HIGH); tutaj trzeba  sie zastanowic co i jak ustawiamy
	}

}
void blink() {
	static bool a = false;

	if (a)
		a = false;
	else
		a = true;
	for (size_t i = 0; i < sizeof(digitalOutArray) / sizeof(digitalOutArray[0]); i++)	
			digitalWrite(digitalOutArray[i], a);	
}

bool getWaterPressState() {
	return  digitalSensorArray[SYSTEM_PRESURE].getValue();
}
bool getBatteryState(int sensor) {
	return true; 
}

void setDefaultValues() {
	if (EEPROM.read(0) == 0) {
		Serial.println("nic do roboty");
		//return;
	}
	Serial.println("moge cos zmienic");
	int start = 0;
	EEPROM.write(0, 1);  //start
	EEPROM.write(1, 5);  //delay
	EEPROM.write(2, 6);  //number of attempts to start

	//serial number adr 20 -39
	{
		int adr = 20;
		Global.saveToEprom(adr, "222222222");
	}

	//software version 40 - 49
	{
		int adr = 40;
		Global.saveToEprom(40, softwareVersion);
	}

	const char* names[14] = { "System pres","Oil pres","Engin t. low", "Fuel l. low,","Fuel v closed",
						"Fuel t. leak","Flow r. water","Thermosat","Charger 1 ", "Charger 2" ,"St 1 fail", "Sta 2 fail", "st 1 run", "st2 run" };

	start = 200;
	for (size_t i = 0; i < 20; i++)
	{
		int adr = start + (i * 20);
		if (i < 14)
			Global.saveToEprom(adr, names[i]);
		else

			Global.saveToEprom(adr, "User value"); //Serial.print("name : "); Serial.println(adr);
		adr += 15;
		EEPROM.write(adr, 0); //Serial.print("state : "); Serial.println(adr);
	}

	start = 600;

	for (size_t i = 0; i < 16; i++)
	{
		int adr = start + (i * 30); //Serial.print("name : "); Serial.println(adr);
		Global.saveToEprom(adr, "AnalogValue");// name
		adr += 15; //Serial.print("unit : "); Serial.println(adr);
		Global.saveToEprom(adr, "V");//unit
		adr += 5; //Serial.print("min : "); Serial.println(adr);
		EEPROM.write(adr, 0);//min
		adr += 3; //Serial.print("max : "); Serial.println(adr);
		EEPROM.write(adr, 30);//max
		adr += 3; //Serial.print("range : "); Serial.println(adr);
		EEPROM.write(adr, 100);//range
	}
}

