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
int roompos = 0;


// we can create a room description using an int array.
// each room has an ID number which will also be used for the CustomCharID on the LCD
// limiting us to 8 rooms onscreen at a time. 

struct aRoom {
  int ID;
  int desc[40];
} aRoom;

struct aRoom ROOMS_1;
struct aRoom ROOMS_2;
    
int ROOM1[40] = {     1,1,1,1,1, 
                      1,0,0,0,1,
                      1,0,0,0,0,
                      1,0,0,0,1,
                      1,0,0,0,1,
                      1,0,0,0,1,
                      1,0,0,0,1,
                      1,1,1,1,1,
                    };                       

int ROOM2[40] = {     1,1,1,1,1, 
                      1,0,0,0,1,
                      0,0,0,0,1,
                      1,0,0,0,1,
                      1,0,0,0,1,
                      1,1,0,0,1,
                      0,1,0,0,1,
                      0,1,1,1,1,
                    }; 

void setup() {


                    
// ROOM


      ROOMS_1.ID = 0;
      for (int i=0;i<40;i++) {
        ROOMS_1.desc[i]=ROOM1[i];
      }


      ROOMS_2.ID = 1;
      for (int i=0;i<40;i++) {
        ROOMS_2.desc[i]=ROOM2[i];
      }



  
  // set up the LCD's number of columns and rows:
                  lcd.begin(16, 2);  

                  pinMode(joy_SW, INPUT);

                  drawPixel();
           
                  
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


void drawPixel(){

// init ROOM with 8 empty rows
byte ROOM_DRAW1[8];
byte ROOM_DRAW2[8];

// Go through the ROOM1 room description array, using the values for populating the actual ROOM byte with the binary
// data for the custom character.
//

for (int i=0; i<8;i++) {

ROOM_DRAW1[i] = (ROOMS_1.desc[0+(i*5)] << 4) + (ROOMS_1.desc[1+(i*5)] << 3) + (ROOMS_1.desc[2+(i*5)]<<2) + (ROOMS_1.desc[3+(i*5)]<<1)+(ROOMS_1.desc[4+(i*5)]);
ROOM_DRAW2[i] = (ROOMS_2.desc[0+(i*5)] << 4) + (ROOMS_2.desc[1+(i*5)] << 3) + (ROOMS_2.desc[2+(i*5)]<<2) + (ROOMS_2.desc[3+(i*5)]<<1)+(ROOMS_2.desc[4+(i*5)]);

}

// add our hero to the room by adding a shifted bit to the x,y position
if (roompos == 0) {
ROOM_DRAW1[xpos] += 1 << (4-ypos);
}

if (roompos == 1) {
ROOM_DRAW2[xpos] += 1 << (4-ypos);
}



// Draw the character to one of the LCD's custom character slots (0).
// 
// This'll be overwritten every time. (So we can get around the LCD's
// 8 custom character limitation.) 
lcd.createChar(ROOMS_1.ID, ROOM_DRAW1);
lcd.createChar(ROOMS_2.ID, ROOM_DRAW2);
// Set the cursor to the correct "ROOM" and "FLOOR" on the LCD display
// then draw the custom character we just created.   
lcd.setCursor(ROOMS_1.ID,0);
lcd.write(byte(ROOMS_1.ID));
lcd.setCursor(ROOMS_2.ID,0);
lcd.write(byte(ROOMS_2.ID));
  
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
          if (ypos == 0) { 
           roompos -=1;
           ypos=5;
        }
        
        if (!(collisionDetect(xpos,(ypos-1)))) { ypos--; } 
      }
  if (dir == "right") { 
        if (ypos == 4) { 
           roompos +=1;
           ypos=-1;
        }
        
        if (!(collisionDetect(xpos,(ypos+1)))) { ypos++; } 
      }
  if (dir == "up") { 
        if (!(collisionDetect((xpos-1),ypos))) { xpos--; } 
      }
  if (dir == "down") {  
        if (!(collisionDetect((xpos+1),ypos))) { xpos++; }  
      }
    
  drawPixel();
  delay(300);   // Slow down, hero.
}

// check the x,y position fed to the function against that
// coordinate in the defined ROOM, and return the result.
// 1 for a wall, 0 for empty space.
int collisionDetect(int xtest,int ytest){

    if (roompos == 0) {
      return ROOM1[int((xtest*5)+ytest)];
    }

    if (roompos == 1) {
      return ROOM2[int((xtest*5)+ytest)];
    }

}
