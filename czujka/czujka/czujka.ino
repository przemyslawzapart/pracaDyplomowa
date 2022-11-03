/*
 Name:		czujka.ino
 Created:	29.10.2022 09:26:44
 Author:	Przemyslaw Zapart
*/

/*
	to do :
		- opracowac RPM
		- do?o?y? bibliotek? Time ds3231
		- biblioteka SD
		- odbieranie danych z panelu
		- wysy?anie danych do panelu
		- chaeck sum
		- pomiar adc 


*/
#include "AnalogSensor.h"
#include "Sensor.h"

const char STARTING = 0;
const char RUNNING = 1;
const char ERROR = 5;

int reset = 10;
bool resetFlag = false;
bool alarm = false;

//bool keyOff = false;
//bool keyHand = false;
//bool keyAuto = false;


int ledArray[] = { 24,23,30,31,32,48 };
int switchArray[] = {33,34,35,36,37,45};
int outputArray[] = {26,27,28,29,25,22};

int starter1 = 49;
int starter2 = 44;
int run = 43;

int keyOff = 10;
int keyAuto = 11;
int keyHand = 12;
int keyArray[] = { keyOff, keyAuto, keyHand };

bool keyOffState = false;
bool keyHandState = false;
bool keyAutoState = false;

bool runningStatus = false;
bool startingStatus = false;
bool starter1Status = false;
bool starter2Status = false;
bool startingErrorStatus = false;
bool engineStoppingStatus = false;

bool resetState = false;

bool battery = true;

bool startReset = false;

SensorClass battery1(33,24, "Start", "V");
SensorClass battery2(34,23, "Stop", "V");
SensorClass battery3(35,30, "Reset", "V");
SensorClass battery4(36, 31, "battery 1", "V");
SensorClass battery5(37, 32, "battery 2", "V");
SensorClass battery6(45, 48, "Druk", "V");
SensorClass sensorArray[] = {battery1,battery2, battery3, battery4, battery5, battery6 };
//SensorClass sensorArray[] = {battery6,battery5, battery4, battery3, battery2, battery1 };

AnalogSensorClass analog1(A0,16,300,"analog1","V");
AnalogSensorClass analog2(A0, 16, 300, "analog2", "V");
AnalogSensorClass AnalogSensorArray[] = {analog1, analog2};


unsigned long timer = 0;
unsigned long sendDataToMonitorTimer = 0;
unsigned long resetTimer = 0;


void setup() {
	Serial.begin(19200);
	Serial1.begin(19200);
	Serial2.begin(19200);
	Serial3.begin(19200);
	Serial.println("startujemy");
	analogReference(INTERNAL2V56);
	pinMode(starter1, OUTPUT);  
	pinMode(starter2, OUTPUT);
	pinMode(43, INPUT_PULLUP);  //run switch

	for (size_t i = 0; i < 6; i++)
	{
		pinMode(outputArray[i], OUTPUT);
		digitalWrite(outputArray[i], LOW);
	}
	for (size_t i = 0; i < 3; i++)	
		pinMode(keyArray[i], INPUT_PULLUP);
	
	Serial.println("key position : ");
	for (size_t i = 0; i < 3; i++)
	{
		Serial.println(digitalRead(keyArray[i]));
	}
	getkeyPosition();
	resetTimer = millis();
	Serial.println("koniec");

	
}



void loop() {	
	getEngineSensorState();
	if (millis() - resetTimer > 5000 && startReset) {
		startReset = false;
	}

	if (keyOffState) {
		;	
	}
	else if (keyAutoState && !startReset) {		
		if (getWaterPressState() && !runningStatus && !startingErrorStatus ) {
			starting();
		}
	}
	else if (keyHandState) {
		if (!runningStatus && !startingErrorStatus && !startReset) {
			while (handStartSignal()) {//start Signal
				handStart();
				getEngineSensorState();
				if (getRpm()) {
					runningStatus = true;
					break;
				}
				if (startingErrorStatus)
					break;
			}
			if (handStartingProces()) {  //bylo odpalone recznie
				digitalWrite(outputArray[STARTING], LOW);
				setEngineStarterState(starter1, LOW);
				setEngineStarterState(starter2, LOW);
				if (battery)
					battery = false;
				else
					battery = true;
			}
		}
		else {
			setEngineStarterState(starter1, LOW);
			setEngineStarterState(starter2, LOW);
		}
	}
	if (runningStatus) {
		runningMode();
	}	
}
void getEngineSensorState() {
	checkSensorState();
	//getkeyPosition();
	checkReset();	
	sendDataToMonitor();
	
}
bool getWaterPressState() {
		return sensorArray[5].Value();
}
void runningMode() {
	static bool state = false;
	unsigned long timer = 0;
	
	while (runningStatus) {
		/*if (millis() - timer > 1000){
			sendDataToMonitor();
			timer = millis();			
		}*/
		setRunningStatus();
		getEngineSensorState();
		serialEvent2();

		if ((keyAutoState && stopSignal() && !getWaterPressState() && !engineStoppingStatus) || (keyHandState && stopSignal() && !engineStoppingStatus)) {
			state = HIGH;
			sendStopSignal();
			engineStoppingStatus = true;
		}
		if (engineStoppingStatus && !getRpm()) {
			engineStopped();
			state = LOW;

		}
		if(state && !digitalRead(outputArray[2]))   // wsicnieto reset
			digitalWrite(outputArray[2], HIGH);
	}
		
}
void sendDataToMonitor() {
	if (millis() - sendDataToMonitorTimer >1000) {		
		static int a = 0;
		char data[50]{ 0 };
		char temp[20]{ 0 };
		if(a==0)
			strcpy(data, "%2049/01/12.04:44:23.50,8.22,4.26,8.10,8.23,9.000");
		if(a==1)
			strcpy(data, "%2033/10/12.14:11:15.75,7.32,4.55.88,9.66,6.555");
		else if (a == 2) 
			strcpy(data, "%2022/11/03.10:52:33.22,5.100.3,9.44,9.24,0.aaa");
		a++;
		if (a == 3)
			a = 0;
		getDigitalStateHex(temp);
		strcat(data, temp);
		strcat(data, ".37*");
		Serial.print("data to send : ");
		Serial.println(data);
		Serial2.println(data);
		sendDataToMonitorTimer = millis();
		//Serial1.println("se 1");
		
		//Serial3.println("se 3");
		
		//% 2049 / 01 / 12.04:44 : 23.50, 8.22, 4.26, 8.10, 8.23, 9.AAAAB.
	}	
}
void sendStopSignal() {
	digitalWrite(outputArray[2], HIGH);  //send stop signal
}
void setRunningStatus() {
	if (digitalRead(outputArray[RUNNING]));
		digitalWrite(outputArray[RUNNING], HIGH);
}
bool handStartSignal() {
	return sensorArray[0].Value();
}
bool stopSignal() {
	return sensorArray[2].Value();
}
void handStart() {
	digitalWrite(outputArray[STARTING], HIGH);
	if (battery) {
		if (!getBatterystate(1)) {//true start 1			
			setEngineStarterState(starter1, HIGH);
		}
		else if (!getBatterystate(2)) {
			battery = false;
		}
		else
			handStartError();
	}
	else {
		if (!getBatterystate(2)) {//true start 1			
			setEngineStarterState(starter2, HIGH);			
		}
		else if (!getBatterystate(1)) {
			battery = true;			
		}
		else
			handStartError();	
	}
}
void setEngineStarterState(int starter, bool state) {
	if (digitalRead(starter) != state)
		digitalWrite(starter, state);
}
void handStartError() {
	Serial.println("error hand start");
	digitalWrite(outputArray[STARTING], LOW);
	digitalWrite(outputArray[ERROR], HIGH);
	startingErrorStatus = true;
}
bool handStartingProces() {
	return digitalRead(outputArray[STARTING]);
}
void starting() {
	int start = 0;
	int startTime = 2000;
	unsigned long startingTimer = millis();
	bool pause = false;
	getBatterystate(1);
	getRpm();
	digitalWrite(outputArray[STARTING], HIGH);
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
			if (battery) {
				if (!getBatterystate(1)) {//true start 1
					setEngineStarterState(starter1, HIGH);
					if (millis() - startingTimer > startTime) {
						Serial.println("start batteri 1");
						start++;
						battery = false;
						setEngineStarterState(starter1, LOW);
						startingTimer = millis();
						pause = true;
					}

				}
				else if (!getBatterystate(2)) {
					battery = false;
				}
				else
					start = 6;
			}
			else {
				if (!getBatterystate(2)) {//true start 1
					setEngineStarterState(starter2, HIGH);
					if (millis() - startingTimer > startTime) {
						Serial.println("start batteri 2");
						start++;
						battery = true;
						setEngineStarterState(starter2, LOW);
						startingTimer = millis();
						pause = true;
					}
				}
				else if (!getBatterystate(1)) {
					battery = true;					
				}
				else
					start = 6;
			}
		}
		
	}
	digitalWrite(outputArray[STARTING], LOW);
	if (start == 6) {
		startingErrorStatus = true;
		Serial.println("nie udala sie wystartowac !!!");
		digitalWrite(outputArray[ERROR], HIGH);
		setEngineStarterState(starter1, LOW);
		setEngineStarterState(starter2, LOW);
	}	
}
void engineStopped() {
	Serial.println("tutaj 3");
	runningStatus = false;
	engineStoppingStatus = false;
	digitalWrite(outputArray[2], LOW);
	digitalWrite(outputArray[RUNNING], LOW);
	resetTimer = millis();
	startReset = true;
}
void engineStarted() {
	digitalWrite(outputArray[STARTING], LOW);
	digitalWrite(outputArray[RUNNING], HIGH);
	runningStatus = true;
	setEngineStarterState(starter1, LOW);
	setEngineStarterState(starter2, LOW);
}
bool getBatterystate(int battery) {
	if (battery == 1) {		
		return sensorArray[3].Value();
	}
	else {
		return sensorArray[4].Value();
	}		
}
int getRpm() {
	//to do:
	//opracowac oblicznie rpm
	return digitalRead(43);
}

void getDigitalStateHex(char* buffer) {
	int cnt = 0;
	int intBuffer = 0;
	for (size_t i = 0; i < 6; i++) {
		
		if (cnt == 4) {
			changeToHex(intBuffer, buffer);
			cnt = 0;			
			intBuffer = 0;
		}
		bool state = sensorArray[i].getValue();
		if (state) {
			intBuffer |= (1 << cnt);
			 
		}
		cnt++;
		
	}
	if (cnt > 0)
		changeToHex(intBuffer, buffer);
}
void changeToHex(int value, char* buffer) {
	char buf[3];
	sprintf(buf, "%x", value);
	strcat(buffer, buf);
}
void checkSensorState() { 
	if (!runningStatus) {
		sensorArray[0].getValue();
		sensorArray[1].getValue();
		sensorArray[2].getValue();
		sensorArray[3].getValue();
		sensorArray[4].getValue();
		sensorArray[5].getValue();
	}
	else {
		for (size_t i = 0; i < 6; i++){
			if (sensorArray[i].getValue())
				setAlarm();
		}
	}
}
void checkReset() {
	//if (digitalRead(10)) {
	if (resetState) {
		if (!resetFlag) {
			resetFlag = true;
			resetState = false;
			//battery2.resetSensor();
			Serial.println("reset"); 
			if (startingErrorStatus) {
				startReset = true;
				resetTimer = millis();
			}

			for (size_t i = 0; i < 6; i++)
			{
				sensorArray[i].resetSensor();
			}
			analog1.resetAnalogSensor();
			startingErrorStatus = false;
			for (size_t i = 0; i < 6; i++)			
				digitalWrite(outputArray[i], LOW);			
		}	
	}
	else {
		resetFlag = false;
	}
	
}
void setAlarm() {
	if (alarm) {
		;
	}
}

void checkIncomingData(char* data) {
	//Serial.println(data[0]);
	//Serial.println(data[1]);
	static int _id = 0;
	char c = data[0];
	if (c =='#') {

		Serial2.println("#A/2/arduino/11/22/50/pz*");
		//Serial.println("#");
		Serial.println("set new analog value");
		Serial.println(sizeof(data) / sizeof(data[0]));
		Serial.println(getIdSensor(data));
		int id = getIdSensor(data);
		if (id < sizeof(data) / sizeof(data[0]))
			AnalogSensorArray[getIdSensor(data)].setNewValue(data);
		else
			Serial.println("bledne dane");
	}
	else if (c == '@') {
		//Serial.println("@");
		Serial.println("new comand : ");
		*data++;
		char *buf;
		buf = strtok(data, "*");		
		Serial.println(buf);
		if (strcmp(buf, "reset") == 0)
			resetState = true;
	}
	else if (c == '$') {
		Serial.println("$");

	}
	else if (c == '!') {
		//Serial.println("!");
		Serial.println("set key position ");
		if (data[1] == 'O') {
			Serial.println("0");
			setKeyPosition(0);
		}
		else if (data[1] == 'A') {
			Serial.println("Auto");
			setKeyPosition(1);
		}
		else if (data[1] == 'H') {
			Serial.println("Hand");
			setKeyPosition(2);
		}
	}

	
	
}
void setKeyPosition(int pos) {
	if (pos == 0) {
		keyOffState = true;
		keyAutoState = false;
		keyHandState = false;		
	}
	else if (pos == 1) {
		keyOffState = false;
		keyAutoState = true;
		keyHandState = false;
	}
	else if (pos == 2) {
		keyOffState = false;
		keyAutoState = false;
		keyHandState = true;
	}
}
void getkeyPosition() {
	for (size_t i = 0; i < 3; i++)
	{
		if (digitalRead(keyArray[i]))
			setKeyPosition(i);
	}
}
int getIdSensor(const char* data) {
	char buf[3];
	int cnt = 0;
	*data++;
	while (*data != '/')
		buf[cnt++] = *data++;
	int id = atoi(buf);
	return id;
}

void serialEvent2(){
	static String data = "";
	while(Serial2.available()){
		char c = (char)Serial2.read();
		Serial.print(c);
		data += c;
		if (c == '*') {
			Serial.print("incoming data : ");
			Serial.println(data);
			data.trim();
			const char* dataChar = data.c_str();
			checkIncomingData((char*)dataChar);
			data = "";
		}
			
		
	}
}