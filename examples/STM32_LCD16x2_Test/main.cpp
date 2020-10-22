/*
  file changed july 2, 2020 MasterSch
  
  LiquidCrystal Library - Autoscroll

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch demonstrates the use of the autoscroll()
 and noAutoscroll() functions to make new text scroll or not.


 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalAutoscroll

*/

#include <Arduino.h>

// include the library code:
#include <LiquidCrystal.h>
#include <stm32f1_rtc.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

//const int rs = PB0, en = PB1, d4 = PB10, d5 = PB11, d6 = PB12, d7 = PB13;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// oder so

const int pin_RS = PB0; 
const int pin_EN = PB1; 
const int pin_d4 = PB10; 
const int pin_d5 = PB11; 
const int pin_d6 = PB12; 
const int pin_d7 = PB13; 
const int pin_BL = PB14; 
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

/*
const uint8_t LCD_RS = PB0;
const uint8_t LCD_RW = PB15;
const uint8_t LCD_EN = PB1;
const uint8_t LCD_D4 = PB10;
const uint8_t LCD_D5 = PB11;
const uint8_t LCD_D6 = PB12;
const uint8_t LCD_D7 = PB13;
const uint8_t BACKLIGHT = PB14;
const uint8_t BEEP = PA1;
LCD_HD44780 lcd(LCD_RS, LCD_RW, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
*/


void setup() {
  // set up the LCD's number of columns and rows:
  //  lcd.begin(20, 4);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(4, 0);
  lcd.print("hello world!");
  lcd.setCursor(4, 1);
  lcd.print("------------");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}

