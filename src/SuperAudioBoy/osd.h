#ifndef OSD_H
#define OSD_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <SD.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

class Osd {
	public:
		Osd(int8_t oled_rst);
		
		enum Osd_state {
			OSD_MAIN,
			OSD_FILE
		};
		
		int init();
		void updateMain();
		void dispMain();
		void updateFiles();
		void dispFiles();
		void update();
		void clear();
		void getFileCount(File dir);
		void getFileList(File dir);
		
		void setState(Osd_state state);
		Osd_state getState();
	private:
		Adafruit_SSD1306 display;
		Osd_state osdState;
		char* files[13];
		int fileCount;
};

#endif