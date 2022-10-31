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
	 int pinPosition;
	 float value;
	 float minValue;
	 float maxValue;
	 int error;
	 const char* name;	 
	 const char* unit;
	 bool errorState;
	 bool previousState;	 
	 bool reset;
	 unsigned long resetTimer;

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
};

//extern AnalogSensorClass AnalogSensor;

#endif

