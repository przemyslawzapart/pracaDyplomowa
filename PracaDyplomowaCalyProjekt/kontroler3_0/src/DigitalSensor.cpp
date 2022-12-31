// 
// 
// 



#include <EEPROM.h>

#include "DigitalSensor.h"
#include "Global.h"
void DigitalSensorClass::init(){}
DigitalSensorClass::DigitalSensorClass() {}
DigitalSensorClass::DigitalSensorClass(int pinPosition) {
	static unsigned int _id = 0;
	this->id = _id++;
	this->pinPosition = pinPosition;
	pinMode(pinPosition, INPUT_PULLUP);
	strcpy(this->name,"default");
	//getValuesFromEeprom();		
}
void DigitalSensorClass::getValuesFromEeprom() {
	int adr = 200 + (id * 20);	
	Global.getFromEeprom(adr, name);
	/*int pos = 0;
	char c = (char)EEPROM.read(adr++);
	while (c != 0) {
		name[pos++] = c;
		c = (char)EEPROM.read(adr++);
	}
	name[pos] = 0;*/
	adr += 15;
	this->contactType = (int)EEPROM.read(adr);

}
void DigitalSensorClass::changeValue(char *name, int contact) {
	int adr = 200 + (id * 20);
	Serial.print("id : ");
	Serial.println(id);
	Serial.print("adr : ");
	Serial.println(adr);
	Serial.print("name : ");
	Serial.println(name);
	Serial.print("contact ");
	Serial.println(contact);
	Global.saveToEprom(adr,(const char*)name);
	//adr += 15;
	adr = 200 + (id * 20) +15;
	EEPROM.update(adr, contact);
	getValuesFromEeprom();
}

void DigitalSensorClass::setState(bool state) {
	digitalWrite(pinPosition, state);
}
char* DigitalSensorClass::getName() {
	return name;
}
int DigitalSensorClass::getContactType() {
	return contactType;
}
bool DigitalSensorClass::getState() {
	return digitalRead(pinPosition);
}


bool DigitalSensorClass::getValue() {
	//return digitalRead(pinPosition);
	Serial.print("value : ");
	Serial.println(value);

	checkValue();
	return errorState;
}

void DigitalSensorClass::checkValue() {	
	value = digitalRead(pinPosition);
	if (reset && (millis() - resetTimer > 3000)) {
		reset = false;		
	}
	else if (!reset) {
		if (value || previousValue) {
			previousValue = true;
			errorState = true;
		}
		else
			errorState = false;
	}
}


void DigitalSensorClass::SetAlarm(bool state) { //chyba nie jest juz potrzebne
	if (state) {
		//digitalWrite(ledPosition, HIGH);
		//Serial.println("wysoki : ");
		;

	}
	else {
		//digitalWrite(ledPosition, LOW);
		//Serial.println("niski : ");		
		errorState = false;
	}
}

void DigitalSensorClass::resetSensor() {
	SetAlarm(false);
	//Serial.println("resetowanie w klasie");
	previousValue = false;
	reset = true;
	resetTimer = millis();
	Serial.println("reste sensor -----------------------------");
}







void DigitalSensorClass::getFromEeprom(int address, char* data) {
	strcpy(data, "");
	int pos = 0;
	char c = EEPROM.read(address++);
	while (c != '\n') {
		data[pos++] = c;
		c = EEPROM.read(address++);
	}
}
void DigitalSensorClass::getSettingsValues(char* buffer) {
	char buf[10];
	strcpy(buffer, "$/");
	itoa(id, buf, 10);
	strcat(buffer, buf);
	strcat(buffer, "/");
	strcat(buffer, name);
	strcat(buffer, "/");
	itoa(contactType, buf, 10);
	strcat(buffer, buf);
	strcat(buffer, "/");
	strcat(buffer, "*");

}
void DigitalSensorClass::show() {
	Serial.print(id);
	Serial.print("  ");
	Serial.print(pinPosition);
	Serial.print("  ");
	Serial.print(name);
	Serial.print("  ");
	Serial.println(contactType);
}

//
//DigitalSensorClass DigitalSensor;

