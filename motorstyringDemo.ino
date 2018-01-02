/*
  HG7881_Motor_Driver_Example - Arduino sketch
   
  This example shows how to drive a motor with using HG7881 (L9110) Dual
  Channel Motor Driver Module.  For simplicity, this example shows how to
  drive a single motor.  Both channels work the same way.
   
  This example is meant to illustrate how to operate the motor driver
  and is not intended to be elegant, efficient or useful.
   
  Connections:
   
    Arduino digital output D10 to motor driver input IA2.
    Arduino digital output D11 to motor driver input IB2.
    Motor driver VCC to operating voltage 5V.
    Motor driver GND to common ground.
    Motor driver MOTOR B screw terminals to a small motor.
     
  Related Banana Robotics items:
   
    BR010038 HG7881 (L9110) Dual Channel Motor Driver Module
    https://www.BananaRobotics.com/shop/HG7881-(L9110)-Dual-Channel-Motor-Driver-Module
    https://www.bananarobotics.com/shop/How-to-use-the-HG7881-(L9110)-Dual-Channel-Motor-Driver-Module
 
  https://www.BananaRobotics.com

Husk at bytte rundt på ledningerne til edn ene motor

*/
 
// functional connections
#define MOTOR_B_PWM 10 // PIN D10 --> Motor B Input A --> MOTOR B+ / PWM Speed (IA1) GRÅ
#define MOTOR_B_DIR 11 // PIN D11 --> Motor B Input B --> MOTOR B  / Direction (IB1) HVID

#define MOTOR_A_PWM 5 // PIN D5 --> Motor B Input A --> MOTOR B+ / PWM Speed (IA2) GUL
#define MOTOR_A_DIR 6 // PIN D6 --> Motor B Input B --> MOTOR B  / Direction (IB2) ORANGE

// the actual values for "fast" and "slow" depend on the motor
#define PWM_SLOW 100  // arbitrary slow speed PWM duty cycle
#define PWM_FAST 200 // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 1000 // brief delay for abrupt motor changes

//Forward and back
boolean FW = HIGH;
boolean BK = LOW;
 
void setup() {
  Serial.begin( 9600 );

  pinMode( MOTOR_A_DIR, OUTPUT );
  pinMode( MOTOR_A_PWM, OUTPUT );
  digitalWrite( MOTOR_A_DIR, LOW );
  digitalWrite( MOTOR_A_PWM, LOW );

  pinMode( MOTOR_B_DIR, OUTPUT );
  pinMode( MOTOR_B_PWM, OUTPUT );
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );
}

void stopMotor() {
      // always stop motors briefly before abrupt changes
      digitalWrite( MOTOR_A_DIR, LOW );
      digitalWrite( MOTOR_A_PWM, LOW );
      digitalWrite( MOTOR_B_DIR, LOW );
      digitalWrite( MOTOR_B_PWM, LOW );
}

void slowStart() {
    for (int i=PWM_SLOW; i <= PWM_FAST; i++){
      analogWrite( MOTOR_B_PWM, 255-i ); // PWM speed = fast          
      analogWrite( MOTOR_A_PWM, 255-i ); // PWM speed = fast          
      delay(10);
    }  
}

void directionMotor(boolean direction){
  //set direction to Back
  digitalWrite( MOTOR_A_DIR, direction ); 
  digitalWrite( MOTOR_B_DIR, direction );   
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
        directionMotor(FW);
        slowStart();
        
        isValidInput = true;
        break;      
                  
      case '3': // 3) Soft stop (preferred)
        Serial.println( "Soft stop (coast)..." );
        stopMotor();
        isValidInput = true;
        break;      
 
         
      case '5': // 5) Fast reverse
        Serial.println( "Fast forward..." );
        stopMotor();
        delay( DIR_DELAY );
        directionMotor(BK);
        slowStart();
        
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
/*EOF*/
