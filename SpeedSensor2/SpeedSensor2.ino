    /*
    -Arduino Position Encoder
    -Using a generic photo-interrupter
    -Basic Test Sketch 1 / June 2014
    -Tested at TechNode Protolabz
    -www.electroschematics.com/
    */
    const int encoderIn1 = 8; // input pin for the interrupter 
    const int encoderIn2 = 9; // input pin for the interrupter 
    const int statusLED = 13; // Output pin for Status indicator

    int detectState1=0; // Variable for reading the encoder status
    int detectState2=0; // Variable for reading the encoder status
    int counter;
    int lastState1;
    int newState1;
    int counter1;
    int lastState2;
    int newState2;
    int counter2;

    void setup() {
       Serial.begin( 9600 );
       pinMode(encoderIn1, INPUT); //Set pin 8 as input
       pinMode(encoderIn2, INPUT); //Set pin 9 as input
       pinMode(statusLED, OUTPUT); //Set pin 13 as output
    }
    
    void loop() {
       detectState1=digitalRead(encoderIn1);
       detectState2=digitalRead(encoderIn2);
       counter++;
       
       if (detectState1 == HIGH) { //If encoder output is high
          digitalWrite(statusLED, HIGH); //Turn on the status LED
          newState1 = 1;
       } else {
          digitalWrite(statusLED, LOW); //Turn off the status LED
          newState1 = 0;          
       }
       if (lastState1 == newState1) {
        if (lastState1 == 0) lastState1 = 1; else lastState1 = 0;
        counter1++;
        //Serial.println(counter);
       }

       if (detectState2 == HIGH) { //If encoder output is high
          newState2 = 1;
       } else {
          newState2 = 0;          
       }
       if (lastState2 == newState2) {
        if (lastState2 == 0) lastState2 = 1; else lastState2 = 0;
        counter2++;
        //Serial.println(counter);
       }
      if (counter1 != counter2) {
        Serial.print("Cnt1: ");  
        Serial.print(counter1);
        Serial.print(" Cnt2: ");  
        Serial.println(counter2);
      }
      
      if (counter == 10) {
        counter1 = 0;
        counter2 = 0;
        counter  = 0;
      }
    }
