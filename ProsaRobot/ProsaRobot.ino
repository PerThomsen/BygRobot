/*
 * Robotbyggeprojekt af 'Elektronik & Mekanik' februar 2018
 * 
 * Styring af trehjulet selvkørende robotbil. 
 * Undgå forhindringer og undgå at køre ud over en kant
 *  
 * Sensorer:
 * Ping-sensor (ultralyd) 1 placeret centralt fortil
 * og 2 placeret i hvert af de forreste hjørner 
 * 
 * Program struktur:
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
                                        //=============== Benforbindelser
// Sætter ben til hastighedssensorer 
#define encoderInV 8 // input venstre
#define encoderInH 9 // input højre 

// Venstre motor benforbindelser
#define MOTOR_L_PWM 11 // PIN D11 --> Motor B Input A --> MOTOR B+ / PWM Speed (IA2) GUL
#define MOTOR_L_DIR 13 // PIN D13 --> Motor B Input B --> MOTOR B  / Direction (IB2) ORANGE
// Højre motor benforbindelser
#define MOTOR_R_PWM 10 // PIN D10 --> Motor B Input A --> MOTOR B+ / PWM Speed (IA1) GRÅ
#define MOTOR_R_DIR 12 // PIN D12 --> Motor B Input B --> MOTOR B  / Direction (IB1) HVID

                                        //=============== Konstanter 
// Prefixed hastigheder
#define PWM_SLOW 100  // Fastsætter lav hastighed PWM duty cycle
#define PWM_MID 150 // Fastsætter medium hastighedd PWM duty cycle
#define PWM_FAST 200 // Fastsætter hurtig hastighed PWM duty cycle
#define DIR_DELAY 1000 // Kort delay for at gøre motor klar til ændringer

// Sætter retningskonstanter
#define M_FORWARD LOW
#define M_REVERSE HIGH

int bias = 0; // Kompensation til højre motor (for at den kører ligeud)

                                                   //=============== SETUP
void setup() {
  // Init forbindelse til seriel monitor
  Serial.begin( 9600 );
  
  // Init venstre motor
  pinMode( MOTOR_L_DIR, OUTPUT );
  pinMode( MOTOR_L_PWM, OUTPUT );
  digitalWrite( MOTOR_L_DIR, LOW );
  digitalWrite( MOTOR_L_PWM, LOW );

  // Init højre motot
  pinMode( MOTOR_R_DIR, OUTPUT );
  pinMode( MOTOR_R_PWM, OUTPUT );
  digitalWrite( MOTOR_R_DIR, LOW );
  digitalWrite( MOTOR_R_PWM, LOW );
}

// Inverter
int invertOurValue(int input) {
  return 255 - input;
}

void measureRMP() { 
 // Aflæs omdrejninger (RPM Measurement)
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
    stopMotor();
    delay( DIR_DELAY );
    speed(PWM_FAST, PWM_FAST + bias, M_FORWARD);
}

void runREW(){
  // Kør baglæns
  stopMotor();
  delay( DIR_DELAY );
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

void motorTest() {
  boolean isValidInput;
  int bias = 0; // Kompensation til højre motor (for at den kører ligeud)
  // draw a menu on the serial port
  Serial.println( "-----------------------------" );
  Serial.println( "MENU:" );
  Serial.println( "1) Fast forward" );
  Serial.println( "3) Soft stop (coast)" );
  Serial.println( "5) Fast reverse" );
  Serial.println( "-----------------------------" );
  do
  {
    byte c;
    // get the next character from the serial port
    Serial.print( "?" );
    while( !Serial.available() )
      ; // LOOP...
    c = Serial.read();
    // execute the menu option based on the character recieved
    switch( c )
    {
      case '1': // 1) Fast forward
        Serial.println( "Fast forward..." );
        runFW();
        isValidInput = true;
        break;      
                  
      case '3': // 3) Soft stop (preferred)
        Serial.println( "Soft stop (coast)..." );
        stopMotor();
        isValidInput = true;
        break;      
 
         
      case '5': // 5) Fast reverse
        Serial.println( "Fast reverse..." );
        runREW();
        isValidInput = true;
        break;
                  
      default:
        // wrong character! display the menu again!
        isValidInput = false;
        break;
    }
  } while( isValidInput == true );
 
  // Main slutter, og starter forfra.
}

//Main
void loop() {
  delay(1000);
  runFW();
  motorTest();
}
