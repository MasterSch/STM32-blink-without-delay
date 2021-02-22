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

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

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

<<<<<<< HEAD
byte digitSegment[] = {
  B00111111,  // 0
  B00000110,  // 1
  B01011011,  // 2
  B01001111,  // 3
  B01100110,  // 4
  B01101101,  // 5
  B01111101,  // 6
  B00000111,  // 7
  B01111111,  // 8
  B01101111,  // 9
};



void drawDots(int x, int y, int color);  
void digit(int x, int y, int number, int color);
void Segment(int x, int y, int segment, int color);
void drawH(int x, int y, int color);
void drawV(int x, int y, int color);

<<<<<<< HEAD
=======
#define ACTIVATED LOW 

void writeLines();
void drawSegmentW(int x, int y, int color);
void drawSegmentS(int x, int y, int color);
void drawDots();
void drawTest();


<<<<<<< HEAD
const int buttonPin = PA8;  
int buttonState = 0, oldbuttonState = 1;    
>>>>>>> 5a264d03fa1b14926259133cf2120b61d0d28a23
=======
int percent = 0;
int progress = 1;
const int buttonPin = PA8;     
int buttonState = 0;    
<<<<<<< HEAD
>>>>>>> f142df6dfa4d5a587340ab598932520fc9bd4e10


void setup() {
  
=======
>>>>>>> dfdceb09b2fe15a1dd2a61e086c7471255422b5f


void setup() {

  pinMode(buttonPin, INPUT_PULLUP);  
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, HIGH);  

>>>>>>> 5a264d03fa1b14926259133cf2120b61d0d28a23
  Serial.begin(115200);
  
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
 // tft.initR(INITR_GREENTAB);   // initialize a ST7735S chip, green tab
 // tft.initR(INITR_REDTAB);   // initialize a ST7735S chip, red tab

<<<<<<< HEAD



tft.fillScreen(ST7735_BLACK);

=======
  tft.fillScreen(ST7735_BLACK);
>>>>>>> 5a264d03fa1b14926259133cf2120b61d0d28a23

}

void loop() {
<<<<<<< HEAD

int x = 30, y = 0, k = 0, zahl = 0, color = MAGENTA;

drawDots( x, y, color);

while(1) {

x = 30;
y = 0;
color = GREEN;
//digit( x, y, 8, BLACK);   // clear all segments
digit( x, y, zahl % 10, color);

y = 25;
color = YELLOW;
//digit( x, y, 8, BLACK);   // clear all segments
digit( x, y, zahl / 10, color);

y = 57;
color = RED;
//digit( x, y, 8, BLACK);   // clear all segments
digit( x, y, 5, color);

y = 82;
color = BLUE;
//digit( x, y, 8, BLACK);   // clear all segments
digit( x, y, 3, color);

y = 114;
color = MAGENTA;
//digit( x, y, 8, BLACK);   // clear all segments
digit( x, y, 4, color);

y = 139;
color = CYAN;
//digit( x, y, 8, BLACK);   // clear all segments
digit( x, y, 2, color);

delay(950);
zahl ++;
if (zahl == 60) zahl = 0;
 }
}

void digit(int x, int y, int number, int color) {

if ((digitSegment[number] & 1 ) == 1) drawH( x,  y+2,  color);
  else drawH( x,  y+2,  BLACK);
if ((digitSegment[number] & 2 ) > 0) drawV( x+3, y,   color);
  else drawV( x+3, y,   BLACK);
if ((digitSegment[number] & 4 ) > 0) drawV( x+24, y,  color);
  else drawV( x+24, y,  BLACK);
if ((digitSegment[number] & 8 ) > 0) drawH( x+42, y+2,  color);
  else drawH( x+42, y+2,  BLACK);
if ((digitSegment[number] & 16) > 0) drawV( x+24, y+19,  color);
  else drawV( x+24, y+19,  BLACK);
if ((digitSegment[number] & 32) > 0) drawV( x+3,  y+19,  color);
  else drawV( x+3,  y+19,  BLACK);
if ((digitSegment[number] & 64) > 0) drawH( x+21,  y+2,  color);
  else drawH( x+21,  y+2,  BLACK);
}


void drawH(int x, int y, int color) {
int line = 0;
=======
  

while(1) {
  buttonState = digitalRead(buttonPin);
  if (buttonState != oldbuttonState) {

  if (buttonState == ACTIVATED) {   
 
  digitalWrite(PC13, LOW);   // turn the LED on (HIGH is the voltage level)
    tft.fillScreen(ST7735_BLACK);
  writeLines();
  delay(50);                       // wait for a second
  }
  else {
    
  digitalWrite(PC13, HIGH);    // turn the LED off by making the voltage LOW
  tft.fillScreen(ST7735_BLACK);
  drawTest();
  delay(50);
  }
oldbuttonState = buttonState;
  } else delay(50);
}

}


void writeLines() {

tft.drawFastVLine(20, 20, 100, YELLOW);
tft.drawFastVLine(40, 20, 100, YELLOW);
tft.drawFastVLine(41, 20, 100, YELLOW);
tft.drawFastVLine(61, 20, 100, YELLOW);
tft.drawFastVLine(62, 20, 100, YELLOW);
tft.drawFastVLine(63, 20, 100, YELLOW);
tft.drawFastVLine(83, 20, 100, YELLOW);
tft.drawFastVLine(84, 20, 100, YELLOW);
tft.drawFastVLine(85, 20, 100, YELLOW);
tft.drawFastVLine(86, 20, 100, YELLOW);
tft.drawFastVLine(106, 20, 100, YELLOW);
tft.drawFastVLine(107, 20, 100, YELLOW);
tft.drawFastVLine(108, 20, 100, YELLOW);
tft.drawFastVLine(109, 20, 100, YELLOW);
tft.drawFastVLine(110, 20, 100, YELLOW);

}

void drawTest() {
int x = 30;
int y = 2;
int color = GREEN;
drawSegmentW(x, y, color);
y = 27;
color = YELLOW;
drawSegmentW(x, y, color);
y = 59;
color = GREEN;
drawSegmentW(x, y, color);
y = 84;
color = MAGENTA;
drawSegmentW(x, y, color);
y = 116;
color = RED;
drawSegmentW(x, y, color);
y = 141;
color = WHITE;
drawSegmentW(x, y, color);

x = 51;
y = 2;
color = GREEN;
drawSegmentW(x, y, color);
y = 27;
color = YELLOW;
drawSegmentW(x, y, color);
y = 59;
color = GREEN;
drawSegmentW(x, y, color);
y = 84;
color = MAGENTA;
drawSegmentW(x, y, color);
y = 116;
color = RED;
drawSegmentW(x, y, color);
y = 141;
color = WHITE;
drawSegmentW(x, y, color);

x = 72;
y = 2;
color = GREEN;
drawSegmentW(x, y, color);
y = 27;
color = YELLOW;
drawSegmentW(x, y, color);
y = 59;
color = GREEN;
drawSegmentW(x, y, color);
y = 84;
color = MAGENTA;
drawSegmentW(x, y, color);
y = 116;
color = RED;
drawSegmentW(x, y, color);
y = 141;
color = WHITE;
drawSegmentW(x, y, color);


x = 33;
y = 0;
color = GREEN;
drawSegmentS(x, y, color);
y = 19;
color = GREEN;
drawSegmentS(x, y, color);
y = 25;
color = YELLOW;
drawSegmentS(x, y, color);
y = 44;
color = YELLOW;
drawSegmentS(x, y, color);

y = 57;
color = GREEN;
drawSegmentS(x, y, color);
y = 76;
color = GREEN;
drawSegmentS(x, y, color);
y = 82;
color = YELLOW;
drawSegmentS(x, y, color);
y = 101;
color = YELLOW;
drawSegmentS(x, y, color);
y = 114;
color = YELLOW;
drawSegmentS(x, y, color);
y = 133;
color = YELLOW;
drawSegmentS(x, y, color);
y = 139;
color = WHITE;
drawSegmentS(x, y, color);
y = 158;
color = WHITE;
drawSegmentS(x, y, color);

x = 54;
y = 0;
color = GREEN;
drawSegmentS(x, y, color);
y = 19;
color = GREEN;
drawSegmentS(x, y, color);
y = 25;
color = YELLOW;
drawSegmentS(x, y, color);
y = 44;
color = YELLOW;
drawSegmentS(x, y, color);
y = 57;
color = GREEN;
drawSegmentS(x, y, color);
y = 76;
color = GREEN;
drawSegmentS(x, y, color);
y = 82;
color = YELLOW;
drawSegmentS(x, y, color);
y = 101;
color = YELLOW;
drawSegmentS(x, y, color);
y = 114;
color = YELLOW;
drawSegmentS(x, y, color);
y = 133;
color = YELLOW;
drawSegmentS(x, y, color);
y = 139;
color = WHITE;
drawSegmentS(x, y, color);
y = 158;
color = WHITE;
drawSegmentS(x, y, color);

drawDots();

}


void drawSegmentW(int x, int y, int color) {

int line = 1;
>>>>>>> 5a264d03fa1b14926259133cf2120b61d0d28a23
do {
int count = y;
tft.drawPixel(x, count++, color);
tft.drawPixel(x, count++, color);
count++;
tft.drawPixel(x, count++, color);
tft.drawPixel(x, count++, color);
count++;
tft.drawPixel(x, count++, color);
tft.drawPixel(x, count++, color);
count++;
tft.drawPixel(x, count++, color);
tft.drawPixel(x, count++, color);
count++;
tft.drawPixel(x, count++, color);
tft.drawPixel(x, count++, color);
count++;
tft.drawPixel(x, count++, color);
tft.drawPixel(x, count++, color);
x++;
line++;
<<<<<<< HEAD
} while(line < 2);
}


void drawV(int x, int y, int color) {
int line = 0;
=======
} while(line < 3);
}


void drawSegmentS(int x, int y, int color) {

int line = 1;
>>>>>>> 5a264d03fa1b14926259133cf2120b61d0d28a23
do {
int count = x;
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
count++;
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
count++;
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
count++;
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
count++;
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
count++;
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
y++;
line++;
<<<<<<< HEAD
} while(line < 2);
}


void drawDots(int x, int y, int color) {
int y1 = y + 50;
int line = 0;
int count = 0;
do {
count = x + 13;
tft.drawPixel(count++, y1, color);
tft.drawPixel(count++, y1, color);
tft.drawPixel(count++, y1, color);
tft.drawPixel(count++, y1, color);
count = x + 26;
tft.drawPixel(count++, y1, color);
tft.drawPixel(count++, y1, color);
tft.drawPixel(count++, y1, color);
tft.drawPixel(count++, y1, color);
y1++;
line++;
} while(line < 2);
y1 = y + 107;
line = 0;
do {
count = x + 13;
tft.drawPixel(count++, y1, color);
tft.drawPixel(count++, y1, color);
tft.drawPixel(count++, y1, color);
tft.drawPixel(count++, y1, color);
count = x + 26;
tft.drawPixel(count++, y1, color);
tft.drawPixel(count++, y1, color);
tft.drawPixel(count++, y1, color);
tft.drawPixel(count++, y1, color);
y1++;
line++;
} while(line < 2);
=======
} while(line < 3);
}


void drawDots() {
  
int color = MAGENTA;
int y = 50;
int line = 1;
int count = 0;
do {
count = 42;
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
count = 55;
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
y++;
line++;
} while(line < 3);
y = 107;
line = 1;
do {
count = 42;
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
count = 55;
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
tft.drawPixel(count++, y, color);
y++;
line++;
} while(line < 3);
>>>>>>> 5a264d03fa1b14926259133cf2120b61d0d28a23
}