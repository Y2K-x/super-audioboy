#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "osd.h"

Osd *display;

void setup() {
	Serial.begin(9600);
	
	display = new Osd(-1);
	
	if (display->init() == -1) {
		Serial.println("Display Init Failed!");
		for(;;) {} //loop infinitely
	}
	
	display->dispInfoBar();
	display->dispMenuBar();
	display->dispVisualizerBox();
}

void loop() {

}