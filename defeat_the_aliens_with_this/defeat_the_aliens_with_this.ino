
  #include <SPI.h>
  #include <DMD.h>
  #include <TimerOne.h>
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
