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
		void resetFileList();
		void updateMain();
		void dispMain();
		void updateFiles();
		void dispFiles();
		void dispNoSD();
		void dispLoad();
		void update();
		void clear();
		void readFileCount(File dir);
		void readFileList(File dir, int fileCount);
		
		void setState(Osd_state state);
		Osd_state getState();
		int getFileCount();
		void setIndex(int _index);
		int getIndex();
		void setPage(int _page);
		int getPage();
		int getPageCount();
		void setSelectedByIndex(int _index);
	private:
		Adafruit_SSD1306 display;
		Osd_state osdState;
		char* files[64];
		int fileCount;
		int index;
		int page;
		int pageCount;
		char* selectedFile;
};

#endif