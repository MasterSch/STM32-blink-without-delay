#include <Arduino.h>


//     1,8 Zoll  blau 128x160
//
//     PIN 1  GND                        -->    GND
//     PIN 2  VCC                        -->    3.3V
//     PIN 3  SCL Serial Clock           -->    PA5    SCK1
//     PIN 4  SDA Serial Data            -->    PA7    MOSI1
//     PIN 5  RES Reset                  -->    PA4  Pin 4  SS1
//     PIN 6  DC  Data/Command control   -->    PA3  Pin 3
//     PIN 7  CS  Chip Select            -->    PA2  Pin 2
//     PIN 8  BLK Backlight Control Pin  -->    3.3V
//
// Adafruit Librarys forked to
//    https://github.com/MasterSch/Adafruit-ST7735-Library
//    https://github.com/MasterSch/Adafruit-GFX-Library

//Please note that you need to delete the Adafruit ST7735 library for Arduino if you have it installed, else the
//sketch won't compile.


#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_CS        2 // Hallowing display control pins: chip select
#define TFT_RST       4 // Display reset
#define TFT_DC        3 // Display data/command select
//#define TFT_BACKLIGHT 7 // Display backlight pin

// OPTION 1 (recommended) is to use the HARDWARE SPI pins, which are unique
// to each board and not reassignable.

// Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// OPTION 2 lets you interface the display using ANY TWO or THREE PINS,
// tradeoff being that performance is not as fast as hardware SPI above.
//#define TFT_MOSI 29  // Data out
//#define TFT_SCLK 30  // Clock out
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

 

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF
/*
#define ITERATIONS 500000L    // number of iterations
#define REFRESH_TFT 7500      // refresh bar every 7500 iterations
#define ACTIVATED LOW 

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif
*/

void testlines(uint16_t color);
void testdrawtext(char *text, uint16_t color);
void testfastlines(uint16_t color1, uint16_t color2);
void testdrawrects(uint16_t color);
void testfillrects(uint16_t color1, uint16_t color2);
void testfillcircles(uint8_t radius, uint16_t color);
void testdrawcircles(uint8_t radius, uint16_t color);
void testtriangles();
void testroundrects();
void tftPrintTest();
void mediabuttons();


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

float p = 3.1415926;

void setup(void) {
  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test"));

  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab

  // OR use this initializer (uncomment) if using a 1.44" TFT:
  //tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab

  // OR use this initializer (uncomment) if using a 0.96" 160x80 TFT:
  //tft.initR(INITR_MINI160x80);  // Init ST7735S mini display

  // OR use this initializer (uncomment) if using a 1.3" or 1.54" 240x240 TFT:
  //tft.init(240, 240);           // Init ST7789 240x240

  // OR use this initializer (uncomment) if using a 2.0" 320x240 TFT:
  //tft.init(240, 320);           // Init ST7789 320x240

  // OR use this initializer (uncomment) if using a 1.14" 240x135 TFT:
  //tft.init(135, 240);           // Init ST7789 240x135

  Serial.println(F("Initialized"));
};

void loop() {

  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);

  // large block of text
  tft.fillScreen(ST7735_BLACK);
  char Testtext[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ";
  testdrawtext(Testtext, ST7735_WHITE);
  delay(1000);

  // tft print function!
  tftPrintTest();
  delay(1000);

  // a single pixel
  tft.drawPixel(tft.width()/2, tft.height()/2, ST7735_YELLOW);
  delay(1000);

  // line draw test
  testlines(ST7735_YELLOW);
  delay(1000);

  // optimized lines
  testfastlines(ST7735_RED, ST7735_BLUE);
  delay(1000);

  testdrawrects(ST7735_GREEN);
  delay(1000);

  testfillrects(ST7735_YELLOW, ST7735_MAGENTA);
  delay(1000);

  tft.fillScreen(ST7735_BLACK);
  testfillcircles(10, ST7735_BLUE);
  testdrawcircles(10, ST7735_WHITE);
  delay(1000);

  testroundrects();
  delay(1000);

  testtriangles();
  delay(1000);

  mediabuttons();
  delay(1000);

  Serial.println("done");

  int count = 0;
  do {
  tft.invertDisplay(true);
  delay(500);
  tft.invertDisplay(false);
  delay(500);
  } while (++count < 10);
}

void testlines(uint16_t color) {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, 0, x, tft.height()-1, color);
    delay(15);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, 0, tft.width()-1, y, color);
    delay(15);
  }

  tft.fillScreen(ST7735_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
    delay(15);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, 0, 0, y, color);
    delay(15);
  }

  tft.fillScreen(ST7735_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, tft.height()-1, x, 0, color);
    delay(15);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
    delay(15);
  }

  tft.fillScreen(ST7735_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
    delay(15);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
    delay(15);
  }
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t y=0; y < tft.height(); y+=5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
    delay(15);
  }
  for (int16_t x=0; x < tft.width(); x+=5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
     delay(15);
  }
}

void testdrawrects(uint16_t color) {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
   delay(15);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t x=tft.width()-1; x > 6; x-=6) {
    tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
    delay(15);
 }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=radius; x < tft.width(); x+=radius*2) {
    for (int16_t y=radius; y < tft.height(); y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    delay(15);
   }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < tft.width()+radius; x+=radius*2) {
    for (int16_t y=0; y < tft.height()+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
   delay(15);
    }
  }
}

void testtriangles() {
  tft.fillScreen(ST7735_BLACK);
  uint16_t color = 0xF800;
  int t;
  int w = tft.width()/2;
  int x = tft.height()-1;
  int y = 0;
  int z = tft.width();
  for(t = 0 ; t <= 15; t++) {
    tft.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
   delay(15);
  }
}

void testroundrects() {
  tft.fillScreen(ST7735_BLACK);
  uint16_t color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int w = tft.width()-2;
    int h = tft.height()-2;
    for(i = 0 ; i <= 16; i+=1) {
      tft.drawRoundRect(x, y, w, h, 5, color);
      x+=2;
      y+=3;
      w-=4;
      h-=6;
      color+=1100;
    delay(15);
   }
    color+=100;
  }
}

void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.setTextColor(ST7735_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
  delay(3000);
  tft.setCursor(0, 0);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.println("------------");
  tft.println();
  tft.setTextSize(1);
  tft.setTextColor(ST7735_GREEN);
  tft.println("Want pi?");
  tft.println(" ");
  tft.println(p, 7);
  tft.println(" ");
  tft.print("Print HEX! ");
  tft.println(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println();
  tft.println();
  tft.println();
  tft.println();
  tft.setTextColor(ST7735_MAGENTA);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.println(" ");
  tft.setTextColor(ST7735_YELLOW);
  tft.print(millis());
  tft.setTextColor(ST7735_BLUE);
  tft.print(" millis.");
}

void mediabuttons() {
  // play
  tft.fillScreen(ST7735_BLACK);
  tft.fillRoundRect(25, 10, 78, 60, 8, ST7735_WHITE);
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST7735_RED);
  delay(15);
  // pause
  tft.fillRoundRect(25, 90, 78, 60, 8, ST7735_WHITE);
  tft.fillRoundRect(39, 98, 20, 45, 5, ST7735_GREEN);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST7735_GREEN);
  delay(15);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST7735_BLUE);
  delay(15);
  // pause color
  tft.fillRoundRect(39, 98, 20, 45, 5, ST7735_RED);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST7735_RED);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST7735_GREEN);
}


