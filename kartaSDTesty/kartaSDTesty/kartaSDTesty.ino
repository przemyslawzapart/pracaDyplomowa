/*
 Name:		kartaSDTesty.ino
 Created:	16.11.2022 18:54:37
 Author:	Przemek
*/

// the setup function runs once when you press reset or power the board
#include <SD.h>
#include <SPI.h>


const int CSpin = 53;
const int detPin = 5;
const int led = 4;

unsigned long timer = 0;

void setup() {
	Serial.begin(19200);
	
	pinMode(led, OUTPUT);	
	pinMode(CSpin, OUTPUT);
	pinMode(detPin, INPUT);	
	SD.begin(CSpin);
	/*while (!SD.begin(CSpin)) {
		Serial.println("nie ma kary lub nieobecna");
		
	}*/
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (millis() - timer > 500) {
		//blink();
		checkSdAvailable();
		timer = millis();
  }
}
void blink() {
	Serial.println("blink");
	if (digitalRead(led))
		digitalWrite(led, LOW);
	else
		digitalWrite(led, HIGH);
}
void checkSdAvailable() {
	static bool sdAvailable = false;
	static bool sdRemoved = false;
	if (digitalRead(detPin)) {//no card
		sdRemoved = true;
		sdAvailable = false;		
		Serial.println("Card not available.");
		//return;
		
	}
	else if(!SD.begin(CSpin)){
		//SD.end();
	//if(File file = SD.open("test.txt")){
		//Serial.println (SD.open("test1.txt", FILE_WRITE));
		sdAvailable = false;
		Serial.println("Card demaged.");
		//digitalWrite(led, LOW);
		blink(true);
		return;
	
	//file.close();
	}
	else {
		sdAvailable = true;	
	}
	
	digitalWrite(led, sdAvailable);
	
}
void blink(bool statee) {
	//bool state = digitalRead(led);
	digitalWrite(led, !digitalRead(led));
}
