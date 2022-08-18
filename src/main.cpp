#include <Arduino.h>

// simple turbo boost gauge for Arduino, 128x64 OLED display and a potentiometer
//
// based on the "u8glib_progress_bar" project by urish - https://wokwi.com/arduino/projects/300867986768527882
//
// created by upir
// full-length youtube tutorial: https://youtu.be/JXmw1xOlBdk

// links for tools used for this project:
// WOWKI emulator: https://wokwi.com/
// Start project from urish: https://wokwi.com/arduino/projects/300867986768527882
// Photopea (online graphics editor): https://www.photopea.com/
// Gamer font: https://www.dafont.com/gamer-2.font?text=-1+2+3+4+5+6+7+8+TURBO+turbo+boost
// Image2CPP tool: https://javl.github.io/image2cpp/
// Pressure sensor with Arduino: https://www.youtube.com/watch?v=MRNc7XUHBks

#include "U8g2lib.h"
#include <U8x8lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); // Fast I2C / TWI
// U8GLIB_SSD1306_128X64 u8g(13, 11, 8, 9, 10); // SPI connection
// for SPI connection, use this wiring:
// for SPI connection, use this wiring:
// GND > GND
// VCC > 5V
// SCL > 13
// SDA > 11
// RES > 10
// DC > 9
// CS > 8


// 'turbo_boost_preview', 128x64px
// this is a fullscreen preview from Photopea 128x64 size, not used for the final version
// note that when this image is not used, it is not placed in the memory
static const unsigned char epd_bitmap_turbo_boost_preview [] U8X8_PROGMEM = {
        0xf8, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x0e, 0x7c, 0x00, 0x00, 0x1c, 0xf8, 0x00, 0x00, 0x00,
        0xd8, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x06, 0x6c, 0x00, 0x00, 0x0c, 0xc0, 0x00, 0x00, 0x00,
        0xd8, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x06, 0x6c, 0x00, 0x00, 0x0c, 0xf8, 0x00, 0x00, 0x00,
        0xd8, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x06, 0x6c, 0x00, 0x00, 0x0c, 0x18, 0x00, 0x00, 0x00,
        0xd8, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x06, 0x6c, 0x00, 0x00, 0x0c, 0x18, 0x00, 0x00, 0x00,
        0xf8, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x0f, 0x7c, 0x00, 0x00, 0x1e, 0xf8, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x20, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02,
        0x20, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02,
        0x20, 0x00, 0x40, 0x00, 0x40, 0x00, 0x80, 0x00, 0x80, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x01, 0x55, 0x55, 0x55, 0x55,
        0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0xa9,
        0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x01, 0x55, 0x55, 0x55, 0x55,
        0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0xa9,
        0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x01, 0x55, 0x55, 0x55, 0x55,
        0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0xa9,
        0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x01, 0x55, 0x55, 0x55, 0x55,
        0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0xa9,
        0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x01, 0x55, 0x55, 0x55, 0x55,
        0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0xa9,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfc, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x20, 0x7e, 0xdb, 0xef, 0xbe, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x20, 0x18, 0xdb, 0x6d, 0xb6, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x20, 0x18, 0xdb, 0x6f, 0x36, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x20, 0x3f, 0xfc, 0x20, 0x18, 0xdb, 0xcd, 0xb6, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x20, 0x49, 0x03, 0xc0, 0x18, 0xdb, 0x6d, 0xb6, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x40, 0xc9, 0x81, 0x00, 0x18, 0xfb, 0x6f, 0xbe, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x41, 0x2a, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x41, 0x1c, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x41, 0xff, 0xc1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x41, 0x1c, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x41, 0x2a, 0x41, 0x00, 0x7d, 0xf7, 0xdf, 0x7e, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x40, 0xc9, 0x81, 0x00, 0x6d, 0xb6, 0xd8, 0x18, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x20, 0x49, 0x02, 0x00, 0x79, 0xb6, 0xdf, 0x18, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x20, 0x3e, 0x02, 0x00, 0x6d, 0xb6, 0xc3, 0x18, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x04, 0x00, 0x6d, 0xb6, 0xdb, 0x18, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x04, 0x00, 0x7d, 0xf7, 0xdf, 0x18, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'bg_bottom', 64x24px
// bottom part of the background, including the icon and text
static const unsigned char epd_bitmap_bg_bottom [] U8X8_PROGMEM = {
        0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfc, 0x20, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
        0x08, 0x00, 0x00, 0x20, 0x7e, 0xdb, 0xef, 0xbe, 0x10, 0x00, 0x00, 0x20, 0x18, 0xdb, 0x6d, 0xb6,
        0x10, 0x00, 0x00, 0x20, 0x18, 0xdb, 0x6f, 0x36, 0x20, 0x3f, 0xfc, 0x20, 0x18, 0xdb, 0xcd, 0xb6,
        0x20, 0x49, 0x03, 0xc0, 0x18, 0xdb, 0x6d, 0xb6, 0x40, 0xc9, 0x81, 0x00, 0x18, 0xfb, 0x6f, 0xbe,
        0x41, 0x2a, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x1c, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x41, 0xff, 0xc1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x1c, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x41, 0x2a, 0x41, 0x00, 0x7d, 0xf7, 0xdf, 0x7e, 0x40, 0xc9, 0x81, 0x00, 0x6d, 0xb6, 0xd8, 0x18,
        0x20, 0x49, 0x02, 0x00, 0x79, 0xb6, 0xdf, 0x18, 0x20, 0x3e, 0x02, 0x00, 0x6d, 0xb6, 0xc3, 0x18,
        0x10, 0x00, 0x04, 0x00, 0x6d, 0xb6, 0xdb, 0x18, 0x10, 0x00, 0x04, 0x00, 0x7d, 0xf7, 0xdf, 0x18,
        0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'bg_top', 128x25px
// top part of the background, including the meter background, tickmarks and numbers
static const unsigned char epd_bitmap_bg_top [] U8X8_PROGMEM = {
        0xf8, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x0e, 0x7c, 0x00, 0x00, 0x1c, 0xf8, 0x00, 0x00, 0x00,
        0xd8, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x06, 0x6c, 0x00, 0x00, 0x0c, 0xc0, 0x00, 0x00, 0x00,
        0xd8, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x06, 0x6c, 0x00, 0x00, 0x0c, 0xf8, 0x00, 0x00, 0x00,
        0xd8, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x06, 0x6c, 0x00, 0x00, 0x0c, 0x18, 0x00, 0x00, 0x00,
        0xd8, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x06, 0x6c, 0x00, 0x00, 0x0c, 0x18, 0x00, 0x00, 0x00,
        0xf8, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x0f, 0x7c, 0x00, 0x00, 0x1e, 0xf8, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x20, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02,
        0x20, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02,
        0x20, 0x00, 0x40, 0x00, 0x40, 0x00, 0x80, 0x00, 0x80, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x55, 0x55, 0x55, 0x55,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0xa9,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x55, 0x55, 0x55, 0x55,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0xa9,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x55, 0x55, 0x55, 0x55,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0xa9,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x55, 0x55, 0x55, 0x55,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0xa9,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x55, 0x55, 0x55, 0x55,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0xa9,
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe
};

// 'bg_bottom2', 64x24px
// bottom part of the background, including the icon and the label
// the turbocharger icon is empty, as the spinning animation should be playing over the icon
static const unsigned char epd_bitmap_bg_bottom2 [] U8X8_PROGMEM = {
        0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfc, 0x20, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
        0x08, 0x00, 0x00, 0x20, 0x7e, 0xdb, 0xef, 0xbe, 0x10, 0x00, 0x00, 0x20, 0x18, 0xdb, 0x6d, 0xb6,
        0x10, 0x00, 0x00, 0x20, 0x18, 0xdb, 0x6f, 0x36, 0x20, 0x3f, 0xfc, 0x20, 0x18, 0xdb, 0xcd, 0xb6,
        0x20, 0x41, 0x03, 0xc0, 0x18, 0xdb, 0x6d, 0xb6, 0x40, 0x80, 0x81, 0x00, 0x18, 0xfb, 0x6f, 0xbe,
        0x41, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x41, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x41, 0x00, 0x41, 0x00, 0x7d, 0xf7, 0xdf, 0x7e, 0x40, 0x80, 0x81, 0x00, 0x6d, 0xb6, 0xd8, 0x18,
        0x20, 0x41, 0x02, 0x00, 0x79, 0xb6, 0xdf, 0x18, 0x20, 0x3e, 0x02, 0x00, 0x6d, 0xb6, 0xc3, 0x18,
        0x10, 0x00, 0x04, 0x00, 0x6d, 0xb6, 0xdb, 0x18, 0x10, 0x00, 0x04, 0x00, 0x7d, 0xf7, 0xdf, 0x18,
        0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'anim_frame_1', 16x9px
// spinning inner part of the turbocharher icon, frame 1
static const unsigned char epd_bitmap_anim_frame_1 [] U8X8_PROGMEM = {
        0x01, 0x00, 0x09, 0x20, 0x05, 0x40, 0x03, 0x80, 0x1f, 0xf0, 0x03, 0x80, 0x05, 0x40, 0x09, 0x20,
        0x01, 0x00
};
// 'anim_frame_2', 16x9px
// spinning inner part of the turbocharher icon, frame 2
static const unsigned char epd_bitmap_anim_frame_2 [] U8X8_PROGMEM = {
        0x00, 0x80, 0x04, 0x80, 0x02, 0xa0, 0x1f, 0xc0, 0x03, 0x80, 0x07, 0xf0, 0x0a, 0x80, 0x02, 0x40,
        0x02, 0x00
};
// 'anim_frame_3', 16x9px
// spinning inner part of the turbocharher icon, frame 3
static const unsigned char epd_bitmap_anim_frame_3 [] U8X8_PROGMEM = {
        0x02, 0x00, 0x02, 0x40, 0x0a, 0x80, 0x07, 0xf0, 0x03, 0x80, 0x1f, 0xc0, 0x02, 0xa0, 0x04, 0x80,
        0x00, 0x80
};

static const unsigned char test [] U8X8_PROGMEM = {
        0xE0, 0xFF, 0xFF, 0x0F, 0x82, 0xFF, 0xFF, 0x07, 0xC1, 0xFF, 0xFF, 0x83,
        0xE0, 0xFF, 0xFF, 0xFF, 0xE4, 0xFF, 0xFF, 0x7F, 0xF2, 0xFF, 0xFF, 0xE7,
        0xC9, 0xFF, 0xFF, 0x9F, 0xFC, 0xFF, 0xFF, 0xFF, 0xE4, 0xFF, 0xFF, 0x1F,
        0x82, 0xFF, 0xFF, 0x07, 0xC9, 0xFF, 0xFF, 0x9F, 0xE0, 0xFF, 0xFF, 0xFF,
        0xE4, 0xFF, 0xFF, 0xCF, 0x9F, 0xFF, 0xFF, 0x3F, 0xC9, 0xFF, 0xFF, 0x9F,
        0xE7, 0xFF, 0xFF, 0xFF, 0xE4, 0xFF, 0xFF, 0xCF, 0x9F, 0xFF, 0xFF, 0x3F,
        0xC9, 0xFF, 0xFF, 0x9F, 0xE7, 0xFF, 0xFF, 0xFF, 0xE0, 0xFF, 0xFF, 0x0F,
        0x82, 0xFF, 0xFF, 0x07, 0xC1, 0xFF, 0xFF, 0x9F, 0xE0, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFB, 0xFF, 0xFF, 0xFF,
        0xFD, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xBF,
        0xFB, 0xFF, 0xFF, 0xFF, 0xFD, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0x7F,
        0xFF, 0xFF, 0xFF, 0xBF, 0xFB, 0xFF, 0xFD, 0xFF, 0xFD, 0xFF, 0xFE, 0xFF,
        0xFE, 0x7F, 0xFF, 0x7F, 0xFF, 0xBF, 0xFF, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x80, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
        0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
        0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
        0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
        0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0x7F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0x73, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x81, 0x7F, 0xFE, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xDE, 0xED, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0x7F, 0xBF, 0x5B, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x7F, 0xFF, 0xEF, 0x0F,
        0x82, 0x60, 0xC8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF,
        0x7F, 0x80, 0xEA, 0x4F, 0x92, 0xE4, 0x49, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0x9F, 0x3F, 0x3F, 0xDF, 0x0F, 0xF3, 0x24, 0x08, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0x9F, 0x7F, 0xDA, 0x4F,
        0xF2, 0x20, 0x49, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF,
        0xDF, 0xDE, 0xB4, 0x4F, 0xF2, 0x3C, 0x49, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xCF, 0x4F, 0x9E, 0xBD, 0x0F, 0xF2, 0x20, 0x48, 0xFE,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xEF, 0xF3, 0xA9, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF,
        0xEF, 0xED, 0xBD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xEF, 0xEF, 0xED, 0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xEF, 0xF3, 0xB9, 0x0F,
        0x82, 0x20, 0x08, 0x92, 0x20, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF,
        0x6F, 0x9E, 0xB5, 0x4F, 0x92, 0x24, 0xCF, 0x93, 0x24, 0xF9, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xDF, 0xCF, 0xDE, 0xAC, 0x4F, 0xF2, 0x24, 0x08, 0x92,
        0x3C, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0x9F, 0x7F, 0xDA, 0x0F,
        0xF2, 0xE0, 0x79, 0x92, 0x3C, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F,
        0x3F, 0x3F, 0xDF, 0xCF, 0xF3, 0x3C, 0x49, 0x92, 0x3C, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xBF, 0x7F, 0x80, 0xEA, 0xCF, 0xF3, 0x20, 0x08, 0x82,
        0x3C, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x7F, 0xFF, 0xF7, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
        0xBF, 0x5B, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xDE, 0xED, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x81, 0x3F, 0xFE, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0x73, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, };
;

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 144)
const int epd_bitmap_allArray_LEN = 3;
const unsigned char* epd_bitmap_allArray[3] = {
        epd_bitmap_anim_frame_1,
        epd_bitmap_anim_frame_2,
        epd_bitmap_anim_frame_3
};

int progress = 0;   // progress of the gauge
int frame = 0;      // frame for the inner part of the icon
int led_progress = 0;
bool led_on = false;

void setup() {
    //u8g.setFont(u8g_font_tpssb);  // no need to set the font, as we are not drawing any strings
    u8g2.begin();

    pinMode(A0, INPUT);    // set the pinmode for A0 to input to read the potentiometer value

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    u8g2.firstPage();
    do {
        u8g2.setColorIndex(0);
        u8g2.drawXBMP( 0, 0, 128, 64, test);
//        u8g2.setColorIndex(1); // white color

//        //u8g.drawBitmapP( 0, 0, 128/8, 64, epd_bitmap_turbo_boost_preview);   // fullscreen image
//        u8g2.drawXBMP( 0, 0, 128, 25, epd_bitmap_bg_top);                  // top part of bg
//        //u8g.drawBitmapP( 32, 31, 64/8, 24, epd_bitmap_bg_bottom);              // bottom part of bg
//        u8g2.drawXBMP( 32, 31, 64, 24, epd_bitmap_bg_bottom2);             // bottom part of bg, no middle part

        //u8g.setColorIndex(0);                                    // black color
        //u8g.drawBox(1, 12, 126, 12);                             // rectangle covering the fullscreen background
        u8g2.setColorIndex(1);                                      // white color
        u8g2.drawBox(2, 14, map(progress, 0, 1023, 0, 124), 10);    // actual gauge fill, remapped from 0-1023 to 0-124 (maximum rectangle size)

//        // draw the animated inside part of the icon
//        u8g2.drawXBMP(37, 39, 16, 9, epd_bitmap_allArray[frame]);
    } while ( u8g2.nextPage() );

    progress = analogRead(A0);  // read the potentiometer value, 0-1023 for 0-5V
    frame = frame+1;            // increase the frame for the inner part of the icon
    if (frame > 2) {frame = 0;} // there are total of 3 frames for the animation

    // if (progress < 100) {    // automatically increase the progress value (not driven by potentiometer)
    //   progress++;
    // } else {
    //   progress = 0;
    // }

    if(led_progress < 100) {
        led_progress++;
    } else {
        led_progress = 0;
        if(led_on) {
            digitalWrite(LED_BUILTIN, LOW);
            led_on = false;
        } else {
            digitalWrite(LED_BUILTIN, HIGH);
            led_on = true;
        }
    }
}