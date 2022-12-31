/*
 Name:		counter.ino
 Created:	06.11.2022 19:06:36
 Author:	Przemek
*/
volatile word steps;
unsigned long timer = 0;

void setup() {
	Serial.begin(19200);
	pinMode(12, INPUT_PULLUP);
	pinMode(2, INPUT_PULLUP);
	//attachInterrupt(digitalPinToInterrupt(2), onStep, FALLING);
attachInterrupt(digitalPinToInterrupt(2), onStep, RISING);

}

// the loop function runs over and over again until power down or reset
void loop() {


	if (millis() - timer > 1000) {
		Serial.println(steps);
		steps = 0;
		timer = millis();
	}
	
	

}
void onStep()
{
	

	steps+=10;
	
}
