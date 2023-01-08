

#include "AnalogSensor.h"
#include "Global.h"
#include<EEPROM.h>
#include "Arduino.h"

#define EEPROM_START_ANALOG     	1400

int value = 0;


void AnalogSensorClass::init(){}
AnalogSensorClass::AnalogSensorClass() {}
AnalogSensorClass::AnalogSensorClass(int pinPosition) {
	Serial.begin(19200);
	static  int _id = 0;
	this->pinPosition = pinPosition;
	this->id = _id++;
	//value = 0;
	strcpy(this->name,"");
	strcpy(this->unit, "");
	//getValuesFromEeprom();
	
	
}

void AnalogSensorClass::getValuesFromEeprom() {
	//int i = id;
	int adr = EEPROM_START_ANALOG + (id * 30);
	Global.getFromEeprom(adr, name);
	adr += 15;
	Global.getFromEeprom(adr, unit);
	adr += 5;
	minValue = EEPROM.read(adr);
	adr += 3;
	maxValue = EEPROM.read(adr);
	adr += 3;
	range = EEPROM.read(adr);

	/*Serial.println(name);
	Serial.println(unit);
	Serial.println(minValue);
	Serial.println(maxValue);
	Serial.println(range);*/
}
void AnalogSensorClass::changeValues(char *_name, char *_unit, int _min, int _max, int _range) {
	int adr = EEPROM_START_ANALOG + (id * 30);
	Global.saveToEprom(adr, _name);
	adr += 15;
	Global.saveToEprom(adr, _unit);
	adr += 5;
	EEPROM.update(adr, _min);
	adr += 3;
	EEPROM.update(adr, _max);
	adr += 3;
	EEPROM.update(adr, _range);

	getValuesFromEeprom();


}
char AnalogSensorClass::getSettingsValues(char *buffer) {
	
	char buf[10];
	strcpy(buffer, "@/");

	//itoa(id, buf, 10);
	sprintf(buf, "%d", id);
	strcat(buffer, buf);
	strcat(buffer, "/");

	strcat(buffer, name);
	strcat(buffer, "/");

	strcat(buffer, unit);
	strcat(buffer, "/");
	
	itoa(minValue, buf, 10);
	strcat(buffer, buf);
	strcat(buffer, "/");

	//sprintf(buf, "%d", 55);
	itoa(maxValue, buf, 10);
	strcat(buffer, buf);
	strcat(buffer, "/");
	itoa(range, buf, 10);
	//sprintf(buf, "%d", range);
	strcat(buffer, buf);
	strcat(buffer, "/");
	strcat(buffer, "*");
	
}
void AnalogSensorClass::measurement() {
	int sensorValue = analogRead(pinPosition);
	/*Serial.print("mesaurment : ");
	Serial.print(sensorValue);*/
	value = (sensorValue*10) * (5.00 / 1023);	
	/*Serial.print(" , value : ");
	Serial.println(value);*/
}

void AnalogSensorClass::getFlaotValue(char *data) {

	;

}

int  AnalogSensorClass::getValue() {
	return value;
}
void AnalogSensorClass::show() {
	Serial.print("id : ");
	Serial.print(id);
	Serial.print(", pin : ");
	Serial.print(pinPosition);
	Serial.print(" , name : ");
	Serial.print(name);
	Serial.print(" , unit : ");
	Serial.print(unit);
	Serial.print(", min : ");
	Serial.print(minValue);
	Serial.print(", max : ");
	Serial.print(maxValue);
	Serial.print(", range : ");
	Serial.println(range);

}
bool AnalogSensorClass::getState() {
	if (value < minValue)
		return false;
	else 
		return true;
}
AnalogSensorClass AnalogSensor;

