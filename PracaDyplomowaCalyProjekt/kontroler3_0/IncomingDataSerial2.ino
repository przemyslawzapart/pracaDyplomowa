void serialEvent2() {
	static String data = "";
	while (Serial2.available()) {
		char c = (char)Serial2.read();
		data += c;
		if (c == '*') {

			/*Serial.print("incoming data 2 : ");
			Serial.println(data);*/

			data.trim();
			const char* dataChar = data.c_str();
			checkIncomingData((char*)dataChar);
			data = "";
		}
	}
}
void checkIncomingData(char* data) {
	char c = data[0];
	//if (!checkSum(data)) {//pomysl nad tym pozniej
	//	//Serial.println("zle czek sum");
	//	return;
	//}

	if (c == '#') {
		Serial.print("SET : ");
		SetNweValue(data);
	}
	else if (c == '@') {
		Serial.print("GET : ");
		getValue(data);
	}
	else if (c == '!') {
		//Serial.println("BUTTON STATE : ");
		checkPanelKeyStatus(data);
	}
}

bool checkSum(char* data) {
	return true;
	int len = strlen(data);
	char buf[3];
	Global.getData(data, buf, len - 3, 2);
	uint8_t checkSum = strtol(buf, NULL, 16);
	if (len == checkSum)
		return true;
	else
		return false;
}
void SetNweValue(char* data) {
	char korektor[] = "#/*";
	char* schowek;
	schowek = strtok(data, korektor);
	//Serial.println(schowek);

	if (strcmp(schowek, "S") == 0) {
		schowek = strtok(NULL, korektor);
		//Serial.println(schowek);
		setSerialNumber(schowek);
		//Serial.print("new srn = ");
		char b[20];
		getSerialNumber(b);
		Serial.print(b);
	}
	else if (strcmp(schowek, "A") == 0) {
		int id = atoi(strtok(NULL, korektor));
		char* name = strtok(NULL, korektor);//name
		char* unit = strtok(NULL, korektor);//unit
		int min = atoi(strtok(NULL, korektor));
		int max = atoi(strtok(NULL, korektor));
		int range = atoi(strtok(NULL, korektor));

		analogSensorArray[id].changeValues(name, unit, min, max, range);
		analogSensorArray[id].show();
	}
	else if (strcmp(schowek, "D") == 0) {
		int id = atoi(strtok(NULL, korektor));
		char* name = strtok(NULL, korektor);//name
		int contact = atoi(strtok(NULL, korektor));

		digitalSensorArray[id].changeValue(name, contact);
		digitalSensorArray[id].show();
	}
	else if (strcmp(schowek, "T") == 0) {
		schowek = strtok(NULL, korektor);
		//Serial.print("new time : ");
		//Serial.println(schowek);
	}

}
void getValue(char* data) {
	char korektor[] = "@/*";
	char* schowek;
	schowek = strtok(data, korektor);
	//Serial.println(schowek);
	if (strcmp(schowek, "A") == 0) {
		int id = atoi(strtok(NULL, korektor));
		char buf[45];
		analogSensorArray[id].getSettingsValues(buf);
		Serial.print(buf);
		//Serial2.print(buf);
	}
	if (strcmp(schowek, "D") == 0) {
		int id = atoi(strtok(NULL, korektor));
		char buf[45];
		digitalSensorArray[id].getSettingsValues(buf);
		Serial.print(buf);
		//Serial2.print(buf);
	}
	if (strcmp(schowek, "S") == 0) {
		char buf[12];
		getSerialNumber(buf);
		char bufferToSend[20];
		strcpy(bufferToSend, "%S/");
		strcat(bufferToSend, buf);
		strcat(bufferToSend, "*");
		Serial.print(bufferToSend);
		//Serial2.print(bufferToSend);
	}
	if (strcmp(schowek, "Q") == 0) { //send all data to panel
		//Serial.println("wyslij wszystko");
		sendAllSettingsToPanel();
	}


}

void checkPanelKeyStatus(char* data) {
	char korektor[] = "!*";
	char* buffer;
	buffer = strtok(data, korektor);
	unsigned long value = strtol(buffer, NULL, 16);
	bool temp = (value & (1 << 4));
	static long timerStart = 0;
	static bool start = false;
	if (keyStart != temp && !start) {
		start = true;
		timerStart = millis();
	}
	else if (keyStart == temp && start) {
		start = false;
	}
	if (millis() - timerStart > 50 && start && (keyStart != temp)) {
		start = false;
		keyStart = value & (1 << 4);
	}

	keyStop = value & (1 << 5);
	keyReset = value & (1 << 6);

	keyHandState = value & (1 << 0);//tutaj nalezy wybrac jaka artosc gdzie;
	keyOffState = value & (1 << 1);
	keyAutoState = value & (1 << 2);

	//showKeyStatus();	//only for debug
}
void showKeyStatus() {
	Serial.print(keyAutoState);
	Serial.print(keyHandState);
	Serial.print(keyOffState);
	Serial.print(keyStart);
	Serial.print(keyStop);
	Serial.print(keyReset);
	Serial.println();
}

void sendAllSettingsToPanel() {
	//$id/name/contact*

	for (int i = 0; i < sizeof(digitalSensorArray) / sizeof(digitalSensorArray[0]); i++)
	{
		char data[100];
		digitalSensorArray[i].getSettingsValues(data);
		Serial.print(data);

		//Serial2.print(data);
	}
	//Serial.println("analog : ");
	for (size_t i = 0; i < sizeof(analogSensorArray) / sizeof(analogSensorArray[0]); i++)
	{
		char data[100];
		analogSensorArray[i].getSettingsValues(data);
		Serial.print(data);
		//Serial2.print(data);
	}
	char data[100];
	char buf[20];
	strcpy(data, "%S/");
	getSerialNumber(buf);
	strcat(data, buf);
	strcat(data, "*");
	Serial.print(data);
	//Serial2.print(data);

	strcpy(data, "%V/");
	getVersionSoftware(buf);
	strcat(data, buf);
	strcat(data, "*");
	Serial.print(data);
	//Serial2.print(data);


}
void setSerialNumber(char* serialNumber) {
	Global.saveToEprom(20, serialNumber);

}
void getSerialNumber(char* serialNumber) {
	Global.getFromEeprom(20, serialNumber);
}
void getVersionSoftware(char* versionSoftware) {
	Global.getFromEeprom(40, versionSoftware);
}