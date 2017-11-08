/* Circuit Playground Catch The Red
 * Tilt the Circiut Board in order to get the blue light to reach the red light
 * 
 * After uploading the code to the Circiut Board, press the left button 
 * to start the game. Play the game. The game is finished when all the lights on the board 
 * turn to red. The score is reported on the serial monitor. To play again, press the
 * left button again.
 * 
 * Author: Alisha Asnani
 */


#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>

bool leftButtonPressed;
bool showedScore;
bool hit;
int target;
int player;
int points;
unsigned long startTime;
unsigned long hitTime;


/*
 * delects the tilt and assigns corresponding pixel. Turns that pixel blue and checks 
 * if that pixel was the red light's pixel. If so, adds a point and return true. 
 * If not, repeats until time runs out and returns false.
 */
bool mov () {
  bool hitTarget = false;
  float X, Y;
 
  while (hitTarget == false and millis() < (startTime + hitTime)) {
    //determining tilt using the accelerometer.
    X = CircuitPlayground.motionX();
    Y = CircuitPlayground.motionY();

    //turning off the current blue light
    CircuitPlayground.setPixelColor(player, 0, 0, 0);


    //assigning the pixel corresponding to the tilt
    if (X < -2 and Y < 0) {
      player = 0;
    }
    else if (X < -1 and Y < 0) {
      player = 1;
    }
    else if (X < 1 and Y < -0) {
      player = 2;
    }
    else if (X < 2 and Y < 0){
      player = 3;
    }
    else if (X > 4 and Y < 0){
      player = 4;
    }
    else if (X > 4 and Y > 0){
      player = 5;
    }
    else if (X > 3 and Y > 0){
      player = 6;
    }
    else if (X > 2 and Y > 0){
      player = 7;
    }
    else if (X > -2 and Y > 0){
      player = 8;
    }
    else if (X > -3 and Y > 0){
      player = 9;
    }
  
    CircuitPlayground.setPixelColor(player, 0, 0, 90);
    
    if (player == target) { //adds a point if, the blue is on the same pixel as the red
      points = points + 1;
      hitTarget = true;
    }
  }

  return hitTarget;
  
}

void setup() {
  Serial.begin(230400);
  CircuitPlayground.begin();
  hit = false;
  showedScore = true;
  Serial.println("Click Left Button To Start");
}

void loop() {
  leftButtonPressed = CircuitPlayground.leftButton();
  
  //if left button is presed, resets all values add allows game to start
  //by setting hit to true.
  if (leftButtonPressed) { 
     for (int i=0; i<10; ++i) {
      CircuitPlayground.setPixelColor(i, 0, 0, 0);
     }
     player = 0;
     points = 0;
     hit = true;
     CircuitPlayground.setPixelColor(player, 0, 0, 90);
     hitTime = 30000; 
     showedScore = false;
   }

  //the game
  while (hit == true) {
      target = (int) random(0,10); 
      CircuitPlayground.setPixelColor(target, 90, 0, 0); //turn a random pixel to turn red
      startTime = millis();
      hit = mov(); //true is player got the red, false if game over

      if(hitTime > 1000){ // decreases time by a second
        hitTime = hitTime - 1000;
      }
      else{ // if less than a second left, decreases by 10%
        float diff = hitTime * (9.0/10.0);
        hitTime = (int) diff;
      }
   }

  //prints score and turns all lights red if the player lost the game
  if(showedScore == false) {
     Serial.print("Score: ");
     Serial.println(points);
     Serial.println("Click Left Button To Start");
     for (int i=0; i<10; ++i) {
      CircuitPlayground.setPixelColor(i, 90, 0, 0);
    }
     showedScore = true;
  }
   

}




