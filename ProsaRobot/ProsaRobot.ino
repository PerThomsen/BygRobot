/*
 * Robotbyggeprojekt af 'Elektronik & Mekanik' februar 2018
 * 
 * Styring af trehjulet autonom robotbil. 
 * Undgå forhindringer og undgå at køre ud over en kant
 *  
 * Sensorer:
 * Ping-sensor (ultralyd) 1 placeret centralt fortil
 * og 2 placeret i hvert af de forreste hjørner 
 * 
 * Program struktur:
 *  GLOBAL SETTINGS
 *    Benforbindelser
 *    Konstanter
 *    Globale variabler
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
                                        //=============== Benforbindelser
// Sætter ben til hastighedssensorer 
#define encoderInV 8 // input venstre
#define encoderInH 9 // input højre 

// Venstre motor benforbindelser
#define MOTOR_L_PWM 11 // PIN D11 --> MOTOR B+ / PWM Speed (IA2) GUL
#define MOTOR_L_DIR 13 // PIN D13 --> MOTOR B  / Retning (IB2) ORANGE
// Højre motor benforbindelser
#define MOTOR_R_PWM 10 // PIN D10 --> MOTOR B+ / PWM Speed (IA1) GRÅ
#define MOTOR_R_DIR 12 // PIN D12 --> MOTOR B  / Retning (IB1) HVID

                                        //=============== Konstanter 
// Prefixed hastigheder
#define PWM_SLOW 100  // Fastsætter lav hastighed PWM duty cycle
#define PWM_MID 150 // Fastsætter medium hastighedd PWM duty cycle
#define PWM_FAST 200 // Fastsætter hurtig hastighed PWM duty cycle
#define DIR_DELAY 1000 // Kort delay for at gøre motor klar til ændringer

// Retningskonstanter
#define M_FORWARD LOW
#define M_REVERSE HIGH

                                        //=============== Globale variabler
int bias = 0; // Kompensation til højre motor (for at den kører ligeud)

// RMP variabler
int detectStateV=0; // Variabel til aflæsning af venstre encoder status
int detectStateH=0; // Variabel til aflæsning af højre encoder status
int counter;
int lastStateV; // Venstre - Sidste status 
int lastStateH; // Højre - Sidste status
int newStateV;  // Venstre - Ny status
int newStateH;  // Højre - Ny status
int counterV;   // Tæller flanker fra venstre encoder
int counterH;   // Tæller flanker fra højre encoder

                                                   //=============== SETUP
void setup() {
  // Init forbindelse til seriel monitor
  Serial.begin( 9600 );
  
  // Init venstre motor
  pinMode( MOTOR_L_DIR, OUTPUT );
  pinMode( MOTOR_L_PWM, OUTPUT );
  digitalWrite( MOTOR_L_DIR, LOW );
  digitalWrite( MOTOR_L_PWM, LOW );

  // Init højre motor
  pinMode( MOTOR_R_DIR, OUTPUT );
  pinMode( MOTOR_R_PWM, OUTPUT );
  digitalWrite( MOTOR_R_DIR, LOW );
  digitalWrite( MOTOR_R_PWM, LOW );

  // Init encoder
  pinMode(encoderInV, INPUT); //Set pin 8 as input
  pinMode(encoderInH, INPUT); //Set pin 9 as input
  
}

// Inverter
int invertOurValue(int input) {
  return 255 - input;
}

void measureRMP() { 
 // Aflæs omdrejninger (RPM Measurement)
       detectStateV=digitalRead(encoderInV);
       detectStateH=digitalRead(encoderInH);
       counter++;
       
       if (detectStateV == HIGH) { //If encoder output is high
          newStateV = 1;
       } else {
          newStateV = 0;          
       }
       if (lastStateV == newStateV) {
        if (lastStateV == 0) lastStateV = 1; else lastStateV = 0;
        counterV++;
        //Serial.println(counter);
       }

       if (detectStateH == HIGH) { //If encoder output is high
          newStateH = 1;
       } else {
          newStateH = 0;          
       }
       if (lastStateH == newStateH) {
        if (lastStateH == 0) lastStateH = 1; else lastStateH = 0;
        counterH++;
        //Serial.println(counter);
       }
      if (counterV != counterH) {
        Serial.print("Cnt1: ");  
        Serial.print(counterV);
        Serial.print(" Cnt2: ");  
        Serial.println(counterH);
      }
      
      if (counter == 100) {
        counterV = 0;
        counterH = 0;
        counter  = 0;
        //delay(1000);
      }
}

void stopMotor() {
      // Altid stoppe motoren kortvarigt, for at gøre den klar til ændringer
      digitalWrite( MOTOR_L_DIR, LOW );
      digitalWrite( MOTOR_L_PWM, LOW );
      digitalWrite( MOTOR_R_DIR, LOW );
      digitalWrite( MOTOR_R_PWM, LOW );
}

void turnLR() {
      // Turn left or right
      // Der skal modtages 2 parametre
      // L/R og faktor for hastighedsforskel
      // kald speed med de respektive paramertre
}

void spinLR() {
      // Spin left or right
      // Der skal modtages 1 parameter (L/R)
      // Motorene sættes til at køre modsat hinanden
      // digitalWrite( MOTOR_L_DIR, M_FORWARD ); 
      // digitalWrite( MOTOR_R_DIR, M_REVERSE );   
 
}

void runFW(){
  // Kør forlæns
  // stopMotor();
  // delay( DIR_DELAY );
  speed(PWM_FAST, PWM_FAST + bias, M_FORWARD);
}

void runREW(){
  // Kør baglæns
  // stopMotor();
  // delay( DIR_DELAY );
  speed(PWM_FAST, PWM_FAST + bias, M_REVERSE);
}

void speed(int speedL, int speedR, int mDir) {
    if (mDir == HIGH) {
      //Hvis baglæns
      speedL = invertOurValue( speedL );
      speedR = invertOurValue( speedR );
    } 
    digitalWrite( MOTOR_L_DIR, mDir ); 
    digitalWrite( MOTOR_R_DIR, mDir );   

    analogWrite( MOTOR_L_PWM, speedL );           
    analogWrite( MOTOR_R_PWM, speedR );           
}

//Main
void loop() {
  runREW();
  measureRMP();
}
