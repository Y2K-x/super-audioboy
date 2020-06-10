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
	switch(display->getState()) {
		case display->OSD_MAIN:
			//read menu button state
			switch(digitalRead(MENU_PIN)) {
				case 1:
					if(lastState != MENU) {
						//display file menu
						display->setState(display->OSD_FILE);
						display->dispLoad();
						display->updateFiles();
						display->update();
						
						Serial.println(display->getFileCount());
						
						lastState = MENU;
					}
					
					break;
				case 0:
					if(lastState == MENU) {
						lastState = NONE;
					}
					
					break;
			}
			
			//read reset button state
			switch(digitalRead(SELECT_PIN)) {
				case 1:
					if(lastState != SELECT) {
						lastState = SELECT;
					}
					break;
				case 0:
					if(lastState == SELECT) {
						lastState = NONE;
					}
					break;
			}
			break;
		case display->OSD_FILE:
			//read menu button state
			switch(digitalRead(MENU_PIN)) {
				case 1:
					if(lastState != MENU) {
						//display main menu
						display->setState(display->OSD_MAIN);
						display->update();
						
						lastState = MENU;
					}
					break;
				case 0:
					if(lastState == MENU) {
						lastState = NONE;
					}
					break;
			}
			
			//read up button state
			switch(digitalRead(UP_PIN)) {
				case 1:
					if(lastState != UP) {
						if(display->getIndex() > 0) {
							display->setIndex(display->getIndex() - 1);
							display->update();
						}
						else if (display->getPage() > 0) {
							display->setPage(display->getPage() - 1);
							display->setIndex(3);
							display->update();
						}
						
						lastState = UP;
					}
					break;
				case 0:
					if(lastState == UP) {
						lastState = NONE;
					}
					break;
			}
			
			//read down button state
			switch(digitalRead(DOWN_PIN)) {
				case 1:
					if(lastState != DOWN) {
							if(display->getIndex() < 3) {
								if(display->getIndex() + (display->getPage() * 4) < display->getFileCount() - 1) {
									Serial.println("TRUE");
									display->setIndex(display->getIndex() + 1);
									display->update();
								}
							}
							else if(display->getPage() < display->getPageCount() - 1) {
									display->setPage(display->getPage() + 1);
									display->setIndex(0);
									display->update();
							}
						Serial.println(display->getIndex() + (display->getPage() * 4));
						
						lastState = DOWN;
					}
					break;
				case 0:
					if(lastState == DOWN) {
						lastState = NONE;
					}
					break;
			}
			
			//read select button state
			switch(digitalRead(SELECT_PIN)) {
				case 1:
					if(lastState != SELECT) {
						display->setSelectedByIndex(display->getIndex() + (display->getPage() * 4));
						display->setState(display->OSD_MAIN);
						display->update();
						
						lastState = SELECT;
					}
					
					break;
				case 0:
					if(lastState == SELECT) {
						lastState = NONE;
					}
					
					break;
			}
			
			break;
	}
	
	delay(10);
}