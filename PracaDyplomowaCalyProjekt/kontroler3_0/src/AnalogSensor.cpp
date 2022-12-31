

#include "AnalogSensor.h"
#include "Global.h"
#include<EEPROM.h>
#include "Arduino.h"

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
	int adr = 600 + (id * 30);
	//Serial.print("id: ");
	//Serial.println(this->id);
	//Serial.print("adr 1: ");
	//Serial.println(adr);
	////Global.saveToEprom(adr, "nazwa");
	//char data[20];
	Global.getFromEeprom(adr, name);
	//char c = (char)EEPROM.read(adr++);
	//int index = 0;
	//while (c != 0) {
	//	Serial.println(c);
	//	name[index++] = c;
	//	c = (char)EEPROM.read(adr++);
	//}
	//	
	//
	////strcpy(name, data);
	////Global.getFromEeprom(adr, name);
	adr += 15;
	//adr = 400 + (id * 45);
	/*unit[0] = (char)EEPROM.read(adr);
	Serial.print("adr 2: ");
	Serial.println(adr);*/
	/*Global.saveToEprom(adr, "uni");
	Global.getFromEeprom(adr, data);
	strcpy(unit, data);*/
	//Global.saveToEprom(adr, "PPPP");
	Global.getFromEeprom(adr, unit);
	//strcpy(unit, "HHH");
	adr += 5;
	minValue = EEPROM.read(adr);
	adr += 3;
	maxValue = EEPROM.read(adr);
	adr += 3;
	range = EEPROM.read(adr);
}
void AnalogSensorClass::changeValues(char *_name, char *_unit, int _min, int _max, int _range) {
	int adr = 600 + (id * 30);
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

AnalogSensorClass AnalogSensor;
