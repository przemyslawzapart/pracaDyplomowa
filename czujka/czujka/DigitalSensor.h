// DigitalSensor.h

#ifndef _DIGITALSENSOR_h
#define _DIGITALSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class DigitalSensorClass
{
 protected:
	 int id;
	 int pinPosition;
	 //int ledPosition;
	 const char* name;
	 bool value;
	 bool previousValue;
	 bool errorState;
	 bool reset;
	 const char* unit;
	 unsigned long resetTimer;
	 bool state;

 public:
	void init();
	DigitalSensorClass(int pinPosition, char* name);
	bool getStateSensor();//get pin state
	void setStateSensor(bool newState);//set pin state
	//DigitalSensorClass(int pinPosition, int ledPosition, const char* name, const char* unit);
	bool Value();
	bool getValue();
	void setValue();
	void checkValue();
	void SetAlarm(bool state);
	void resetSensor();
	void show();

	String getSettings();
};

//extern DigitalSensorClass DigitalSensor;

#endif

