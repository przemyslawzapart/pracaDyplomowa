
void sendDataToPanel() {
	char dataToSend[300];
	makeStringToPanel(dataToSend);
	Serial.println(dataToSend);
	Serial2.print(dataToSend);

}
void makeStringToPanel(char* data) {
	strcpy(data, "#");
	getTime(data);
	addRpm(data);//rpm
	addIntValueToString(data, engineState);//???
	addIntValueToString(data, digitalInputState);
	addIntValueToString(data, digitalOutputState);
	makeAnalogStringToPanel(data);
	makeCheckSum(data);
}
void getTime(char* data) {
	//strcat(data, "22-05-01/12:14:33/");
	time.timeStamp(data);
}
void addRpm(char* data) {
	strcat(data, "FFF");
	strcat(data, "/");
}
void addIntValueToString(char* data, int value) {
	char buf[5];
	sprintf(buf, "%04x", value);
	strcat(data, buf);
	strcat(data, "/");
}
void makeAnalogStringToPanel(char* data) {
	for (size_t i = 0; i < sizeof(analogSensorArray) / sizeof(analogSensorArray[0]); i++)
	{
		char buffer[10];
		int value = analogSensorArray[i].getValue();
		int first = value / 10;
		int second = value - (first * 10);
		sprintf(buffer, "%03x", value);
		strcat(data, buffer);
		strcat(data, "/");
	}
}
void makeCheckSum(char* data) {
	int len = strlen(data);
	len += 3;
	char buf[3];
	sprintf(buf, "%02x", len);
	strcat(data, buf);
	strcat(data, "*");
}