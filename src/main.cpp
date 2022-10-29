#include <Arduino.h>
#include "U8g2lib.h"
#include <U8x8lib.h>

#include "break_pressure_graphics.h"

#include <CAN.h>

const int CS_PIN = 7;
const int IRQ_PIN = 9;
const int QUARTZ_MHZ = 16;  // Some MCP2515 boards have 8 MHz quartz.
const int SPI_MHZ = 8;

// CAN Connections:
//  MCP | BOARD
//  INT | Not used, can connect to Pin 9
//  SCK | SCK
//   SI | MO
//   SO | MI
//   CS | Pin 7
//  GND | GND
//  VCC | 3.3V



//For Memory (SRAM) optimization
//Read constructor DOCS:https://github.com/olikraus/u8g2/wiki/u8g2setupcpp#constructor-name
U8G2_SSD1306_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE,19,18); // Fast I2C / TWI
//Alternative constructors:
//U8G2_SSD1306_128X64_NONAME_F_2ND_HW_I2C
//U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, 19, 18);
//U8G2_SSD1306_128X64_NONAME_2_SW_I2C u8g2(U8G2_R0, 19, 18);

// U8GLIB_SSD1306_128X64 u8g(13, 11, 8, 9, 10); // SPI connection
// for SPI connection, use this wiring:
// GND > GND
// VCC > 5V
// SCL > 13
// SDA > 11
// RES > 10
// DC > 9
// CS > 8


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
    // OLED Display config
    //u8g.setFont(u8g_font_tpssb);  // no need to set the font, as we are not drawing any strings
    u8g2.begin();

    // CAN configuration
    Serial.begin(115200);

    uint32_t startTimeMs = millis();
    while (!Serial && millis() - startTimeMs < 1000);
    if (!Serial) {
        Serial.println("Started!");
    } else {
        // Whatever, noone's going to see anyways.
    }

    CAN.setClockFrequency(QUARTZ_MHZ * 1E6);
    CAN.setSPIFrequency(SPI_MHZ * 1E6);
    CAN.setPins(CS_PIN, IRQ_PIN);

    // Subaru BRZ uses a 500k baud rate.
    while (!CAN.begin(500000)) {
        Serial.println("Failed to connect to the CAN controller!");
        delay(1000);
    }
    Serial.println("CAN controller connected");

    // Debug diode config
    pinMode(A0, INPUT);    // set the pinmode for A0 to input to read the potentiometer value
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    u8g2.firstPage();
    do {
        u8g2.setColorIndex(0);
        u8g2.drawXBMP(0, 0, 128, 64, break_pressure_gauge_with_overlay_text_and_graphics);
//        u8g2.setColorIndex(1); // white color

//        //u8g.drawBitmapP( 0, 0, 128/8, 64, epd_bitmap_turbo_boost_preview);   // fullscreen image
//        u8g2.drawXBMP( 0, 0, 128, 25, epd_bitmap_bg_top);                  // top part of bg
//        //u8g.drawBitmapP( 32, 31, 64/8, 24, epd_bitmap_bg_bottom);              // bottom part of bg
//        u8g2.drawXBMP( 32, 31, 64, 24, epd_bitmap_bg_bottom2);             // bottom part of bg, no middle part

        //u8g.setColorIndex(0);                                    // black color
        //u8g.drawBox(1, 12, 126, 12);                             // rectangle covering the fullscreen background
        u8g2.setColorIndex(1); // white color
        u8g2.drawBox(2, 14, map(progress, 0, 1023, 0, 124), 10); // actual gauge fill, remapped from 0-1023 to 0-124 (maximum rectangle size)

//        // draw the animated inside part of the icon
//        u8g2.drawXBMP(37, 39, 16, 9, epd_bitmap_allArray[frame]);
    } while ( u8g2.nextPage() );

    progress = analogRead(A0);  // read the potentiometer value, 0-1023 for 0-5V
    frame = frame+1;            // increase the frame for the inner part of the icon
    if (frame > 2) {frame = 0;} // there are total of 3 frames for the animation

//    // LED check for board freeze
//    if(led_progress < 100) {
//        led_progress++;
//    } else {
//        led_progress = 0;
//        if(led_on) {
//            digitalWrite(LED_BUILTIN, LOW);
//            led_on = false;
//        } else {
//            digitalWrite(LED_BUILTIN, HIGH);
//            led_on = true;
//        }
//    }
}
