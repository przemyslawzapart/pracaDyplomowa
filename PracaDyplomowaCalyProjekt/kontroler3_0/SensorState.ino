
void getStnsorState() {
	//serialEvent();
	checkReset();
	for (size_t i = 0; i < sizeof(digitalSensorArray) / sizeof(digitalSensorArray[0]); i++)
	{
		
		bool currentState = digitalSensorArray[i].getValue();
		bool previousState = digitalInputState & (1 << i);

		if (currentState != previousState) {
			if (currentState) {
				//Serial.println("ustaw jakiegos bita ________________");
				if (!firstStart) {
					SendToSd(i);
				}

				digitalInputState |= (1 << i);
			}
			else {
				digitalInputState &= ~(1 << i);
			}
		}
	}
	//Serial.println(digitalInputState ,BIN);
	//Serial.println(digitalInputState, HEX);
	for (size_t i = 0; i < sizeof(digitalOutArray) / sizeof(digitalOutArray[0]); i++)  //tutaj trzeba sobie dopracoac co i jak 
	{
		if (digitalRead(digitalOutArray[i]))
			digitalOutputState |= (1 << i);
			//setBit(digitalOutputState, i);
		else
			digitalOutputState &= ~(1 << i);
			//unSetBit(digitalOutputState, i);
	}

	for (size_t i = 0; i < sizeof(analogSensorArray) / sizeof(analogSensorArray[0]); i++)
	{
		setPinPositionMultiplexer(i);
		analogSensorArray[i].measurement();
	}

	if (firstStart) {
		firstStart = false;
	}
}

void checkReset() {
	if (keyReset) {		
		if (!resetFlag) {
			SendToSd("Reset");
			resetFlag = true;
			resetState = false;
			//battery2.resetSensor();
			Serial.println("reset");
			setErrorState(false);

			if (startingErrorStatus) {
				startReset = true;
				resetTimer = millis();
			}

			for (size_t i = 0; i < sizeof(digitalSensorArray) / sizeof(digitalSensorArray[i]); i++)
			{
				//sensorArray[i].resetSensor();
				digitalSensorArray[i].resetSensor();
			}
			//analog1.resetAnalogSensor();
			startingErrorStatus = false;
			/*for (size_t i = 0; i < 6; i++)
				digitalWrite(outputArray[i], LOW);*/
		}
	}
	else {
		resetFlag = false;
	}

}

void setBit(unsigned int& value, int position) {
	value |= (1 << position);
}
void unSetBit(unsigned int& value, int position) {
	value &= ~(1 << position);
}
void setPinPositionMultiplexer(int position) {
	for (size_t i = 0; i < sizeof(muxPinPosition) / sizeof(muxPinPosition[0]); i++)
	{
		bool state = false;
		if (position & (1 << i))
			state = true;
		digitalWrite(muxPinPosition[i], state);
	}

}