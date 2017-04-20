/*
  
    LCD-pixel-rogue 0.1.0
    A minimal roguelike engine for arduino, using an LCD 16x2 as the display.
    by joey comeau
    uses LiquidCrystal library for interacting with the LCD. 
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// SETUP joystick pins
// replace all the joystick stuff with whatever
// control method you prefer to use.

const int joy1 = A1;
const int joy2 = A0;
const int joy_SW = 20;

int value1 = 0;                  // variable to read the value from the analog pin 0
int value2 = 0;

// our global x and y co-ordinates, used for locating the pixel in a display 'box'
// the 'box' is one of the 5x8 pixel character fields on the LCD
int xpos = 2;
int ypos = 1;

//   our empty horizontal line.
byte BYTE_EMPTY = B10001;

// we can create a room using 


void setup() {
  // set up the LCD's number of columns and rows:
                  lcd.begin(16, 2);  

                  pinMode(joy_SW, INPUT);

                  drawPixel(xpos,ypos,0,0);

                  lcd.setCursor(6,0);
                  lcd.print("lcd.pxl.rg");
                  lcd.setCursor(6,1);
                  lcd.print("ver 0.1.0");
                  
  
              }

void loop() {

// just a few move commands to move our hero around and back to their starting position
// demonstrating all four directional commands.
// this probably does NOT need to use string objects. But for readability here, it does.
//

readJoystick();

//move("left");
//move("up");
//move("up");

  
  } // end main loop



int readJoystick(){
  value1 = treatValue(analogRead(joy1));  
  // this small pause is needed between reading
  // analog pins, otherwise we get the same value twice
  delay(100);            
  // reads the value of the variable resistor
  value2 = treatValue(analogRead(joy2)); 
  delay(100);

  if (value1 > 54) { move("down"); }
  if (value1 < 50) { move("up"); }    
  if (value2 > 54) { move("right"); }
  if (value2 < 50) { move("left"); }      
}


int treatValue(int data) {
  return (data * 9 / 1024) + 48;
 }



// Draw our pixel hero.
//
// each panel of our 16x2 display is a separate 5x8 room 
//
// The room number ROOMNUM tells which of the 16 panels across to use.
// The floor number FLOORNUM tells which of the two rows our row is on.
//
// X and Y describe our hero's position within the specified room.
//
// THE ENTIRE ROOM, with the pixel character placed 
// is then written as a custom character to the LCD.
// 
// 


void drawPixel(int x, int y,int FLOORNUM, int ROOMNUM){

// init ROOM with 8 empty rows
// an array of bytes, each of which is populated by global variable BYTE_EMPTY
// creating a blank room character.
byte ROOM[8];

    ROOM[0] = B11111;
    ROOM[7] = B11111;
    
  for (int i=1; i <= 6; i++){  
    ROOM[i] = BYTE_EMPTY;
  }


// Place Character in correct column in PIXGUY!
// this creates an array of bytes in PIXGUY, each of which
// represents the pixel hero being at a different position
// horizontally on a row.
// All 5 posibilities are saved in the array, each indexed by
// the position they represent, so that PIXGUY can be used
// with a y value to give the right representation
// of a pixel AT that y value.
//
byte PIXGUY[5];
PIXGUY[0] = B10001;
PIXGUY[1] = B11001;
PIXGUY[2] = B10101;
PIXGUY[3] = B10011;
PIXGUY[4] = B10001;
  
  
// Place PIXGUY in correct row in ROOM! 
// X is the current row that our hero is on. So we replace the current 
// value for that row in ROOM (which should be BYTE_EMPTY) with a row
// that shows a pixel at the current Y value!
ROOM[x] = PIXGUY[y];


// Draw the character to one of the LCD's custom character slots (0).
// 
// This'll be overwritten every time. (So we can get around the LCD's
// 8 custom character limitation.) 
lcd.createChar(0, ROOM);

// Set the cursor to the correct "ROOM" and "FLOOR" on the LCD display
// then draw the custom character we just created.   
lcd.setCursor(ROOMNUM, FLOORNUM);
lcd.write(byte(0));
  
 } // end drawPixel




// MOVE command
//
// takes 4 directional inputs,
// then changes the global xpos or ypos to reflect the requested movement.
//
// then calls drawPixel to redraw the room with the hero in their new location 
//
// 

void move (String dir){
  if (dir == "left") { 
        if (ypos>1) { ypos--; } 
      }
  if (dir == "right") { 
        if (ypos<3) { ypos++; } 
      }
  if (dir == "up") { 
        if (xpos>1) { xpos--; } 
      }
  if (dir == "down") {  
        if (xpos<6) { xpos++; }  
      }
    
drawPixel(xpos,ypos,0,0);
delay(300);   // Slow down, hero.
}
