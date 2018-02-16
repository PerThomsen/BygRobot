/* januar 2018 Erik Christensen
 * Arduino sketch-board version 1.8.5
 * 
 * Styring af trehjulet selvkørende robotbil. 
 * Undgå forhindringer og 
 * undgå at køre ud over en kant 
 * 
 * Controlleren er et Arduino Duamillenova board, med ATmega320P processor. 
 * 
 * Kørsel og styring er med to modificerede servoer til de drivende hjul 
 * Et lille coaster wheel holder balancen
 * 
 * Sensorer:
 * Ping-sensor (ultralyd) placeret centralt fortil
 * IR sensor (infrarød) skråt 45 grader nedad og 45 grader til siden 
 * placeret i hvert af de forreste hjørner.
 */

/* Program structure:
 *  GLOBAL SETTINGS
 *  SETUP
 *  LOOP (main)
 *  functions:
 *  DRIVE
 *  (PRINT) debugging
 *  PING STATE
 *  IR STATE
 *  MOTOR STATE
 *  
 */

                                           //=============== GLOBAL SETTINGS
#include <Servo.h> // include library

Servo servoL;  // initiate servo instance
Servo servoR;  // initiate servo instance  

const int servoLPin = 10; // setting pins
const int servoRPin = 11;
const int pingPin = 7;
const int irLeftPin = 0;
const int irRightPin = 5;

// globale variables:
int pingState = 0;  // 0 or 1 or 2 
int irLstate = 0;   // 1 or 0 or 2
int irRstate = 0;   // 1 or 0 or 2
int moState = 0;    // 0 to 4
int fast = 40;      // motor speed
int slow = 70;      // motor speed
int wait = 300;      // delay in ms
int leftAngle = 0;   // servo position in degrees (motor speed) 
int rightAngle = 0;  // servo position in degrees (motor speed) 

void setup() {                              //==================== SETUP
  // put your setup code here, to run once:
  Serial.begin(9600);         // activating serial communication
  servoL.attach(servoLPin);   // attaching servo to pin
  servoR.attach(servoRPin);   // attaching servo to pin 
}

void loop() {                               //==================== LOOP
  // this is the main code
  // get states:
  irLstate = getIRstate(irLeftPin);
  irRstate = getIRstate(irRightPin);
  pingState = getPingState();
  setmoState();   // select motor action:
  
  print_states();  // debugging
  
  // regulate fast and slow according to pingstate
  if (pingState == 0) {fast = 30; slow = 60; }
  if (pingState == 2) {fast = 60; slow = 75; }
  if (pingState == 1) {fast = 75; slow = 85; }
  
  // execute the action acording to moState
  switch (moState) {
    case 0:               // drive forward
      drive(fast, 180-fast);
      delay(wait / 2);
      break;
    case 1:               // turn left
      drive(slow, 180 - fast);
      delay(wait / 2);
      drive(slow, 180 - slow);
      break;
    case 2:               // turn right
      drive(fast, 180 - slow);
      delay(wait / 2);
      drive(slow, 180 - slow);
      break;
    case 3:               // spin left
      drive(180 - slow, 180 - slow);
      delay(wait);
      drive(slow, 180 - slow);
      break;
    case 4:               // spin right
      drive(slow, slow);
      delay(wait);
      drive(slow, 180 - slow);
      break;
  }

   delay(wait / 2);  // regulate loop frequence

} // end loop

// function drive is activating the servo motors ================= DRIVE
void drive(int left, int right) {
  servoL.write(left);
  servoR.write(right);
}

// function print-states                   //====================== PRINT
// to serial terminalfor debugging
void print_states() {
  Serial.print("Ping: ");
  Serial.print(pingState);
  Serial.print("\tIR Left: ");
  Serial.print(irLstate);
  Serial.print(\t"IR Right: ");
  Serial.print(irRstate);
  Serial.print("\tmoState: ");
  Serial.print(moState);
  Serial.println();
} // end function print_states

                                          //======================== PING STATE
// function getPingState måler afstand og returnerer pingState
int getPingState() {

  //local variables:
  long duration;
  int cm = 0;

  // activate a ping
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // get distance in cm
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH); // tid til ekko registreres
  cm = duration / 29 / 2;

  // evaluate to state
  pingState = 0;
  if (cm <= 10 ){ pingState = 1; }
  if (cm > 10 && cm < 25){ pingState = 2; }
  
  return pingState;
  
} // end function getPingState  

// function getIRstate aflæser IR sensor //========================IR state
int getIRstate (int sensorpin) {
  
  int irState = 0;
  
  // aflæs IR sensor
  int val = analogRead(sensorpin);
  int cm = (6762 / (val - 9)) - 4;
  
  // evaluate to state
  if (cm < 8) { irState =1; } //tæt på forhindring
  if (cm > 15) { irState =2; } //tæt på bordkant
  
  return irState;
  
} // end function getIRstate

                                                //================= MOSTATE       
void setmoState()  { 
  /* function getmoState evaluates IR and PING and sets moState
   * irStates 1: upstackle 2: over the edge
   * 
   * moState for each combinaton of irLstate and irRstate
   * 0  0   0 forward
   * 0  1   1 left
   * 0  2   1 left
   * 1  0   2 right
   * 1  1   3 spin left
   * 1  2   3 spin left
   * 2  0   2 right
   * 2  1   4 spin right
   * 2  2   3 spin left
   */

  moState = 0;
  if (irLstate == 0) {
    if (irRstate == 0) { moState = 0; } // forward
    if (irRstate == 1) { moState = 1; } // left
    if (irRstate == 2) { moState = 1; } // left
   }
    
  if (irLstate == 1) {
    if (irRstate == 0) { moState = 2; } // right
    if (irRstate == 1) { moState = 3; } // spin left
    if (irRstate == 2) { moState = 3; } // spin left
   }
    
  if (irLstate == 2) {
    if (irRstate == 0) { moState = 2; } // right
    if (irRstate == 1) { moState = 4; } // spin right
    if (irRstate == 2) { moState = 3; } // spin left
   } 

/*
  // combine with pingstate 
  // irStates 1: upstackle 2: over the edge
  if (pingState == 1) {
    if (moState != 4) { moState = 3; } // spin left
  }

  if (pingState == 2) {
    if (moState == 0) { moState = 1; } //left
  } 
*/

} // end function setmoState

  //============================= SLUT =================================
