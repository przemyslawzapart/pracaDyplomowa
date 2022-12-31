// 
// 
// 

#include "DigitalSensor.h"

void DigitalSensorClass::init(){}

DigitalSensorClass::DigitalSensorClass(int pinPosition, char* name) {
	static int _id = 0;
	this->id = _id++;

	this->pinPosition = pinPosition;	
	this->name = name;	
	value = false;
	previousValue = false;
	errorState = false;
	reset = false;		//czas po wcisnieciu przycisku reset
	state = true;	//true - NO , false - NC

	pinMode(pinPosition, INPUT_PULLUP);
}

bool DigitalSensorClass::getStateSensor() {
	state =  digitalRead(pinPosition);
	return state;
}

void DigitalSensorClass::setStateSensor(bool newState) {	
	digitalWrite(pinPosition, newState);
}
bool DigitalSensorClass::getValue() {
	//return digitalRead(pinPosition);
	checkValue();
	return errorState;
}
void DigitalSensorClass::checkValue() {
	value = digitalRead(pinPosition);

	if (reset && (millis() - resetTimer > 3000)) {
		reset = false;
		//Serial.println("ok wskoczyl timer reset");
	}
	else if (!reset) {
		if (value) {
			SetAlarm(true);
			errorState = true;
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

void DigitalSensorClass::SetAlarm(bool state) {
	if (state) {
		//digitalWrite(ledPosition, HIGH);
		Serial.println("wysoki : ");

	}
	else {
		//digitalWrite(ledPosition, LOW);
		Serial.println("niski : ");		
		errorState = false;
	}
}
void DigitalSensorClass::show() {
	Serial.print(id);
	Serial.print("  ");
	Serial.print(pinPosition);
	Serial.print("  ");
	Serial.print(name);
	Serial.print("  ");
	Serial.println(unit);
}
void DigitalSensorClass::resetSensor() {
	SetAlarm(false);
	//Serial.println("resetowanie w klasie");
	previousValue = false;
	reset = true;
	resetTimer = millis();
}
String DigitalSensorClass::getSettings() {
	char buf[20];
	String data = "#D/";
	sprintf(buf, "%d", id);
	data += buf;
	data += "/";
	data += name;
	data += "/";
	if (state)
		data += "1";
	else
		data += "0";
	data += "*";
	return data;

}






//DigitalSensorClass DigitalSensor;

