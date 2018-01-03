/*
  HG7881_Motor_Driver_Example - Arduino sketch      
*/
 
// Right motor
#define MOTOR_R_PWM 10 // PIN D10 --> Motor B Input A --> MOTOR B+ / PWM Speed (IA1) GRÅ
#define MOTOR_R_DIR 12 // PIN D12 --> Motor B Input B --> MOTOR B  / Direction (IB1) HVID
// Left motor
#define MOTOR_L_PWM 11 // PIN D11 --> Motor B Input A --> MOTOR B+ / PWM Speed (IA2) GUL
#define MOTOR_L_DIR 13 // PIN D13 --> Motor B Input B --> MOTOR B  / Direction (IB2) ORANGE

#define PWM_SLOW 100  // arbitrary slow speed PWM duty cycle
#define PWM_FAST 170 // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 1000 // brief delay for abrupt motor changes

#define M_BACK LOW
#define M_FORWARD HIGH
 
void setup() {
  Serial.begin( 9600 );

  pinMode( MOTOR_L_DIR, OUTPUT );
  pinMode( MOTOR_L_PWM, OUTPUT );
  digitalWrite( MOTOR_L_DIR, LOW );
  digitalWrite( MOTOR_L_PWM, LOW );

  pinMode( MOTOR_R_DIR, OUTPUT );
  pinMode( MOTOR_R_PWM, OUTPUT );
  digitalWrite( MOTOR_R_DIR, LOW );
  digitalWrite( MOTOR_R_PWM, LOW );
}

void stopMotor() {
      // always stop motors briefly before abrupt changes
      digitalWrite( MOTOR_L_DIR, LOW );
      digitalWrite( MOTOR_L_PWM, LOW );
      digitalWrite( MOTOR_R_DIR, LOW );
      digitalWrite( MOTOR_R_PWM, LOW );
}

void slowStart(int direk) {
    int j;
    for (int i=PWM_SLOW; i <= PWM_FAST; i++){
      if (direk == 1) {
        j = invertOurValue( i );
      } else {
        j = i;
      }
      analogWrite( MOTOR_R_PWM, j );           
      analogWrite( MOTOR_L_PWM, j );           
      delay(10);
    }  
}

void motorDirection(bool mDir){
  digitalWrite( MOTOR_L_DIR, mDir ); 
  digitalWrite( MOTOR_R_DIR, mDir );   
}

int invertOurValue(int input) {
  return 255 - input;
}

void loop() {
  boolean isValidInput;
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
        stopMotor();
        delay( DIR_DELAY );
        motorDirection(M_FORWARD);
        //analogWrite( MOTOR_R_PWM, 255-PWM_FAST ); // PWM speed = fast          
        //analogWrite( MOTOR_L_PWM, 255-PWM_FAST ); // PWM speed = fast                  
        slowStart(0);
        
        isValidInput = true;
        break;      
                  
      case '3': // 3) Soft stop (preferred)
        Serial.println( "Soft stop (coast)..." );
        stopMotor();
        isValidInput = true;
        break;      
 
         
      case '5': // 5) Fast reverse
        Serial.println( "Fast reverse..." );
        stopMotor();
        delay( DIR_DELAY );
        motorDirection(M_BACK);
        //analogWrite( MOTOR_R_PWM, PWM_FAST+30 ); // PWM speed = fast          
        //analogWrite( MOTOR_L_PWM, PWM_FAST ); // PWM speed = fast          
        slowStart(1);
        
        isValidInput = true;
        break;
                  
      default:
        // wrong character! display the menu again!
        isValidInput = false;
        break;
    }
  } while( isValidInput == true );
 
  // repeat the main loop and redraw the menu...
}
