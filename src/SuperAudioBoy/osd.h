#ifndef OSD_H
#define OSD_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define MAX_FILES 64

class Osd {
	public:
		enum Osd_state {
			OSD_MAIN,
			OSD_FILE
		};
		
		Osd(int8_t oled_rst);
		int init();
		
		void mallocFileList();

		void readFileCount(File dir);
		void readFileList(File dir, int fileCount);		
		
		void update();
		void updateFiles();
		void updateMain();
		
		void clear();
		void drawFileBrowser();
		void drawLoad();
		void drawMain();
		void drawNoSD();
		
		int getFileCount();
		int getIndex();
		void setIndex(int _index);
		int getPage();
		void setPage(int _page);
		int getPageCount();
		void setSelectedByIndex(int _index);
		Osd_state getState();
		void setState(Osd_state state);
	private:
		Adafruit_SSD1306 display;
		char* files[64];
		char* selectedFile;
		int fileCount;
		int index;
		int page;
		int pageCount;
		Osd_state osdState;
};

#endif