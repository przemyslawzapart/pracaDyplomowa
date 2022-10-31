// Sensor.h

#ifndef _SENSOR_h
#define _SENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SensorClass
{
 protected:
	 int id;
	 int pinPosition;
	 int ledPosition;
	 const char* name;
	 bool value;
	 bool previousValue;
	 bool errorState;
	 bool reset;
	 const char* unit;
	 unsigned long resetTimer;

 public:
	SensorClass(int pinPosition, const char* name, const char* unit);
	SensorClass(int pinPosition, int ledPosition, const char* name, const char* unit);
	bool Value();
	bool getValue();	
	void setValue();
	void checkValue();
	void SetAlarm(bool state);
	void resetSensor();
	void show();
	void init();
};

//extern SensorClass Sensor;

#endif

