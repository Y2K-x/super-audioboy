![Logo](https://github.com/Y2K-x/super-audioboy/raw/master/design/sabLogo.png)

# Super AudioBoy

A portable SPC player using real Super Nintendo audio hardware. Currently a work in progress.

## Hardware

This project is based around the Teensy 4.1 microcontroller. You can purchase one directly from the creators at their online storefront. (https://www.pjrc.com/store/teensy41.html)

The display being used is a 128x64 OLED display based on the SSD1306 driver chip. I purchased one from UCTRONICS on Amazon, but a similar one should suffice.
Whichever one you purchase, make sure it is an I2C module, and is a 128x64 display. SPI modules and ones with a differing resolution will not work. Feel free to fork and make changes to this repo if you would like to use SPI or a higher/lower resolution display.

## Repository Layout + Info

### Source + Building (/src)

You'll need to have a Teensyduino environment set up w/ Adafruit's GFX library. You'll also need to grab the latest version of the SSD1806 library from here. (https://github.com/adafruit/Adafruit_SSD1306)

Place the library in your Teensyduino directory under hardware/teensy/avr/libraries/Adafruit_SSD1306/

Once that's done, you should be able to load the INO in the Arduino IDE. Ensure that you select the Teensy 4.1 board type and set USB type to Serial, and then you can build.

### Schematic + Board Layout (/board)

I used KiCad for creating the main board schematic and layout. 

The Teensy symbol and footprint libraries required to view these files can be fount below.

Symbols: https://github.com/XenGi/teensy_library  
Footprints: https://github.com/XenGi/teensy.pretty

### Design Files (/design)

I've included different breadboard layouts I used as I prototype out the project, if you would like to duplicate them. I have also included the logo PDN and PNG for those who want to use them.

### Documentation (/docs)

I've included some documentation that I have aquired along the way, in case anyone wants to design something similar to what I have. 
Info on the SNES APU seems to be scattered around, so hopefully this will be the one place for everything you need.
