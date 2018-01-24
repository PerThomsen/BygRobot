/* 
 *  http://www.nutsvolts.com/magazine/article/smileys_workshop_an_avr_c_programming_series_part_16
 */Simple_Motor_Speed_Control 8/13/09 Joe Pardue
 * This program is based on other Arduino code discussed
 * in Smiley's Workshops 9 through 16.
*/

#define ADJUST 5 // Increment or decrement

// variable to keep PWM value 
int value = 0; 
// pin for motor PWM signal
int motorpin = 9;                        

// variables for serial input
int myInput = 0;
int myNum[6];
int myCount = 0;

// always declare interrupt variables
// as volatile
volatile int count = 0;

// serial input converted to integer
int input = 0;

// value for PWM
int speed = 0;

// time keeping
long oldTime = 0;
long newTime = 0;

void setup()
{
   Serial.begin(9600); 
   Serial.println("Simple_Motor_Speed_Control");
   
  // attach interrupt 0 (pin 2) to the 
  // edgeDetect function
  // run function on falling edge interrupt
  attachInterrupt(0,edgeDetect, FALLING); 
  
  oldTime = millis();
}

void loop()
{ 
  newTime = millis();
  if(newTime > (oldTime + 1000))
  {
    oldTime = newTime;
    Serial.print("Count: ");
    Serial.print(count);
    Serial.print(" Input: ");
    Serial.print(input);
    Serial.print(" Speed: ");
    Serial.print(speed);
    Serial.println(); 
    
    if( (speed >= 0)&&(speed<=255) )
    {   
      if(count < input) 
      {
        if (speed != 255)
        {
          speed += ADJUST;
        }
      }
      else
     {
       if (speed != 0)
       {
         speed -= ADJUST;
       }
     }
     analogWrite(motorpin, speed); 
    } 
    else (speed = 0);
    
    count = 0;       
  }
      
  getNum();
  if(myInput == '!')
  {
    // convert end-of-number character '!' to 0
    myInput = 0;
    myNum[--myCount] = 0;
  
    // convert ASCII string to integer
    input = ATOI();
  
    // map the count number to the PWM value
    Serial.print("input: ");
    Serial.println(input,DEC);

    // clean up and do it all again
    clearAll();
  } 
}

// Put serial characters in a character array
void getNum()
{   
  if(Serial.available())
  {  
     myInput = Serial.read(); 
     // put the character in the array
     myNum[myCount++] = myInput;  
  }    
}

int ATOI()
{    
  // algorithm from atoi() in C standard library   
  int i = 0;
  int n = 0;
  for(i = 0; myNum[i] >= '0' && myNum[i] <= '9'; ++i)
    n = 10 * n + (myNum[i] - '0');

  return(n);  
}

void clearAll()
{
  int i;
  
  myCount = 0;
  for(i = 0; i < 6; i++)
  {
    myNum[i] = 0;
  }
  Serial.flush();  
}

// On each IR detector interrupt 
// increment the count
void edgeDetect()
{
  count++;
}
