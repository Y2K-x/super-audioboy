#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
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

static const unsigned char PROGMEM sd_bmp[10] = {
	0xf8, 
	0xfc, 
	0xfe, 
	0x7e, 
	0x7e, 
	0xfe, 
	0xfe, 
	0xfe, 
	0xfe, 
	0xfe
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

static const unsigned char PROGMEM rst_bmp[7]= {
	0x40, 
	0xfc, 
	0x42, 
	0x02, 
	0x02, 
	0x02, 
	0x1c
};

Osd::Osd(int8_t oled_rst) :
	display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, oled_rst) 
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

void Osd::dispInfoBar() {
	display.drawBitmap(2, 3, spkr_bmp, 16, 10, SSD1306_WHITE);
	display.drawBitmap(102, 3, sd_bmp, 8, 10, SSD1306_WHITE);
	display.drawBitmap(112, 4, bat_bmp, 16, 8, SSD1306_WHITE);
	
	display.display();
}

void Osd::dispMenuBar() {
	display.drawBitmap(2, 57, menu_bmp, 8, 5, SSD1306_WHITE);
	display.drawBitmap(119, 55, rst_bmp, 8, 7, SSD1306_WHITE);
	
	display.display();
}

void Osd::dispVisualizerBox() {
	display.drawRect(36, 16, 56, 26, SSD1306_WHITE);
	
	display.setCursor(44, 48);
	display.setTextColor(SSD1306_WHITE);
	display.println(F("NO FILE"));
	
	display.display();
}

void Osd::dispTestPattern(int size) {
	display.clearDisplay();
	
	for(int y = 0; y < 64; y++) {
		for(int x = 0; x < 128; x++) {
			if(y % size == 0) {
				if(x % size == 0) {
					//place pixel
					display.drawPixel(x, y, SSD1306_WHITE);
				}
			}
			else {
				if(x % size != 0) {
					display.drawPixel(x, y, SSD1306_WHITE);
				}
			}
		}
	}
	
	display.display();
}