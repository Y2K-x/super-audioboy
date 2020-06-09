#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SD.h>
#include <SPI.h>
#include "osd.h"

#define MENU_PIN 32
#define UP_PIN 31
#define DOWN_PIN 30
#define SELECT_PIN 29

Osd *display;

int menu = 0;
int up = 0;
int down = 0;
int sel = 0;

enum Inputs {
	MENU,
	UP,
	DOWN,
	SELECT,
	NONE
};

Inputs lastState = NONE;

void setup() {
	Serial.begin(9600);
	
	pinMode(MENU_PIN, INPUT);
	pinMode(UP_PIN, INPUT);
	pinMode(DOWN_PIN, INPUT);
	pinMode(SELECT_PIN, INPUT);
	
	display = new Osd(-1);
	
	if (display->init() == -1) {
		Serial.println("Display Init Failed!");
		for(;;) {} //loop infinitely
	}
	
	display->clear();
	
	if (!SD.begin(BUILTIN_SDCARD)) {
		Serial.println("initialization failed!");
		
		display->dispNoSD();
		
		while(true) {
			if(SD.begin(BUILTIN_SDCARD)) {
				break;
			}
		}
	}
	
	display->update();
}

void loop() {
	if(digitalRead(MENU_PIN)) {
		if(lastState != MENU) {
			switch(display->getState()) {
				case display->OSD_MAIN:
					display->setState(display->OSD_FILE);
					display->update();
					break;
				case display->OSD_FILE:
					display->setState(display->OSD_MAIN);
					display->update();
					break;
			}
			lastState = MENU;
		}
	}
	else {
		if(lastState == MENU) {
			lastState = NONE;
		}
	}
	
	delay(10);
}