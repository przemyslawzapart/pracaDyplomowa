/*
 Name:		doTestowaniaKodu.ino
 Created:	31.12.2022 09:15:35
 Author:	Przemek
*/



void setup() {
	Serial.begin(19200);
	Serial1.begin(19200);
	Serial2.begin(19200);
	Serial3.begin(19200);


	
}

void loop() {
	send();
	serialEvent2();
	delay(1000);
}

void send() {
	Serial.println("serial 0");
	Serial1.println("serial 1");
	Serial2.println("serial 2");
	Serial3.println("serial 3");
}

void serialEvent() {
	static String data = "";
	while (Serial.available()) {
		
		char inChar = (char)Serial.read();		
		data += inChar;
		
		if (inChar == '\n') {
			Serial.print("data 0 : ");
			Serial.println(data);
			data = "";
		}
	}
}
void serialEvent1() {
	static String data = "";
	while (Serial1.available()) {

		char inChar = (char)Serial1.read();
		data += inChar;

		if (inChar == '\n') {
			Serial1.println(data);
			Serial.print("data 1 : ");
			Serial.println(data);
			data = "";
		}
	}
}
void serialEvent2() {
	static String data = "";
	while (Serial2.available()) {
		Serial2.println("data");
		char inChar = (char)Serial2.read();
		data += inChar;

		if (inChar == '\n') {
			Serial2.println(data);
			Serial.print("data 2 : ");
			Serial.println(data);
			data = "";
		}
	}
}
void serialEvent3() {
	static String data = "";
	while (Serial3.available()) {

		char inChar = (char)Serial3.read();
		data += inChar;

		if (inChar == '\n') {
			Serial3.println(data);
			Serial.print("data 3 : ");
			Serial.println(data);
			data = "";
		}
	}
}