/* Robimek - Robotik Systems
 *  Speed Sensor
 *  http://make.robimek.com/lm393-ir-speed-sensor-using/
www.robimek.com */
#include <TimerOne.h>
unsigned int sayac=0;
  
int in1 = 9; 
int in2 = 5; 
 int ena = 10;
void say() // sayma işlemi
{
 sayac++; // sayacı bir arttır
} 
  
void timerIsr()
{
 Timer1.detachInterrupt(); //stop timer
 Serial.print(" Motor hastighed : "); 
 int devir = (sayac / 20); // hvor 20 er antal huller i encoder.
 Serial.print(devir,DEC); 
 Serial.println(" antal omdrejninger "); 
 sayac=0; // sayacı sıfırla
 Timer1.attachInterrupt( timerIsr ); // aktiver timerIsr
}
  
void setup() 
{
 Serial.begin(9600);
  
 pinMode(in1, OUTPUT); 
 pinMode(in2, OUTPUT); 
 pinMode(ena, OUTPUT);
 digitalWrite(in1, HIGH); 
 digitalWrite(in2, LOW); 
 Timer1.initialize(1000000); // indstil timer til 1 sekund
 attachInterrupt(0, say, RISING); // forøg tæller, når sensorout er høj.
 Timer1.attachInterrupt( timerIsr ); // aktiver timerIsr
} 
  
void loop()
{
 int pot_deger = analogRead(0); // Læser data fra potentiometer 
 int motor_hizi = map(pot_deger, 0, 680, 255, 0); // beregn hastighedsværdi
 analogWrite(ena, motor_hizi); // send hastighed til motor.
  
}
