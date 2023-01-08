/*
 Name:		doTestowaniaKodu.ino
 Created:	31.12.2022 09:15:35
 Author:	Przemek
*/


#include <DigiPotX9Cxxx.h>

DigiPot pot(9,10,8);
DigiPot pot1(9, 10, 11);
DigiPot potentiometr_2(6, 7, 5);
DigiPot potentiometr_3(49,50,48);
//DigiPot potentiometr_3(9, 10, 8);


//int digitalIn[] = { 8,9,10,11,12,13,22,23,24,25,26,27,28,29,30,31 };
//int digitalOut[] = { 32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47 };

int digitalOut[] = { 8,9,10,11,12,13,22,23,24,25,26,27,28,29,30,31 };
int digitalIn[] = { 32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47 };



long rpm = 0;
long rpmTimer = 0;
long interruptRpm = 0;
bool rpmState = false;
int iloscZebow = 100;

int rpmPin = A0;
int amounfOfTeeth = 100;

void setup() {
	Serial.begin(19200);
	Serial1.begin(19200);
	Serial2.begin(19200);
	Serial3.begin(19200);

	//pinMode(digitalIn[2], INPUT_PULLUP);
	//attachInterrupt(digitalPinToInterrupt(2), show, RISING);
	attachInterrupt(digitalPinToInterrupt(2), rpmCount, RISING);
	//attachInterrupt(digitalPinToInterrupt(2), rpmCount, CHANGE);
	//
	
	pinMode(rpmPin, OUTPUT);
	digitalWrite(rpmPin, HIGH);

	//for (size_t i = 0; i < sizeof(digitalOut) / sizeof(digitalOut[0]); i++)
	//{
	//	pinMode(digitalIn[i], INPUT_PULLUP);
	//	pinMode(digitalOut[i], OUTPUT);
	//	digitalWrite(digitalOut[i], LOW);
	//}
	//while (1) {};
}


void rpmCount() {
	rpm++;
	/*static long cnt = 0;
	static  long timer = 0;
	cnt++;	
	if (millis() - timer > 1000) {
		timer = millis();
		rpm = cnt;
		cnt = 0;
	}*/
}
void show() {
	Serial.println("przerwanie");
	Serial.print(digitalRead(digitalIn[2]));
}
void print(int ana) {
	int analog = analogRead(ana);
	Serial.print(ana);
	Serial.print(" - analog value : ");
	Serial.println(analog);

}
void loop() {

	Serial2.println("sjdfhkdjashfk;asdhf");
	Serial.println("Sesrial 0");
	delay(1000);

	//for (int i = 0; i < 100; i++) {
	//	//Serial.print("Increasing, i = ");
	//	//Serial.println(i, DEC);
	//	pot.increase(1);
	//	pot1.increase(1);
	//	potentiometr_2.increase(1);
	//	potentiometr_3.increase(1);
	//	//print(A0);
	//	print(A1);
	//	//print(A2);
	//	delay(200);
	//}

	//for (int i = 0; i < 100; i++) {
	//	//Serial.print("Decreasing, i = ");
	//	//Serial.println(i, DEC);
	//	pot.decrease(1);
	//	pot1.decrease(1);
	//	potentiometr_2.decrease(1);
	//	potentiometr_3.decrease(1);
	//	//print(A0);
	//	print(A1);
	//	//print(A2);

	//	delay(200);
	//}



	//if ( millis() - rpmTimer > 1000) {
	//	long temp = rpm;
	//	int r = (temp / iloscZebow) * 60;
	//	rpm = 0;
	//	rpmTimer = millis();
	//	Serial.print("przerwanie  = ");
	//	Serial.println(temp);
	//	Serial.print(" RPM = ");
	//	Serial.println(r);
	//}

	////if ( millis() - interruptRpm > 100) {
	//if ( micros() - interruptRpm > 200) {
	//	
	//	interruptRpm = micros();
	//	//interruptRpm = millis();
	//	digitalWrite(rpmPin, rpmState);
	//	//digitalWrite(digitalOut[1], rpmState);
	//	if (rpmState)
	//		rpmState = false;
	//	else
	//		rpmState = true;
	//}


	/*send();
	serialEvent2();*/

	//blink(true);
	//Serial2.println("#2023-01-02 08:20:25/FFF/0001/0001/FFFF/032/032/032/032/3b*");
	//delay(1000);
	//blink(false);
	//Serial2.println("#2023-01-02 08:20:26/FFF/0002/0002/0000/032/032/032/032/3b*");
	//delay(1000);
	//blink(true);
	////Serial2.println("#2023-01-02 08:20:27/FFF/0004/1000/5555/032/032/032/032/3b*");
	//delay(1000);
	//blink(false);
	////Serial2.println("#2023-01-02 08:20:28/FFF/0008/3000/AAAA/032/032/032/032/3b*");
	//delay(1000);
}
void blink(bool state) {
	for (size_t i = 0; i < 8; i++)
	//for (size_t i = 0; i < sizeof(digitalOut) / sizeof(digitalOut[0]); i++)
	{
		digitalWrite(digitalOut[i], state);
	}
	
	read();
}
void send() {
	static int a = 0;
	Serial.println("serial 0");
	//Serial2.println(a++);
	Serial2.println("#2023-01-02 08:20:25/FFF/0004/8001/0000/032/032/032/032/3b*");

}
void read() {
	Serial.print("digital in : ");
	for (size_t i = 0; i < sizeof(digitalIn) / sizeof(digitalIn[0]); i++)
	//for (size_t i = 0; i < 8; i++)
	{
		Serial.print(digitalRead(digitalIn[i]));
	}
	Serial.println();

	Serial.print("digital out: ");
	//for (size_t i = 0; i < 8; i++)
	for (size_t i = 0; i < sizeof(digitalOut) / sizeof(digitalOut[0]); i++)
	{
		Serial.print(digitalRead(digitalOut[i]));
	}
	Serial.println();
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
		//Serial2.println("data");
		char inChar = (char)Serial2.read();
		data += inChar;

		if (inChar == '\n') {
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