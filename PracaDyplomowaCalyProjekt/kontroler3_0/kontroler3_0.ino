/*
 Name:		kontroler3_0.ino
 Created:	30.12.2022 08:02:13
 Author:	Przemyslaw Zapart
*/

#include <SD.h>
#include <Wire.h>
#include <DS1307.h>
#include "Time.h"
#include <EEPROM.h>
#include "src/AnalogSensor.h"
#include "src/DigitalSensor.h"
#include "src/Global.h"


//difine digital in
#define SYSTEM_PRESURE			0
#define OIL_PRESURE				1
#define ENGINE_TEMPERATURE		2
#define FUEL_LEVEL				3
#define FUEL_VALVE_CLOSE		4
#define FUEL_TANK_LEAK			5
#define FLOW_RAW_WATER			6
#define THERMOSTAT				7
#define CHARGER_1_FAIL			8
#define CHARGER_2_FAIL			9
#define STARTER_1_FAIL			10
#define STARTER_2_FAIL			11

#define BATTERY_1				12
#define BATTERY_2				13
#define RPM						14
#define USER_2					15

//define digital out
#define STARTER_1				0
#define STARTER_2				1
#define STARTING				2
#define RUNNING					3
#define ERROR					4
#define STOP					5
#define READY					6
//#define 
//#define 

//define engine state
#define ENGINE_RUNNING			0
#define ENGINE_STARTING			1
#define ENGINE_READY			2
#define ENGINE_ERROR			3
#define BATTERY1_FAIL			4
#define BATTERY2_FAIL			5


TimeClass time;

DigitalSensorClass digitalSensorArray[] = { {32},{33},{34},{35},{36},{37},{38},{39},{40},{41},{42},{43},{44},{45},{ 46 }, {47} };//8,9,10,11,12,13,22,23,24,25,26,27,28,29,30,31
AnalogSensorClass analogSensorArray[] = { {A4}, {A4}, {A4}, {A4} };

uint8_t digitalOutArray[] = { 8,9,10,11,12,13,22,23,24,25,26,27,28,29,30,31 };
const char* softwareVersion = "1.002";

bool flagHandStarting = false;		//sprawdza czy przycisk hand start został pryciśnięty
bool flagBattery = true;			//true - bateria 1 , false - bateria 2


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
bool readyStatus = false;
bool startingStatus = false;
bool starter1Status = false;
bool starter2Status = false;
bool startingErrorStatus = false;
bool engineStoppingStatus = false;

unsigned int engineErrorStatus = 0;

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

	getStnsorState();
	SendToSd("Start");

	setEngineStatus(ENGINE_READY, true);
}


void loop() {
	


	if (millis() - timer > 100) { // check all data 
		getEngineSensorState();
		timer = millis();		
		setBatteryState();
		//blink();
	}
	if (millis() - resetTimer > 5000 && startReset) {//??
		startReset = false;
		startingErrorStatus = false;
	}
	if (!runningStatus && !startingErrorStatus && !startReset && !keyOffState) {
		setEngineStatus(ENGINE_READY, true);
	}	
	if (keyOffState) {
		setEngineStatus(ENGINE_READY, false);				
	}
	 if (keyAutoState && !startReset) {//na razie ie wiem dlaczego tak zrobiłem zastanów sie co to ma znaczyć 
		if (getWaterPressState() && !runningStatus && !startingErrorStatus) {
			starting();
		}
	}
	
	else if (keyHandState) 
	{
		if (!runningStatus && !startingErrorStatus && !startReset) 
		{
			while (keyStart) 
			{
				getEngineSensorState();				
				handStart();					
				if (!flagHandStarting) {					
					flagHandStarting = true;
					Serial.println("hand start ");
					setEngineStatus(ENGINE_READY, false);
					setEngineStatus(ENGINE_STARTING ,true);
					Serial.println("hand start koniec ");
				}
					
				if (getRpm()) 
				{
					runningStatus = true;
					setEngineStatus(ENGINE_RUNNING, true);				
					break;
				}
				if (startingErrorStatus) {

					break;
				}					
			}
			if (flagHandStarting) //bylo odpalone recznie
			{
				setStartersState(LOW);
				setEngineStatus(ENGINE_STARTING, false);
				if (flagBattery)			//następnym razem zostanie wybrana inna bateria do rozruchu
					flagBattery = false;
				else
					flagBattery = true;
				flagHandStarting = false;
			}
		}
		else {
			setStartersState(LOW);

		}
	}
	if (runningStatus) {
		runningMode();
	}	
}
void setEngineStatus(uint8_t position, bool state){

	if (!state) {
		engineErrorStatus &= ~(1 << position);
	}
	else {
		engineErrorStatus |= (1 << position);
	}
}	
void runningMode() {
	static bool state = false;
	static bool readyToStop = false;
	unsigned long timer = 0;
	setEngineStatus(ENGINE_RUNNING, true);

	while (runningStatus) {		
		getEngineSensorState();

		if ((keyAutoState && stopSignal() && !getWaterPressState() && !engineStoppingStatus) || (keyHandState && stopSignal() && !engineStoppingStatus)) {
			state = HIGH;
			sendStopSignal();
			engineStoppingStatus = true;
			
		}
		if (!getRpm() && engineStoppingStatus && !readyToStop) {
			timer = millis();
			readyToStop = true;
		}

		if (millis() - timer > 3000 && readyToStop) {
			engineStopped();
			state = LOW;
			readyToStop = false;
		}
	}

}
void setErrorState(bool state) {
	digitalWrite(digitalOutArray[ERROR], state); 
	setEngineStatus(ENGINE_ERROR, state);
}
void engineStopped() {
	
	Serial.println("tutaj 3");
	runningStatus = false;
	engineStoppingStatus = false;
	//digitalWrite(digitalOutArray[2], LOW);
	//digitalWrite(digitalOutArray[RUNNING], LOW);
	digitalWrite(digitalOutArray[STOP], LOW);
	//digitalWrite(digitalOutArray[READY], HIGH);
	setEngineStatus(ENGINE_RUNNING, false);
	setEngineStatus(ENGINE_READY, true);
	resetTimer = millis();
	startReset = true;
}
void sendStopSignal() {
	digitalWrite(digitalOutArray[STOP], HIGH);  //send stop signal
}

bool stopSignal() {
	return keyStop;
}
void getEngineSensorState() {
	serialEvent2();
	getStnsorState();	
	checkReset();
	sendDataToPanel();
}

void setStartersState(bool state) {
	digitalWrite(digitalOutArray[STARTER_1], state);
	digitalWrite(digitalOutArray[STARTER_2], state);
}
void handStart() {

	if (flagBattery) {
		if (!getBatterystate(1)) {//true start 1			
			digitalWrite(digitalOutArray[STARTER_1], HIGH);
		}
		else if (!getBatterystate(2)) {
			flagBattery = false;
		}
		else
			handStartError();
	}
	else {
		if (!getBatterystate(2)) {//true start 1			
			digitalWrite(digitalOutArray[STARTER_2], HIGH);
		}
		else if (!getBatterystate(1)) {
			flagBattery = true;
		}
		else
			handStartError();
	}
}

void handStartError() {
	Serial.println("error hand start");
	digitalWrite(digitalOutArray[ERROR], HIGH);
	setEngineStatus(ENGINE_ERROR, true);
	startingErrorStatus = true;
	startReset = true;
	resetTimer = millis();
}
void starting() {	
	setEngineStatus(ENGINE_READY, false);
	int start = 0;
	int startTime = 2000;
	unsigned long startingTimer = millis();
	bool pause = false;
	getBatterystate(1);
	getRpm();
	while (start < 6) {
		getEngineSensorState();
		if (getRpm()) {
			engineStarted();
			break;
		}
		if (pause) {
			if (millis() - startingTimer > startTime) {
				pause = false;
				startingTimer = millis();
			}

		}
		else {
			getRpm();
			if (flagBattery) {
				if (!getBatterystate(1)) {//true start 1
					setEngineStatus(ENGINE_STARTING, true);
					digitalWrite(digitalOutArray[STARTER_1], HIGH);
					if (millis() - startingTimer > startTime) {
						Serial.println("start batteri 1");
						start++;
						flagBattery = false;
						digitalWrite(digitalOutArray[STARTER_1], LOW);
						setEngineStatus(ENGINE_STARTING, false);
						startingTimer = millis();
						pause = true;
					}

				}
				else if (!getBatterystate(2)) {
					flagBattery = false;
				}
				else
					start = 6;
			}
			else {
				if (!getBatterystate(2)) {//true start 1
					setEngineStatus(ENGINE_STARTING, true);
					digitalWrite(digitalOutArray[STARTER_2], HIGH);
					if (millis() - startingTimer > startTime) {
						Serial.println("start batteri 2");
						start++;
						flagBattery = true;
						setEngineStatus(ENGINE_STARTING, true);
						digitalWrite(digitalOutArray[STARTER_2], LOW);
						startingTimer = millis();
						pause = true;
					}
				}
				else if (!getBatterystate(1)) {
					flagBattery = true;
				}
				else
					start = 6;
			}
		}

	}
	if (start == 6) {
		startingErrorStatus = true;
		Serial.println("nie udala sie wystartowac !!!");
		digitalWrite(digitalOutArray[ERROR], HIGH);
		setEngineStatus(ENGINE_ERROR, true);
		digitalWrite(digitalOutArray[STARTER_1], LOW);
		digitalWrite(digitalOutArray[STARTER_2], LOW);
	}
}
void engineStarted() {
	setEngineStatus(ENGINE_STARTING, false);
	setEngineStatus(ENGINE_RUNNING, true);
	runningStatus = true;
	digitalWrite(digitalOutArray[STARTER_1], LOW);
	digitalWrite(digitalOutArray[STARTER_2], LOW);
}
int getRpm() {
	static long timer = millis();
	static bool state = false;
	if (millis() - timer > 3000 && !state) {
		state = true;
		return true;
	}

	else
		return digitalSensorArray[RPM].getState();
}
void setBatteryState() {
	setEngineStatus(BATTERY1_FAIL, digitalSensorArray[BATTERY_1].getState());
	setEngineStatus(BATTERY2_FAIL, digitalSensorArray[BATTERY_2].getState());
}
bool getBatterystate(int8_t battery) {
	bool state = true;
	//powinien z analog sprawdzic jaka jest wrtosc
	if (battery == 1) {		
		//return digitalSensorArray[BATTERY_1].getValue();
		if (digitalSensorArray[BATTERY_1].getState() || digitalSensorArray[STARTER_1_FAIL].getState())
			return true;
		else 
			return false;
		//return digitalSensorArray[BATTERY_1].getState();
	}
	else {	
		if (digitalSensorArray[BATTERY_2].getState() || digitalSensorArray[STARTER_2_FAIL].getState())
			return true;
		else
			return false;
		//return digitalSensorArray[BATTERY_2].getValue();
		//return digitalSensorArray[BATTERY_2].getState();
	}
}
bool handStartingProces() {
	return flagHandStarting;// digitalRead(outputArray[STARTING]);
}
bool getWaterPressState() {
	return  digitalSensorArray[SYSTEM_PRESURE].getState();
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

void setDefaultValues() {
	if (EEPROM.read(0) == 0) {
		Serial.println("nic do roboty");
		return;
	}
	Serial.println("moge cos zmienic");
	int start = 0;
	EEPROM.write(0, 0);  //start
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

