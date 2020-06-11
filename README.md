![Logo](https://github.com/Y2K-x/super-audioboy/raw/master/design/sabLogo.png)

# Super AudioBoy

A portable SPC player using real Super Nintendo audio hardware. Currently a work in progress.

### Hardware

This project is based around the Teensy 4.1 microcontroller. You can purchase one directly from the creators at their online storefront. (https://www.pjrc.com/store/teensy41.html)

### Building

You'll need to have a Teensyduino environment set up w/ Adafruit's GFX library. You'll also need to grab the latest version of the SSD1806 library from here. (https://github.com/adafruit/Adafruit_SSD1306)

Place the library in your Teensyduino directory under hardware/teensy/avr/libraries/Adafruit_SSD1306/

Once that's done, you should be able to load the INO in the Arduino IDE. Ensure that you select the Teensy 4.1 board type and set USB type to Serial, and then you can build.
