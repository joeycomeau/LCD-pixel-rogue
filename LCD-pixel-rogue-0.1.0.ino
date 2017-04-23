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
int ypos = 2;


// we can create a room using an int array

int ROOM1[] = {       1,1,1,1,1, 
                      1,0,0,0,1,
                      1,0,0,0,1,
                      1,0,0,0,1,
                      1,0,0,0,1,
                      1,0,0,0,1,
                      1,0,0,0,1,
                      1,1,1,1,1
                    };
                 


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


readJoystick();


  
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
byte ROOM[8];


// Go through the ROOM1 room description array, populating the actual ROOM byte with the binary
// data for the custom character.
//
// this needs to be rewritten as a loop to save space and improve readability.

int increment = 0;
ROOM[0] = (ROOM1[0+increment] << 4) + (ROOM1[1+increment] << 3) + (ROOM1[2+increment]<<2) + (ROOM1[3+increment]<<1)+(ROOM1[4+increment]);
increment += 5;
ROOM[1] = (ROOM1[0+increment] << 4) + (ROOM1[1+increment] << 3) + (ROOM1[2+increment]<<2) + (ROOM1[3+increment]<<1)+(ROOM1[4+increment]);
increment += 5;
ROOM[2] = (ROOM1[0+increment] << 4) + (ROOM1[1+increment] << 3) + (ROOM1[2+increment]<<2) + (ROOM1[3+increment]<<1)+(ROOM1[4+increment]);
increment += 5;
ROOM[3] = (ROOM1[0+increment] << 4) + (ROOM1[1+increment] << 3) + (ROOM1[2+increment]<<2) + (ROOM1[3+increment]<<1)+(ROOM1[4+increment]);
increment += 5;
ROOM[4] = (ROOM1[0+increment] << 4) + (ROOM1[1+increment] << 3) + (ROOM1[2+increment]<<2) + (ROOM1[3+increment]<<1)+(ROOM1[4+increment]);
increment += 5;
ROOM[5] = (ROOM1[0+increment] << 4) + (ROOM1[1+increment] << 3) + (ROOM1[2+increment]<<2) + (ROOM1[3+increment]<<1)+(ROOM1[4+increment]);
increment += 5;
ROOM[6] = (ROOM1[0+increment] << 4) + (ROOM1[1+increment] << 3) + (ROOM1[2+increment]<<2) + (ROOM1[3+increment]<<1)+(ROOM1[4+increment]);
increment += 5;
ROOM[7] = (ROOM1[0+increment] << 4) + (ROOM1[1+increment] << 3) + (ROOM1[2+increment]<<2) + (ROOM1[3+increment]<<1)+(ROOM1[4+increment]);


// add our hero to the room by adding a shifted bit to the x,y position
ROOM[xpos] += 1 << (4-ypos);



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
        if (!(collisionDetect(xpos,(ypos-1)))) { ypos--; } 
      }
  if (dir == "right") { 
        if (!(collisionDetect(xpos,(ypos+1)))) { ypos++; } 
      }
  if (dir == "up") { 
        if (!(collisionDetect((xpos-1),ypos))) { xpos--; } 
      }
  if (dir == "down") {  
        if (!(collisionDetect((xpos+1),ypos))) { xpos++; }  
      }
    
  drawPixel(xpos,ypos,0,0);
  delay(300);   // Slow down, hero.
}

// check the x,y position fed to the function against that
// coordinate in the defined ROOM, and return the result.
// 1 for a wall, 0 for empty space.
int collisionDetect(int xtest,int ytest){
    return ROOM1[int((xtest*5)+ytest)];
}


