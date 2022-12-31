void SendToSd(uint8_t position) {
	Serial.println("wysylam co do sd !!!!!!!!!!!!!!!!!!");
	char dataToSd[200];
	time.timeStamp(dataToSd);
	strcat(dataToSd, digitalSensorArray[position].getName());
	strcat(dataToSd, "/jakies/analogowe/dane/tutaj/sobie/beda");
	Serial.println(dataToSd);
}
void SendToSd(const char* data) {
	Serial.println("wysylam co do sd !!!!!!!!!!!!!!!!!!");
	char dataToSd[200];
	time.timeStamp(dataToSd);
	strcat(dataToSd, data);
	//strcat(dataToSd, ";jakies/analogowe/dane/tutaj/sobie/beda");
	Serial.println(dataToSd);
}
