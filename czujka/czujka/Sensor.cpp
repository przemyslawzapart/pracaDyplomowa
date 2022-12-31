

#include "Sensor.h"

void SensorClass::init(){
} 
SensorClass::SensorClass(int pinPosition, const char *name, const char *unit) {
	static int _id = 0;
	this->id = _id++;
	this->pinPosition = pinPosition;
	this->name = name;
	this->unit = unit;
	value = false;
	pinMode(pinPosition, INPUT_PULLUP);
}
SensorClass::SensorClass(int pinPosition, int ledPosition,const char* name, const char* unit) {
	static int _id = 0;
	this->id = _id++;
	this->pinPosition = pinPosition;
	this->ledPosition = ledPosition;
	this->name = name;
	this->unit = unit;
	value = false;
	previousValue = false;
	errorState = false;
	reset = false;
	state = true;	//true - NO , false - NC

	pinMode(pinPosition, INPUT_PULLUP);
	pinMode(ledPosition, OUTPUT);
	
}
bool SensorClass::Value() {
	return digitalRead(pinPosition);
}
bool SensorClass::getValue() {	
	//return digitalRead(pinPosition);
	checkValue();
	return errorState;
}
void SensorClass::setValue() {

}
void SensorClass::checkValue() {
	value =digitalRead(pinPosition);

	if (reset && ( millis() - resetTimer > 3000)) {
		reset = false;
		//Serial.println("ok wskoczyl timer reset");
	}
	else if (!reset) {
		if (value) {
			SetAlarm(true);
			errorState =  true;
			previousValue = true;
		}
		else {
			if (previousValue) {
				//Serial.print("nie zresetowany jeszcze jest : ");
				SetAlarm(true);
				errorState = true;
			}		
		}
	}
		
}


void SensorClass::SetAlarm(bool state) {
	if (state) {
		digitalWrite(ledPosition, HIGH);
		//Serial.println("wysoki : ");
		
	}
	else{
		digitalWrite(ledPosition, LOW);
		//Serial.println("niski : ");		
		errorState = false;
	}
}
void SensorClass::show() {
	Serial.print(id);
	Serial.print("  ");
	Serial.print(pinPosition); 
	Serial.print("  ");
	Serial.print(name); 
	Serial.print("  ");
	Serial.println(unit);
}
void SensorClass::resetSensor() {
	SetAlarm(false);
	//Serial.println("resetowanie w klasie");
	previousValue = false;
	reset = true;
	resetTimer = millis();
}
String SensorClass::getSettings() {
	char buf[20];
	String data = "#D/";
	sprintf(buf, "%d", id);
	data += buf;
	data += "/";
	data += name;
	data += "/";
	if(state)
		data += "1";
	else
		data += "0";
	data += "*";
	return data;

}
//SensorClass Sensor;

