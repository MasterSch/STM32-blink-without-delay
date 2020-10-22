// ----------------------------------------------------------------------------
// LCDcharset - Display LCD character set for hd44780 hd44780_pinIO i/o class
// ----------------------------------------------------------------------------
// This sketch is a wrapper sketch for the hd44780 library example LCDcharset.
// Note:
// This is not a normal sketch and should not be used as model or example
// of hd44780 library sketches.
// This sketch is simple wrapper that declares the needed lcd object for the
// hd44780 library sketch.
// It is provided as a convenient way to run a pre-configured sketch for
// the i/o class.
// The source code for this sketch lives in hd44780 examples:
// hd44780/examples/hd44780examples/LCDcharset/LCDcharset.ino
// From IDE:
// [File]->Examples-> hd44780/hd44780examples/LCDcharset
//

#include <hd44780.h>
#include <hd44780ioClass/hd44780_pinIO.h>

// define the LCD geometry
#define LCD_COLS 16
#define LCD_ROWS 2

// declare Arduino pins used for LCD functions
// and the lcd object

// Note: this can be with or without backlight control:

// without backlight control:
// note that ESP8266 based arduinos must use the Dn defines rather than
// raw pin numbers.
#if defined (ARDUINO_ARCH_ESP8266)
const int rs=D8, en=D9, db4=D4, db5=D5, db6=D6, db7=D7; // esp8266 Lolin/Wemos D1 R1 (uno form factor)
#elif defined(ARDUINO_ARCH_ESP32)
// note: GPIO12 needs a pulldown resistor
const int rs=12, en=13, db4=17, db5=16, db6=27, db7=14; // esp32 espduino32 D1 R32 (uno form factor)
#else
const int rs=PB0, en=PB1, db4=PB10, db5=PB11, db6=PB12, db7=PB13;       // for all other devices

#endif
hd44780_pinIO lcd(rs, en, db4, db5, db6, db7);

//with backlight control:
//	backlight control requires two additional parameters
//	- an additional pin to control the backlight
//	- backlight active level which tells the library the level
//		needed to turn on the backlight.
//		note: If the backlight control pin supports PWM, dimming can be done
//			using setBacklight(dimvalue);
//
//	WARNING: some lcd keypads have a broken backlight circuit
//		If you have a lcd keypad, it is recommended that you first run the
//		LCDKeypadCheck sketch to verify that the backlight circuitry
//		is ok before enabling backlight control.
//		However, the hd44780_PinIO class will autodetect the issue and
//		work around it in s/w. If the backlight circuitry is broken,
//		dimming will not be possible even if the backlight pin supports PWM.
//
#if defined (ARDUINO_ARCH_ESP8266)
// esp8266 Lolin/Wemos D1 R1 (uno form factor)
//const int rs=D8, en=D9, db4=D4, db5=D5, db6=D6, db7=D7, bl=D10, blLevel=HIGH;
#elif defined(ARDUINO_ARCH_ESP32)
// esp32 espduino32 D1 R32 (uno form factor)
// note: GPIO12 needs a pulldown resistor
//const int rs=12, en=13, db4=17, db5=16, db6=27, db7=14, bl=5, blLevel=HIGH;
#else
//const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7, bl=10, blLevel=HIGH;
#endif
//hd44780_pinIO lcd(rs, en, db4, db5, db6, db7, bl, blLevel);

// tell the hd44780 sketch the lcd object has been declared
#define HD44780_LCDOBJECT

// include the hd44780 library LCDcharset sketch source code
// #include <LCDcharset.ino>


static const int dummyvar = 0; // dummy declaration for older broken IDEs!!!!
// vi:ts=4
// ----------------------------------------------------------------------------
// LCDcharset - simple demonstration to show character set font in LCD
// Created by Bill Perry 2018-01-06
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
// 
// sketch displays the character for all the character codes 0x00 to 0xff
// miniture digits are assigned to the custom character codepoints corresponding
// to the codepoint value. i.e. 
// codepoint 0 is assigned miniture '0' character.
// codepoint 1 is assigned miniture '1' character. etc...
//
// ----------------------------------------------------------------------------
// History
// 2018.01.06 bperrybap - Original creation
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
// ---------------------------------------------------------------------------

#ifndef HD44780_LCDOBJECT

#error "Special purpose sketch: Use i/o class example wrapper sketch instead."

/*
 * If not using a hd44780 library i/o class example wrapper sketch,
 * you must modify the sketch to include any needed header files for the
 * intended library and define the lcd object.
 *
 * Add your includes and constructor.
 * The lcd object must be named "lcd"
 * and comment out the #error message.
 */

#endif

// ============================================================================
// user configurable options below this point
// ============================================================================


/*
 * Define LCD size
 * 16x2 will work ok on larger displays but if you want the frame rate
 * for the full LCD geometry, define that here
 * Note: these can be (and are) overridden by defines in a wrapper sketch.
 */
#ifndef LCD_COLS
#define LCD_COLS 16
#endif

#ifndef LCD_ROWS
#define LCD_ROWS 2
#endif

const char* month[12] = {"Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};
const char* wheekday[7] = {"Sonntag","Montag","Dienstag", "Mittwoch","Donnerstag","Freitag","Samstag"};

// minature digits
// To be used for the Custom characters
const PROGMEM uint8_t minidigit[10][8] = {
	{0x07,0x05,0x05,0x05,0x07,0x00,0x00,0x00}, // minature 0
	{0x02,0x06,0x02,0x02,0x07,0x00,0x00,0x00}, // minature 1
	{0x03,0x05,0x02,0x04,0x07,0x00,0x00,0x00}, // minature 2
	{0x07,0x01,0x07,0x01,0x07,0x00,0x00,0x00}, // minature 3
	{0x05,0x05,0x07,0x01,0x01,0x00,0x00,0x00}, // minature 4
	{0x07,0x04,0x06,0x01,0x07,0x00,0x00,0x00}, // minature 5
	{0x07,0x04,0x07,0x05,0x07,0x00,0x00,0x00}, // minature 6
	{0x07,0x01,0x02,0x04,0x04,0x00,0x00,0x00}, // minature 7
	{0x07,0x05,0x07,0x05,0x07,0x00,0x00,0x00}, // minature 8
	{0x07,0x05,0x07,0x01,0x01,0x00,0x00,0x00}, // minature 9
};


void setup()
{
int status;

	// initialize LCD with number of columns and rows: 
	// hd44780 returns a status from begin() that can be used
	// to determine if initalization failed.
	// the actual status codes are defined in <hd44780.h>
	// See the values RV_XXXX
	//
	// looking at the return status from begin() is optional
	// it is being done here to provide feedback should there be an issue
	//
	// note:
	//	begin() will automatically turn on the backlight
	//
	status = lcd.begin(LCD_COLS, LCD_ROWS);
	if(status) // non zero status means it was unsuccesful
	{
		status = -status; // convert negative status value to positive number

		// begin() failed so blink error code using the onboard LED if possible
		hd44780::fatalError(status); // does not return
	}

	// initalization was successful, the backlight should be on now
	if(LCD_COLS > 8)
		lcd.print("LCD ");

	lcd.print("Charset");

	// Fill in the 8 custom characters with miniture digits
	// so they can be seen in the character set
	for(uint8_t i=0; i < 8; i++)
		lcd.createChar(i, minidigit[i]); // mini digit matching each codepoint

    delay(2000);
}
void loop(void)
{
static int c = 0;

	lcd.clear();
	if(LCD_COLS>15)
    	lcd.print("Codes 0");
    lcd.print("x");
	lcd.print(c, HEX);
    lcd.print("-0x"); lcd.print(c+LCD_COLS-1, HEX);

    if(LCD_ROWS > 1)
	{
    	lcd.setCursor(0, 1);
		for(int col = 0; col < LCD_COLS; col++)
			lcd.write(' ');
    	lcd.setCursor(0, 1);
	}
    else
	{
    	delay(2000);
		lcd.clear();
	}


    for(int j=0; j<LCD_COLS; j++)
	{
      	lcd.write((uint8_t)(c));
		if(++c > 255)
		{
			c = 0;
			break;
		}
    }
    delay(4000);

}
