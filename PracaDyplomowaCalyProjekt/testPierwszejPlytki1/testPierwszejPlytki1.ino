/*
 Name:		testPierwszejPlytki1.ino
 Created:	29.12.2022 14:17:20
 Author:	Przemek Zapart
*/ 

//to bedzie czerwona plytka


int digitalIn[] = { 32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47 };
int digitalOut[] = { 8,9,10,11,12,13,22,23,24,25,26,27,28,29,30,31 };



unsigned int digitalDataIn = 0;



unsigned long timer = 0;
//digital not working WHYYYYYYY

void setup() {
	Serial.begin(19200);
	Serial.println("pierwsza plytka");
	
	for (size_t i = 0; i < sizeof(digitalIn) / sizeof(digitalIn[0]); i++)
	{
		pinMode(digitalIn[i], INPUT_PULLUP);
		

	}
	for (size_t i = 0; i < sizeof(digitalOut) / sizeof(digitalOut[0]); i++)
	{
		pinMode(digitalOut[i], OUTPUT);
		digitalWrite(digitalOut[i], LOW);
	}
	delay(2000);
}


void loop() {



	/*for (size_t i = 0; i < sizeof(digitalOut) / sizeof(digitalOut[0]); i++)
	{
		digitalWrite(digitalOut[i], HIGH);
		delay(500);

	}
	for (size_t i = 0; i < sizeof(digitalOut) / sizeof(digitalOut[0]); i++)
	{
		digitalWrite(digitalOut[i], LOW);
		delay(500);

	}*/


	static bool a = false;
	if (millis() - timer > 1000 && !a) {
		a = true;
		timer = millis();
		for (size_t i = 0; i < sizeof(digitalOut) / sizeof(digitalOut[0]); i++)
		{
			digitalWrite(digitalOut[i], LOW);

		}
	}
	if (millis() - timer > 1000 && a) {
		a = false;
		timer = millis();
		for (size_t i = 0; i < sizeof(digitalOut) / sizeof(digitalOut[0]); i++)
		{
			digitalWrite(digitalOut[i], HIGH);

		}
	}	

	for (size_t i = 0; i < sizeof(digitalIn) / sizeof(digitalIn[0]); i++)
	{
		int pos = i;
		bool state = digitalRead(digitalIn[pos]);

		if (state) {
			Serial.print(1);
			digitalDataIn |= (1 << pos);
		}

		else {
			Serial.print(0);
			digitalDataIn &= ~(1 << pos);
		}
	}
	//for (size_t i = sizeof(digitalIn) / sizeof(digitalIn[0]); i >0 ; i--)
	//{
	//	int pos = i - 1;
	//	bool state = digitalRead(digitalIn[pos]);

	//	if (state) {
	//		Serial.print(1);
	//		digitalDataIn |= (1 << pos);
	//	}
	//		
	//	else {
	//		Serial.print(0);
	//		digitalDataIn &= ~(1 << pos);
	//	}
	//	//Serial.print(digitalRead(digitalIn[i]));
	//	
	//	
	//}
	Serial.print(" = ");
	Serial.println(digitalDataIn ,HEX);

}
