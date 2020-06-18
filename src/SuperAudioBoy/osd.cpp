#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <SD.h>
#include "osd.h"

static const unsigned char PROGMEM spkr_bmp[20] = {
	0x02, 0x00, 
	0x06, 0x10, 
	0x0e, 0x48, 
	0xfe, 0x28, 
	0xfe, 0xa8, 
	0xfe, 0xa8, 
	0xfe, 0x28, 
	0x0e, 0x48, 
	0x06, 0x10, 
	0x02, 0x00
};

static const unsigned char PROGMEM bat_bmp[16] = {
	0x3f, 0xfc, 
	0xe0, 0x04, 
	0xad, 0xb4, 
	0xad, 0xb4, 
	0xad, 0xb4, 
	0xad, 0xb4, 
	0xe0, 0x04, 
	0x3f, 0xfc
};

static const unsigned char PROGMEM menu_bmp[5] = {
	0xfe, 
	0x00, 
	0xfe,
	0x00,
	0xfe,
};

static const unsigned char PROGMEM rst_bmp[7] = {
	0x40, 
	0xfc, 
	0x42, 
	0x02, 
	0x02, 
	0x02, 
	0x1c
};

static const unsigned char PROGMEM enter_bmp[4] = {
	0x02, 
	0x42, 
	0xfc, 
	0x40
};

static const unsigned char PROGMEM nosd_bmp[36] = {
	0x3f, 0xfc, 
	0x20, 0x04, 
	0x20, 0x04, 
	0x20, 0x04, 
	0x21, 0x04, 
	0x21, 0x04, 
	0x21, 0x04, 
	0x41, 0x04, 
	0x81, 0x04, 
	0x81, 0x04, 
	0x41, 0x04, 
	0x80, 0x04, 
	0x80, 0x04, 
	0x81, 0x04, 
	0x80, 0x04, 
	0x80, 0x04, 
	0x80, 0x04, 
	0xff, 0xfc, 
};

static const unsigned char PROGMEM load_bmp[7] = {
	0xfe, 
	0x44, 
	0x28, 
	0x28, 
	0x28, 
	0x44, 
	0xfe
};

static const unsigned char PROGMEM up_bmp[6] = {
	0x20, 
	0x70, 
	0xa8, 
	0x20, 
	0x20, 
	0x20
};

static const unsigned char PROGMEM down_bmp[6] = {
	0x20, 
	0x20, 
	0x20, 
	0xa8, 
	0x70, 
	0x20
};

Osd::Osd(int8_t oled_rst) :
	display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, oled_rst),
	osdState(OSD_MAIN),
	index(0),
	page(0)
{
	resetFileList();
}

int Osd::init() {
	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
		Serial.println("SSD1306 allocation failed!");
		return -1;
	}
	
	selectedFile = (char*)malloc(13 * sizeof(char));
	
	display.cp437(true);
	
	display.clearDisplay();
	
	return 0;
}

/*
void Osd::dispFileMenuBar() {
	display.drawBitmap(2, 57, menu_bmp, 8, 5, SSD1306_WHITE);
	display.drawBitmap(119, 58, enter_bmp, 8, 4, SSD1306_WHITE);
	
	display.display();
	
	return;
}


void Osd::dispLoadIcon() {
	display.drawBitmap(103, 4, load_bmp, 8, 7, SSD1306_WHITE);
	
	display.display();
	
	return;
}
*/


void Osd::resetFileList() {
	for(int i = 0; i < 64; i++) {
		files[i] = (char*)malloc(13 * sizeof(char));
	}
}

void Osd::updateMain() {
	return;
}

void Osd::dispMain() {
	display.clearDisplay();
	
	//draw top info bar
	display.drawBitmap(2, 3, spkr_bmp, 16, 10, SSD1306_WHITE);
	display.drawBitmap(112, 4, bat_bmp, 16, 8, SSD1306_WHITE);
	
	//draw bottom menu bar
	display.drawBitmap(2, 57, menu_bmp, 8, 5, SSD1306_WHITE);
	display.drawBitmap(119, 55, rst_bmp, 8, 7, SSD1306_WHITE);
	
	//draw center audio visualizer
	display.drawRect(36, 16, 57, 26, SSD1306_WHITE);
	display.setTextColor(SSD1306_WHITE);
	
	if(strlen(selectedFile) <= 1) {
		display.setCursor(44, 48);
		display.println(F("NO FILE"));
	}
	else {
		display.setCursor(29 + ((12 - strlen(selectedFile)) * 3), 48);
		display.println(selectedFile);
	}
	
	display.display();
	
	return;
}

void Osd::updateFiles() {
	if(!SD.begin(BUILTIN_SDCARD)) {
		dispNoSD();
		
		while(true) {
			if(SD.begin(BUILTIN_SDCARD)) {
				break;
			}
		}
	}
	
	File root = SD.open("/");
	readFileCount(root);
	root.close();
	
	root = SD.open("/");
	readFileList(root, fileCount);
	root.close();
	
	pageCount = (int)ceil((double)fileCount / 4);
	
	return;
}

void Osd::dispFiles() {	
	display.clearDisplay();
	
	//draw top info bar
	display.drawBitmap(2, 3, spkr_bmp, 16, 10, SSD1306_WHITE);
	display.drawBitmap(112, 4, bat_bmp, 16, 8, SSD1306_WHITE);
	
	//draw bottom menu bar
	display.drawBitmap(2, 57, menu_bmp, 8, 5, SSD1306_WHITE);
	display.drawBitmap(119, 58, enter_bmp, 8, 4, SSD1306_WHITE);
	
	if(fileCount == 0) {
		//display.setCursor(0, 16);
		//display.println(F("No SPCs found!"));
	}
	
	display.setCursor(0, 19);
	display.setTextColor(SSD1306_WHITE);
	
	//int index = 0;
	
	for(int i = 0; i < 4; i++) {
		if(i + (page * 4) < fileCount) {
			if(i == index) {
				display.print("> ");
			}
			
			display.println(files[i + (page * 4)]);
		}
	}
	
	/*
	for(int i = 0; i < 4 && i < fileCount; i++) {
			if(i == index) {
				display.print("> ");
			}
			display.println(files[i]);
	}
	*/
	
	if(page <= 0) {
		display.drawBitmap(121, 43, down_bmp, 8, 6, SSD1306_WHITE);
	}
	else if (page >= pageCount - 1) {
		display.drawBitmap(121, 19, up_bmp, 8, 6, SSD1306_WHITE);
	}
	else {
		display.drawBitmap(121, 19, up_bmp, 8, 6, SSD1306_WHITE);
		display.drawBitmap(121, 43, down_bmp, 8, 6, SSD1306_WHITE);
	}
	
	display.display();
	
	return;
}

void Osd::dispNoSD() {
	display.clearDisplay();
	
	display.drawBitmap(56, 18, nosd_bmp, 16, 18, SSD1306_WHITE);
	display.setCursor(49, 39);
	display.setTextColor(SSD1306_WHITE);
	display.println(F("NO SD"));
	
	display.display();
	
	return;
}

void Osd::dispLoad() {
	display.drawBitmap(103, 4, load_bmp, 8, 7, SSD1306_WHITE);
	display.display();
}

void Osd::update() {
	if(osdState == OSD_MAIN) {
		updateMain();
		dispMain();
	}
	else if (osdState == OSD_FILE) {
		dispFiles();
	}
	
	return;
}

void Osd::readFileCount(File dir) {
	fileCount = 0;
	
	while(true) {
		File entry = dir.openNextFile();
		if(!entry) { break; }

		if(!entry.isDirectory()) {
			if(strstr(entry.name(), ".SPC")) {
				fileCount += 1; 
			}
		}
		
		entry.close();
	}
	
	return;
}

void Osd::readFileList(File dir, int count) {
	int i = 0;
	
	while(true) {
		File entry = dir.openNextFile();
		if(!entry){ 
			break; 
		}
		
		if(!entry.isDirectory()) {
			if(i < count || i < 50) {
				if(files[i] != NULL) {
					//Serial.println(strstr(entry.name(), ".SPC"));
					if(strstr(entry.name(), ".SPC")) {
						strncpy(files[i], entry.name(), 13);
					}
				}
				
				i++;
			}
		}
		
		entry.close();
	}
	
	return;
}

void Osd::clear() {
	display.clearDisplay();
	
	return;
}

void Osd::setState(Osd_state state) {
	osdState = state;
}

Osd::Osd_state Osd::getState() {
	return osdState;
}

void Osd::setIndex(int _index) {
	index = _index;
}

int Osd::getFileCount() {
	return fileCount;
}

int Osd::getIndex() {
	return index;
}

int Osd::getPage() {
	return page;
}

void Osd::setPage(int _page) {
	page = _page;
}

int Osd::getPageCount() {
	return pageCount;
}

void Osd::setSelectedByIndex(int _index) {
	strncpy(selectedFile, files[_index], 13);
	Serial.println(strlen(selectedFile));
	return;
}