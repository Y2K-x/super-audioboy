#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
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


/* CONSTRUCTOR + INIT */

Osd::Osd(int8_t oled_rst) :
	display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, oled_rst),
	index(0),
	page(0),
	osdState(OSD_MAIN)
{}

int Osd::init() {
	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
		Serial.println("SSD1306 allocation failed!");
		return -1;
	}
	
	mallocFileList();
	
	selectedFile = (char*)malloc(13 * sizeof(char));
	
	display.cp437(true);
	
	display.clearDisplay();
	
	return 0;
}

/* MEMORY MANAGEMENT FUNCTIONS */

void Osd::mallocFileList() {
	for(int i = 0; i < MAX_FILES; i++) {
		files[i] = (char*)malloc(13 * sizeof(char));
	}
}

/* SD I/O FUNCTIONS */

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
			if(i < count || i < MAX_FILES) {
				if(files[i] != NULL) {
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

/* OSD UPDATE FUNCTIONS */

void Osd::update() {
	//check OSD state and draw proper screen
	if(osdState == OSD_MAIN) {
		updateMain();
		drawMain();
	}
	else if (osdState == OSD_FILE) {
		drawFileBrowser();
	}
	
	return;
}

void Osd::updateMain() {
	return;
}

void Osd::updateFiles() {
	//Check if SD card is present. Not sure if there is a better way to do this, but this works.
	if(!SD.begin(BUILTIN_SDCARD)) {
		drawNoSD();
		
		while(true) {
			if(SD.begin(BUILTIN_SDCARD)) {
				break;
			}
		}
	}
	
	//Open root and get a count of all SPC files, then close
	File root = SD.open("/");
	readFileCount(root);
	root.close();
	
	//Open root and read out all SPC files in root to the file list array, then close
	root = SD.open("/");
	readFileList(root, fileCount);
	root.close();
	
	//calculate number of pages needed to display all SPC files
	pageCount = (int)ceil((double)fileCount / 4);
	
	return;
}

/* OSD DRAW FUNCTIONS */

void Osd::drawMain() {
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
	
	//draw currently selected file name if one was selected
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

void Osd::drawFileBrowser() {	
	display.clearDisplay();
	
	//draw top info bar
	display.drawBitmap(2, 3, spkr_bmp, 16, 10, SSD1306_WHITE);
	display.drawBitmap(112, 4, bat_bmp, 16, 8, SSD1306_WHITE);
	
	//draw bottom menu bar
	display.drawBitmap(2, 57, menu_bmp, 8, 5, SSD1306_WHITE);
	display.drawBitmap(119, 58, enter_bmp, 8, 4, SSD1306_WHITE);
	
	display.setCursor(0, 19);
	display.setTextColor(SSD1306_WHITE);
	
	//draw file names to display based on the current page. Add a ">" to the line depending on what file is highlighted
	for(int i = 0; i < 4; i++) {
		if(i + (page * 4) < fileCount) {
			if(i == index) {
				display.print("> ");
			}
			
			display.println(files[i + (page * 4)]);
		}
	}
	
	//draw page up and down icons
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

void Osd::drawNoSD() {
	display.clearDisplay();
	
	display.drawBitmap(56, 18, nosd_bmp, 16, 18, SSD1306_WHITE);
	display.setCursor(49, 39);
	display.setTextColor(SSD1306_WHITE);
	display.println(F("NO SD"));
	
	display.display();
	
	return;
}

void Osd::drawLoad() {
	display.drawBitmap(103, 4, load_bmp, 8, 7, SSD1306_WHITE);
	display.display();
}

void Osd::clear() {
	display.clearDisplay();
	
	return;
}

/* PRIVATE VARIABLE GETTERS & SETTERS */

int Osd::getFileCount() {
	return fileCount;
}

int Osd::getIndex() {
	return index;
}

void Osd::setIndex(int _index) {
	index = _index;
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

Osd::Osd_state Osd::getState() {
	return osdState;
}

void Osd::setState(Osd_state state) {
	osdState = state;
}

void Osd::setSelectedByIndex(int _index) {
	strncpy(selectedFile, files[_index], 13);
	Serial.println(strlen(selectedFile));
	return;
}