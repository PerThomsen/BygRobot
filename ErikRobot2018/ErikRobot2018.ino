/*
 *   Trehjulet robot fra forårskursus 2018
 *   rettet 30 marts 2018
 *   sidst rettet 24 april 2018
 *   created marts 2018
 *   by Erik Christesen
 *   
 *   Bestykning:
 *   Arduino UNO controller
 *   To DC-motorer styret af et 
 *   Adafruit piggy back shield
 *   
 *   tre ping sensorer i front
 *   Den midterste regulerer hastighede i tre trin
 *   De to i siderne observerer forhindringer
 *   
 *   Struktur:
 *   
 *   includes
 *   create objects
 *   constants
 *   global variabels
 *   setup
 *   
 *   loop
 *   
 *   readPingC
 *   setSpeedC
 *   readPings
 *   drive
 *   reverse
 *   turnLeft
 *   turnRight
 *   stopM
 *   reduceSpeed
 *   increaseSpeed
 *   panic
 *   
 */
#include <Servo.h> 
#include <AFMotor.h>

AF_DCMotor motorR(4);
AF_DCMotor motorL(3);
// Servo servo01;    // create servo object to control a servo 

//============================= CONSTANTS ===  
const int echoLpin = 15;         // ping echo pin Left
const int trigLpin = 14;         // ping trigger pin Left
const int echoCpin = 17;         // ping echo pin Center
const int trigCpin = 16;         // ping trigger pin Center
const int echoRpin = 19;         // ping echo pin Right
const int trigRpin = 18;         // ping trigger pin Right

const int bumpRpin = 10;         // Bumper pin Right
const int bumpLpin = 9;          // Bumper pin Left

const int ledRpin = 3;          // Led pin Right (green
const int ledLpin = 13;           // Led pin Left  (red)

const int tonePin = 11;          // Speaker pin

const int minPing = 20;          // Min threshold for ping in cm
const int maxPing = 55;          // Max threshold for ping in cm

const int minSpeed = 75;
const int medSpeed = 90;
const int maxSpeed = 150;

const int wait = 50;

                              // =============================== Global variables ====
int timeout = 5000;           // timeout for ping ca. 85 cm
int speedR = medSpeed;        // basis-speed sættes altid på højre motor
int speedL = speedR;          // Venstre motor
int biasF = 110;               // difference-faktor mellem R og L forlæns 
int biasB = 110;              // difference-faktor mellem R og L baglæns 
boolean debug = 1;            // true: testværdier sendes til seriel terminal 
boolean lastTurn = 0;

// ==================================== SETUP ==============================
void setup() {                
  // initialize serial communications at 9600 bps:
  if(debug)  Serial.begin(9600); 
  // turn on motor
  motorR.setSpeed(speedR);
  motorR.run(RELEASE);
  motorL.setSpeed(speedL);
  motorL.run(RELEASE);
  // set Ping I/O
  pinMode(trigLpin, OUTPUT);
  pinMode(echoLpin, INPUT);
  pinMode(trigCpin, OUTPUT);
  pinMode(echoCpin, INPUT);
  pinMode(trigRpin, OUTPUT);
  pinMode(echoRpin, INPUT);
  pinMode(ledRpin, OUTPUT);
  pinMode(ledLpin, OUTPUT);
  pinMode(bumpRpin, INPUT);
  pinMode(bumpLpin, INPUT);
  digitalWrite(bumpRpin, HIGH);
  digitalWrite(bumpLpin, HIGH);
} // end setup

//====================================== LOOP ===============================
void loop() {

//  ================== test pings =============
  
  digitalWrite(ledLpin, HIGH);
  digitalWrite(ledRpin, HIGH);
  int cmL = readPing(trigLpin, echoLpin);
  int cmC = readPing(trigCpin, echoCpin);
  int cmR = readPing(trigRpin, echoRpin);
  if(cmL > 0 && cmL <= maxPing)  digitalWrite(ledLpin, LOW);
  if(cmR > 0 && cmR <= maxPing)  digitalWrite(ledRpin, LOW);
  
//  ======================== end test ping
  
/*  if(debug) {
  } // end debug
  
  int state = setState();
  switch (state) {
    case 0:
      speedR = setSpeedR(speedR);                        // regulerer speedR efter ping C
      speedL = (speedR * biasF) / 100;
      drive(speedR, speedL);                             // drive forward
      break;
    case 1:
      turnRight();
      break;
    case 2:
      turnLeft();
      break;
    case 3:
      panic();
      break;
  } // end switch
  delay(wait);
  
*/
  if(debug) {
  //  Serial.println("Loop ");
  //  Serial.print("State: ");
  //  Serial.print(state);
    Serial.print(" cmL: ");
    Serial.print(cmL);
    Serial.print(" cmC: ");
    Serial.print(cmC);
    Serial.print(" cmR: ");
    Serial.println(cmR);
    delay(500);
  } // end debug
  
}   // end loop

// ======================================= setState ========================
int setState() {
  int state = 0;

  boolean right = digitalRead(bumpRpin);
  if (!right) state = 2;                  //bumpPin low = bump
  digitalWrite(ledRpin, right);

  boolean left = digitalRead(bumpLpin);
  digitalWrite(ledLpin, left);
  if (!left) state = ++state;

  if (state > 0) {
    tone(tonePin, 100, 200);
    return state;
  }
 
  int cmR = readPing(trigRpin, echoRpin);
  if(cmR > 0) {
    if (cmR < maxPing) {
      speedR = decreaseSpeed(speedR);
      speedL = speedR * biasF;
      }  //reduce speedR
    if (cmR < minPing) state = 2;  // turn left
  } // end if cmR

  int cmL = readPing(trigLpin, echoLpin);
  if(cmL > 0) {
    if(cmL < maxPing) {               //reduce speed
      speedR = decreaseSpeed(speedR);
      speedL = speedR * biasF;
    } // end if
    if(cmL < minPing) state = state+1;
  } // end if cmL

  return state;
  
} // end setState

//======================================= setSpeedR =========================
int setSpeedR(int speedIn) {
  int speedOut = speedIn;
  int cmC = readPing(trigCpin, echoCpin); // læser pingC i cm
  if (!(cmC > 0 && cmC < maxPing)) {      // 0 eller større end max
    speedOut = increaseSpeed(speedIn);    // increase speedOut
  } else {
    if (cmC < minPing) {               // pingC nærmere end minPing
      panic();                         // STOP og BAK
    } else { 
      speedOut = decreaseSpeed(speedIn); // pingC er mellem minPing og maxPing
    }  // end if
  }
  if(debug) {
    Serial.print(" Ping cmC: ");
    Serial.print(cmC);
    Serial.print(" ");
  } // end debug
  return speedOut;
} // setSpeedR

//============================================= readPing =========================== 
int readPing(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int duration = pulseIn(echoPin, HIGH, timeout); // if > timeout then = 0
  int cm = (duration / 58);
  return cm;
} // end readPing  

//========================================= drive ===========================
void drive(int right, int left) {
  motorR.run(FORWARD);
  motorL.run(FORWARD);
  motorR.setSpeed(right);  
  motorL.setSpeed(left);  
  delay(wait*4);
  if(debug) {
    Serial.print("Forward speed R: ");
    Serial.print(right);
    Serial.print(" L: ");
    Serial.println(left);
  }
} // end drive

// ======================================= turnRight =========================
void turnRight(){
  lastTurn = true;
  motorR.run(FORWARD);
  motorL.run(FORWARD);
  motorR.setSpeed(0);  
  motorL.setSpeed(minSpeed);  
  if(debug) {
    Serial.print("Right turn speed R: ");
    Serial.print("0");
    Serial.print(" L: ");
    Serial.println(minSpeed);
  }
  delay(wait*4);
} // end turnRight

// ======================================= spinRight =========================
void spinRight(){
  lastTurn = true;
  motorR.run(BACKWARD);
  motorL.run(FORWARD);
  motorR.setSpeed(minSpeed);  
  motorL.setSpeed(minSpeed);  
  if(debug) {
    Serial.print("Right spin speed R: ");
    Serial.print("0");
    Serial.print(" L: ");
    Serial.println(minSpeed);
  }
  delay(wait*4);
} // end turnRight

// ======================================= turnLeft ===========================
void turnLeft() {
  lastTurn = false;
  motorR.run(FORWARD);
  motorL.run(FORWARD);
  motorR.setSpeed(minSpeed);  
  motorL.setSpeed(0);  
  if(debug) {
    Serial.print("Left turn speed R: ");
    Serial.print(minSpeed);
    Serial.print(" L: ");
    Serial.println(minSpeed);
  }
  delay(wait*4);
}

// ======================================= spinLeft ===========================
void spinLeft() {
  lastTurn = false;                     // false: left
  motorR.run(FORWARD);
  motorL.run(BACKWARD);
  motorR.setSpeed(minSpeed);  
  motorL.setSpeed(minSpeed);  
  if(debug) {
    Serial.print("Left spin speed R: ");
    Serial.print(minSpeed);
    Serial.print(" L: ");
    Serial.println(minSpeed);
  }
  delay(wait*4);
}

//============================================= reverse ===========================
void reverse() {
  speedR = minSpeed;
  speedL = (speedR * biasB) / 100;
  motorR.run(BACKWARD);
  motorL.run(BACKWARD);
  motorR.setSpeed(speedR);  
  motorL.setSpeed(speedL);  
  delay(wait*4);
  if(debug) {
    Serial.print("Bakward speed R: ");
    Serial.print(speedR);
    Serial.print("L: ");
    Serial.println(speedL);
  }
} // end reverse

// ========================================== PANIC ======================
void panic() {
  stopM();                         // STOP
  delay(wait*2);
  reverse();                       // BAK
  delay(wait*40);
  stopM();                         // STOP
  delay(wait*2);
  if (lastTurn) {                    // Same as last
    spinRight();                   // true: right
  } else {
    spinLeft();                    // false: left
  }
  delay(wait*20);
  stopM();                         // STOP
  delay(wait*2);
} // end panic

// ================================== stopM ===============================
void stopM() {
  motorR.setSpeed(0);  
  motorL.setSpeed(0);
  delay(wait*4);
}

//======================================== increaseSpeed =====================
int increaseSpeed(int speedIn) {
  int speedOut = maxSpeed;
  if (speedIn < medSpeed) { 
    speedOut = medSpeed;
  } // endif
  return speedOut;
} // end increase

// ======================================= decreaseSpeed =====================
int decreaseSpeed(int speedIn) {
  int speedOut = medSpeed;
  if (speedIn < maxSpeed) {        
    speedOut = minSpeed;
  }
  return speedOut;
} //end decrease
// ========================================  LAST LINE ============================
