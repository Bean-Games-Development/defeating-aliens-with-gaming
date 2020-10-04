  /*--------------------------------------------------------------------------------------

  dmd_test.cpp 
    Demo and example project for the Freetronics DMD, a 512 LED matrix display
    panel arranged in a 32 x 16 layout.

  Copyright (C) 2011 Marc Alexander (info <at> freetronics <dot> com)

  See http://www.freetronics.com/dmd for resources and a getting started guide.

  Note that the DMD library uses the SPI port for the fastest, low overhead writing to the
  display. Keep an eye on conflicts if there are any other devices running from the same
  SPI port, and that the chip select on those devices is correctly set to be inactive
  when the DMD is being written to.

  USAGE NOTES
  -----------

  - Place the DMD library folder into the "arduino/libraries/" folder of your Arduino installation.
  - Get the TimerOne library from here: http://code.google.com/p/arduino-timerone/downloads/list
    or download the local copy from the DMD library page (which may be older but was used for this creation)
    and place the TimerOne library folder into the "arduino/libraries/" folder of your Arduino installation.
  - Restart the IDE.
  - In the Arduino IDE, you can open File > Examples > DMD > dmd_demo, or dmd_clock_readout, and get it
    running straight away!

  * The DMD comes with a pre-made data cable and DMDCON connector board so you can plug-and-play straight
    into any regular size Arduino Board (Uno, Freetronics Eleven, EtherTen, USBDroid, etc)
    
  * Please note that the Mega boards have SPI on different pins, so this library does not currently support
    the DMDCON connector board for direct connection to Mega's, please jumper the DMDCON pins to the
    matching SPI pins on the other header on the Mega boards.

  This example code is in the public domain.
  The DMD library is open source (GPL), for more see DMD.cpp and DMD.h

  --------------------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------------------
    Includes
  --------------------------------------------------------------------------------------*/
  #include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
  #include <DMD.h>        //
  #include <TimerOne.h>   //
  #include "SystemFont5x7.h"
  #include "Arial_black_16.h"

  //Fire up the DMD library as dmd
  #define DISPLAYS_ACROSS 1
  #define DISPLAYS_DOWN 1
  DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
  int playerX = 20;
  int playerY = 10;
  int playerW = playerX-1;
  int playerH = playerY-1;
  int playerVX = 1;
  int playerVY = 0;
  int joystickX;
  int joystickY;
  int targetX = 31;
  int targetY = random(0, 16);
  int targetVX = 1;
  int targetW = targetX - 1;
  int targetH = targetY - 1;
  int score = 0;
  int speed = 7;

  /*--------------------------------------------------------------------------------------
    Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
    called at the period set in Timer1.initialize();
  --------------------------------------------------------------------------------------*/
  void ScanDMD()
  { 
    dmd.scanDisplayBySPI();
  }

  /*--------------------------------------------------------------------------------------
    setup
    Called by the Arduino architecture before the main loop begins
  --------------------------------------------------------------------------------------*/
  void setup(void)
  {

    //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
    Timer1.initialize( 5000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
    Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

    //clear/init the DMD pixels held in RAM
    dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
    Serial.begin(9600);
    Serial.println("Your score is: " + score);


  }

  /*--------------------------------------------------------------------------------------
    loop
    Arduino architecture main loop
  --------------------------------------------------------------------------------------*/
  void loop(void)
  {
    joystickX = analogRead(A0);
    joystickY = analogRead(A1);
    if (joystickX < 357 - 40){
      playerVX = -1;
    }
    if (joystickX > 357 + 40){
      playerVX = 1;
    }
    if (joystickX > 357 - 40 && joystickX < 357 + 40){
      playerVX = 0;
    }
    if (joystickY < 365 - 40){
      playerVY = -1;
    }
    if (joystickY > 365 + 40){
      playerVY = 1;
    }
    if (joystickY > 365 - 40 && joystickY < 365 + 40){
      playerVY = 0;
    }
    if (playerY >= 15){
      playerY = 15;
    }
    if (playerY <= 1){
      playerY = 1;
    }
    if (playerX >= 31){
      playerX = 31;
    }
    if (playerX <= 1){
      playerX = 1;
    }
    if (playerX + 1 >= targetX && playerX - 1 <= targetX && playerY + 1 >= targetY && playerY - 1 <= targetY){
      targetX = 31;
      targetY = random(0, 16);
      targetW = targetX - 1;
      targetH = targetY - 1;
      speed += 1;
      score += 1;
      Serial.println("Your score is: " + String(score));
    }
    if (targetX <= 1){
      playerX = 15;
      playerY = 8;
      speed = 7;
      targetX = 31;
      targetY = random(0,16);
      playerW = playerX-1;
      playerH = playerY-1;
      targetW = targetX - 1;
      targetH = targetY - 1;
      score = 0;
      Serial.println("Your suck your score is now: " + String(score)) + " LOL";
    }
    playerW = playerX-1;
    playerH = playerY-1;
    
    dmd.drawBox(  playerX,playerY, playerW, playerH, GRAPHICS_NORMAL );
    dmd.drawBox( targetX, targetY, targetW, targetH, GRAPHICS_NORMAL );
    targetX = targetX - targetVX;
    targetW = targetX - 1;
    targetH = targetY - 1;
    delay(1000/speed);
    playerX = playerX + playerVX;
    playerY = playerY + playerVY;
    dmd.clearScreen(true);
  }
