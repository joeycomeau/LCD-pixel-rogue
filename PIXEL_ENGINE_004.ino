/*
  LiquidCrystal Library - Hello World

  Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
  library works with all LCD displays that are compatible with the
  Hitachi HD44780 driver. There are many of them out there, and you
  can usually tell them by the 16-pin interface.

  This sketch prints "Hello World!" to the LCD
  and shows the time.

  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008
  by David A. Mellis
  library modified 5 Jul 2009
  by Limor Fried (http://www.ladyada.net)
  example added 9 Jul 2009
  by Tom Igoe
  modified 22 Nov 2010
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystal
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// our x and y co-ordinates, used for locating the pixel on the display. 
int xpos = 0;
int ypos = 0;

//  the five horizontal positions a pixel can occupy 
byte BYTE0 = B10000;
byte BYTE1 = B01000;
byte BYTE2 = B00100;
byte BYTE3 = B00010;
byte BYTE4 = B00001;

// an array of the pixel positions in byte form, accessable by indexing with the y value.
// so, like, if the y value is 3, we can get the byte for position 3 with
// char_line[3] or using the ypos directly like charLine[ ypos ]
//
byte charLine[] = { BYTE0, BYTE1, BYTE2, BYTE3, BYTE4, };


//   our empty horizontal line.
byte BYTE_EMPTY = B00000;

// we can create a room using 


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  
  
}

void loop() {

move("right");
move("right");
move("right");
move("left");
move("down");
move("right");
move("left");
move("down");
move("left");
move("left");
move("up");
move("up");

  
  } // loop


// Draw our pixel hero.
//
// each panel of our 16x2 display is a separate 5x8 room 
//
// The room number ROOMNUM is which of the 16 panels across to use.
// The floor number FLOORNUM is which of the two rows our row is on.
//
// X and Y describe our hero's position within the specified room.
//
// THE ENTIRE ROOM, with the pixel character placed 
// is then written as a custom character to the LCD.

void drawPixel(int x, int y,int FLOORNUM, int ROOMNUM){

// init ROOM with 8 empty rows
byte ROOM[8];
for (int i=0; i <= 7; i++){  
  ROOM[i] = BYTE_EMPTY;
}


// Place Character in correct column in PIXGUY!
byte PIXGUY[5];
PIXGUY[0] = B10000;
PIXGUY[1] = B01000;
PIXGUY[2] = B00100;
PIXGUY[3] = B00010;
PIXGUY[4] = B00001;
  
  

// Place PIXGUY in correct row in ROOM! (replacing the existing BYTE_EMPTY)
  ROOM[x] = PIXGUY[y];


// Draw the character to one of the LD's custom character slots (0).
//  
  lcd.createChar(0, ROOM);

// Initialize the lcd and give its dimensions
  lcd.begin(16, 2);

// Set the cursor to the correct "ROOM" on the LCD display to draw the custom character   
  lcd.setCursor(ROOMNUM, FLOORNUM);
  lcd.write(byte(0));
  
 }

void move (String dir){
  if (dir == "left") { ypos--; }
  if (dir == "right") { ypos++; }
  if (dir == "up") { xpos--; }
  if (dir == "down") {xpos++; }
    
drawPixel(xpos,ypos,0,0);
delay(500);
}


