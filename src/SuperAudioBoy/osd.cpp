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

Osd::Osd(int8_t oled_rst) :
	display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, oled_rst),
	osdState(OSD_MAIN)
{}

int Osd::init() {
	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
		Serial.println("SSD1306 allocation failed!");
		return -1;
	}
	
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
	display.drawRect(36, 16, 56, 26, SSD1306_WHITE);
	display.setCursor(44, 48);
	display.setTextColor(SSD1306_WHITE);
	display.println(F("NO FILE"));
	
	display.display();
	
	return;
}

void Osd::updateFiles() {
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
	
	File root = SD.open("/");
	getFileList(root);
	root.close();
	
	display.display();
}

void Osd::update() {
	if(osdState == OSD_MAIN) {
		updateMain();
		dispMain();
	}
	else if (osdState == OSD_FILE) {
		display.drawBitmap(103, 4, load_bmp, 8, 7, SSD1306_WHITE);
		display.display();
		updateFiles();
		dispFiles();
	}
	
	return;
}

void Osd::getFileCount(File dir) {
	while(true) {
		File entry = dir.openNextFile();
		if(!entry) { break; }

		if(!entry.isDirectory()) { fileCount += 1; }
		
		entry.close();
	}
}

void Osd::getFileList(File dir) {
	display.setCursor(0, 16);
	while(true) {
		File entry = dir.openNextFile();
		if(!entry){ break; }
		
		if(!entry.isDirectory()) {
			display.println(entry.name());
		}
		
		entry.close();
	}
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