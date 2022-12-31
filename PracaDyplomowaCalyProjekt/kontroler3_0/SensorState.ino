
void getStnsorState() {
	//serialEvent();
	checkReset();
	for (size_t i = sizeof(digitalSensorArray) / sizeof(digitalSensorArray[0]); i > 0; i--)
	{		
		int position = i - 1;
		bool currentState = digitalSensorArray[position].getValue();
		bool previousState = digitalInputState & (1 << position);

		if (currentState != previousState) {
			if (currentState) {
				Serial.println("ustaw jakiegos bita ________________");
				if (!firstStart) {
					SendToSd(position);

				}
				
				digitalInputState |= (1 << position);
			}
			else {
				digitalInputState &= ~(1 << position);
			}


		}

	//	if (!digitalSensorArray[position].getValue())
	//	{
	//		digitalInputState &= ~(1 << position);
	//		//setBit(digitalInputState, position);
	//		//Serial.print(1);
	//	}
	//		
	//	else
	//	{
	//		//unSetBit(digitalInputState, position);
	//		digitalInputState |= (1 << position);
	//		//Serial.print(0);
	//	}
	//		
	}
	Serial.println();
	Serial.println(digitalInputState ,BIN);
	Serial.println(digitalInputState, HEX);
	for (size_t i = 0; i < sizeof(digitalOutArray) / sizeof(digitalOutArray[0]); i++)  //tutaj trzeba sobie dopracoac co i jak 
	{
		if (!digitalRead(digitalOutArray[i]))
			setBit(digitalOutputState, i);
		else
			unSetBit(digitalOutputState, i);
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
		Serial.println("reset hier !!!!!!!!!!!!!!");
		if (!resetFlag) {
			SendToSd("Reset");
			resetFlag = true;
			resetState = false;
			//battery2.resetSensor();
			Serial.println("reset");
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