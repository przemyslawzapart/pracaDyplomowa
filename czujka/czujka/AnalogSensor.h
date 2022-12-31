// AnalogSensor.h

#ifndef _ANALOGSENSOR_h
#define _ANALOGSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif




class AnalogSensorClass 
{
 protected:
	 int id;
	 int pinPosition;
	 float value;
	 int minValue;
	 int maxValue;
	 int range;
	 int error;
	 char* name;	 
	 char* unit;
	 bool errorState;
	 bool previousState;	 
	 bool reset;
	 unsigned long resetTimer;
	 bool enabled;

 public:
	void init();
	AnalogSensorClass();
	AnalogSensorClass(int pinPosition, int minValue, int maxValue, const char* name, const char* unit);
	float getValue();
	float checkValue();
	bool checkErrorState();
	void resetAnalogSensor();
	void setNewValue(const char*newValue);
	void show();
	String getSettings();
	void setNewValue(const char* _name, int minValue, int maxValue, int range, const char* unit);
};

//extern AnalogSensorClass AnalogSensor;

#endif

