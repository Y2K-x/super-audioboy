#ifndef OSD_H
#define OSD_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

class Osd {
	public:
		Osd(int8_t oled_rst);
		int init();
		void dispInfoBar();
		void dispMenuBar();
		void dispVisualizerBox();
		void dispTestPattern(int size);
	private:
		Adafruit_SSD1306 display;
};

#endif