/*
 * Interrupt.ino - this sketch demonstrates how to implement a "virtual" interrupt using
 * the technique of "single step" to avoid heavy duty cycles within the loop function.
 * https://stackoverflow.com/questions/13583419/arduino-interrupt-alternatives
 * 
 */ 
 

int maxPwm = 128;   // max pwm amount
int myPwm = 0;      // current pwm value
int phase = 1;      // current beat phase
int greenPin = 13;  // output led pin
int buttonPin = 9;  // input button pin
int buttonFlag = 1; // button flag for debounce

int myDir[] = {0,1,-1,1,-1}; // direction of heartbeat loop
int myDelay[] = {0,500,1000,500,3000}; // delay in microseconds of a single step

void setup()
{
   pinMode(buttonPin, INPUT); // enable button pin for input
   // it's not necessary to enable the analog output
}

void loop()
{
   if(phase>0) heartbeat(); // if phase 1 to 4 beat, else steady
   buttonRead(); // test if button has been pressed
}

// heartbeat function - each time is executed, it advances only one step
// phase 1: the led is given more and more voltage till myPwm equals to maxPwm
// phase 2: the led is given less and less voltage till myPwm equals to zero
// phase 3: the led is given more and more voltage till myPwm equals to maxPwm
// phase 4: the led is given less and less voltage till myPwm equals to zero
void heartbeat() 
{
   myPwm += myDir[phase];
   analogWrite(greenPin, myPwm);
   delayMicroseconds(myDelay[phase]);
   if(myPwm==maxPwm||myPwm==0) phase = (phase%4)+1;
}

// buttonRead function - tests if the button is pressed; 
// if so, forces phase 0 (no beat) and enlightens the led to the maximum pwm
// and remains in "inoperative" state till the button is released
void buttonRead()
{
   if(digitalRead(buttonPin)!=buttonFlag)   // if button status changes (pressed os released)
   {
      buttonFlag = 1 - buttonFlag; // toggle button flag value
      if(buttonFlag) // if pressed, toggle between "beat" status and "steady" status
      {
         if(phase) myPwm = maxPwm; else myPwm = 0;
         phase = phase==0;
         analogWrite(greenPin, myPwm);
      }
   }
}
