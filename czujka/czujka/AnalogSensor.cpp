// 
// 
// 
#include "AnalogSensor.h"
#include "Sensor.h"



AnalogSensorClass::AnalogSensorClass() {};
AnalogSensorClass::AnalogSensorClass(int pinPosition,int minValue, int maxValue, const char* name, const char* unit) {
	this->pinPosition = pinPosition;
	this->minValue = minValue;
	this->maxValue = maxValue;
	this->unit = unit;
	this->name = name;
	value = 25.6;
	errorState = false;
	reset = false;
	pinMode(pinPosition, INPUT_PULLUP);
}

float AnalogSensorClass::getValue() {
	checkValue();
	return value;
}
float AnalogSensorClass::checkValue() {
	//tutaj tzrba sobie zmierzyc jak wartosc
	/*to do 
	
	errors
	zapisane na 2 bitach 
	0001 error  za niskie
	0010 error  za wysokie

	*/
	Serial.print("wartosc : ");
	Serial.println(analogRead(pinPosition));
	value = analogRead(pinPosition);

	if (reset && (millis() - resetTimer >5000)) {
		reset = false;
	}

	else if (!reset) {
		if (value < minValue) {
			error |= (1 << 0);
			errorState = true;
		}
		if (value > maxValue) {
			error |= (1 << 1);
			errorState = true;
		}
	}

	Serial.print("error : ");
	Serial.println(error);
}
bool AnalogSensorClass::checkErrorState() {
	return errorState;
}
void AnalogSensorClass::resetAnalogSensor() {
	errorState = false;
	error = 0;
	reset = true;
	resetTimer = millis();
}

void AnalogSensorClass::setNewValue(const char* newValue) {

	//Serial.println(newValue);
	//show();	
	
	char valueToSet[10]{ 0 };
	char value[10]{ 0 };
	int i = 0;
	while (*newValue != '/')
		*newValue++;
	*newValue++;
	while (*newValue != '/') 
		valueToSet[i++] = (*newValue++);	
	i = 0;
	*newValue++;
	while (*newValue != '*')
		value[i++] = *newValue++;

	Serial.println(valueToSet);
	Serial.println(value);

	if (strcmp(valueToSet, "maxValue") == 0)
		maxValue = atoi(value);
	else if (strcmp(valueToSet, "minValue") == 0)
		minValue = atoi(value);
	else if (strcmp(valueToSet, "unit") == 0)
		unit = value;		 
	show();
}

void  AnalogSensorClass::show() {
	Serial.print("show: ");
	Serial.println(name);
	Serial.print("min : ");
	Serial.print(minValue);
	Serial.print("max : ");
	Serial.print(maxValue);
	Serial.print("unit : ");
	Serial.println(unit);	

}
void AnalogSensorClass::init(){}


//AnalogSensorClass AnalogSensor;

