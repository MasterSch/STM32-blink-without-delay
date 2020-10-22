#include <Arduino.h>

//           1,44 Zoll rot 128x128
//
//     PIN 1  LED Backlight Control Pin  -->    3.3V
//     PIN 2  SCK Serial Clock           -->    PA5    SCK1
//     PIN 3  SDA Serial Data            -->    PA7    MOSI1
//     PIN 4  A0  Data/Command control   -->    PA3  Pin 3
//     PIN 5  RES Reset                  -->    PA4  Pin 4  SS1
//     PIN 6  CS  Chip Select            -->    PA2  Pin 2
//     PIN 7  GND                        -->    GND
//     PIN 8  VCC                        -->    3.3V
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

#define ITERATIONS 500000L    // number of iterations
#define REFRESH_TFT 7500      // refresh bar every 7500 iterations
#define ACTIVATED LOW 

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

void fillBar(int percent);
void printUI();
void removePressKeyText();
void drawBar();
void startCalculation();

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

int percent = 0;
int progress = 1;
const int buttonPin = PA0;     
int buttonState = 0;    


void setup() {

  pinMode(buttonPin, INPUT_PULLUP);  
  
  Serial.begin(115200);
  
 // tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.initR(INITR_GREENTAB);   // initialize a ST7735S chip, green tab
 // tft.initR(INITR_REDTAB);   // initialize a ST7735S chip, red tab




  tft.fillScreen(ST7735_BLACK);

  printUI();
 
}

void loop() {
  
  buttonState = digitalRead(buttonPin);
  if (buttonState == ACTIVATED) {   
    removePressKeyText();
    drawBar();
    startCalculation();
  }
  else {
  }

delay(10);
}

void fillBar(int percent)
{
    int counter =40;
    percent = map(percent,0,100,5,121);     
    for(counter = 40; counter<55;counter++)
    {
     tft.drawFastHLine(5, counter, percent, YELLOW );
    }
}

void printUI()
{
 tft.setCursor(10,5);
 tft.setTextColor(CYAN);
 tft.setTextSize(1);
 tft.print("PI BENCHMARK");

 tft.setCursor(10,45);
 tft.setTextColor(WHITE);
 tft.setTextSize(1);
 tft.print("PRESS KEY");
 
 tft.setCursor(5,70);
 tft.setTextColor(RED);
 tft.setTextSize(1);
 tft.print("PI:");

 tft.setCursor(5,100);
 tft.setTextColor(MAGENTA);
 tft.setTextSize(1);
 tft.print("TIME:");
}

void removePressKeyText()
{
 tft.setCursor(10,45);
 tft.setTextColor(BLACK);
 tft.setTextSize(1);
 tft.print("PRESS KEY");
}

void drawBar()
{
  tft.drawRect(5,40,120,15, YELLOW);
}

void startCalculation()
{
 unsigned long start, time;
 unsigned long niter=ITERATIONS;
 int LEDcounter = 0;
 unsigned long i;
 float x = 1.0;
 float pi=1.0;

 Serial.begin(115200);
 Serial.print("Beginning ");
 Serial.print(niter);
 Serial.println(" iterations...");
 Serial.println();

 start = millis();  
 for ( i = 2; i < niter; i++) {
   x *= -1.0;
   pi += x / (2.0f*(float)i-1.0f);   
    if (LEDcounter++ > REFRESH_TFT) {
     LEDcounter = 0;
     progress++;
     percent = progress*100/(ITERATIONS/ REFRESH_TFT);
     fillBar(percent);
    }
 }
 time = millis() - start;
 
 pi = pi * 4.0;

 Serial.print("# of trials = ");
 Serial.println(niter);
 Serial.print("Estimate of pi = ");
 String piString = String(pi,7);
 String timeString = String(time)+"ms";
 Serial.println(piString);
 
 Serial.print("Time: "); Serial.print(time); Serial.println(" ms");
 
 tft.setCursor(40,70);
 tft.setTextColor(GREEN);
 tft.setTextSize(1);
 tft.print(piString);

 tft.setCursor(40,100);
 tft.setTextColor(GREEN);
 tft.setTextSize(1);
 tft.print(timeString);
}
