#include <Scheduler.h>
#include <Scheduler/Semaphore.h>
#include <Ardumoto.h>

Ardumoto Moto;

Semaphore light_mutex;
Semaphore motor_mutex;

#define MOTOR_L_PWM 11 // PIN D11 --> Motor B Input A --> MOTOR B+ / PWM Speed (IA2) GUL
#define MOTOR_L_DIR 13 // PIN D13 --> Motor B Input B --> MOTOR B  / Direction (IB2) ORANGE
// Right motor
#define MOTOR_R_PWM 10 // PIN D10 --> Motor B Input A --> MOTOR B+ / PWM Speed (IA1) GRÅ
#define MOTOR_R_DIR 12 // PIN D12 --> Motor B Input B --> MOTOR B  / Direction (IB1) HVID

//Speed
#define PWM_SLOW 100  // arbitrary slow speed PWM duty cycle
#define PWM_MID 150 // arbitrary medium speed PWM duty cycle
#define PWM_FAST 200 // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 1000 // brief delay for abrupt motor changes

#define M_FORWARD LOW
#define M_REVERSE HIGH

#define ANALOG_SENSOR_PIN A0
#define DIGITAL_SENSOR_PIN 3
#define LEDPIN 13

const int dataIN = 2; //IR sensor INPUT

unsigned long prevmillis; // To store time
unsigned long duration; // To store time difference
unsigned long refresh; // To store time for refresh of reading

int rpm; // RPM value
int light_value;

boolean currentstate; // Current state of IR input scan
boolean prevstate; // State of IR sensor in previous scan

struct motor_command {
  int left;
  int right;
};
typedef struct motor_command MotorCommand;

MotorCommand mc;

 
void setup() {
  Serial.begin( 9600 );
  Serial.println("setup");
  Moto.begin();
  Scheduler.start(NULL, light_sensor);
  Scheduler.start(NULL, control_motor);

  pinMode( MOTOR_L_DIR, OUTPUT );
  pinMode( MOTOR_L_PWM, OUTPUT );
  digitalWrite( MOTOR_L_DIR, LOW );
  digitalWrite( MOTOR_L_PWM, LOW );

  pinMode( MOTOR_R_DIR, OUTPUT );
  pinMode( MOTOR_R_PWM, OUTPUT );
  digitalWrite( MOTOR_R_DIR, LOW );
  digitalWrite( MOTOR_R_PWM, LOW );

  pinMode(dataIN,INPUT);       
  prevmillis = 0;
  prevstate = LOW;  
}

 
 // RPM Measurement
  currentstate = digitalRead(dataIN); // Read IR sensor state
 if( prevstate != currentstate) // If there is change in input
   {
     if( currentstate == HIGH ) // If input only changes from LOW to HIGH
       {
         duration = ( micros() - prevmillis ); // Time difference between revolution in microsecond
         rpm = (60000000/duration); // rpm = (1/ time millis)*1000*1000*60;
         prevmillis = micros(); // store time for nect revolution calculation
       }
   }
  prevstate = currentstate; // store this scan (prev scan) data for next scan
  
  // LCD Display
  if( ( millis()-refresh ) >= 100 )
    {
       Serial.println(rpm);  
    }

}
void stopMotor() {
      // always stop motors briefly before abrupt changes
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

void speed(int speedL, int speedR, int mDir) {
    if (mDir == HIGH) {
        //If reverse
        speedL = invertOurValue( speedL );
        speedR = invertOurValue( speedR );
    } 
    digitalWrite( MOTOR_L_DIR, mDir ); 
    digitalWrite( MOTOR_R_DIR, mDir );   

    analogWrite( MOTOR_L_PWM, speedL );           
    analogWrite( MOTOR_R_PWM, speedR );           
}


int invertOurValue(int input) {
  return 255 - input;
}

void light_sensor() {
  int switch_state = digitalRead(DIGITAL_SENSOR_PIN);  
  if (switch_state == LOW) {
    digitalWrite(LEDPIN, HIGH);
  } else {
    digitalWrite(LEDPIN, LOW);
  }

  light_mutex.wait();
  light_value = analogRead(ANALOG_SENSOR_PIN);
  light_mutex.signal();
  delay(250);
}

void control_motor() {
  Serial.print("control_motor ");
  motor_mutex.wait();
  // kopierer værdier - command_center overskriver dem
  int left = mc.left;
  int right = mc.right;
  motor_mutex.signal();

  Serial.println(left, DEC);
  Serial.println(right, DEC);
  Moto.setSpeed('A', left);
  Moto.setSpeed('B', right);
  delay(250);
}
void MOTORSTYR() {
  boolean isValidInput;
  int bias = 0; // Compensation to right motor  // draw a menu on the serial port
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
        speed(PWM_FAST, PWM_FAST + bias, M_FORWARD);
        
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
        speed(PWM_FAST, PWM_FAST + bias, M_REVERSE);
        
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

void command_center() {
  const int max = 100;
  static int last_proportional = 0;
  static int integral = 0;
  
  unsigned int position;
  light_mutex.wait();
  position = light_value;
  light_mutex.signal();
  Serial.println(light_value, DEC);

  int proportional = ((int)position) - 511; // lyssensor mellem 0 og 1023
  int derivative = proportional - last_proportional;
  integral += proportional;
  last_proportional = proportional;

  int power_difference = proportional/20 + integral/10000 + derivative*3/2;
  Serial.print("power_difference = ");
  Serial.println(power_difference, DEC);
  if (power_difference > max)
    power_difference = max;
  if (power_difference < -max)
    power_difference = -max;

  motor_mutex.wait();
  if (power_difference < 0) {
    mc.left = max+power_difference;
    mc.right = max;
  } else {
    mc.left = max;
    mc.right = max-power_difference;
  }
  motor_mutex.signal();
  
  delay(500);
}

void loop() {
  command_center();
}
