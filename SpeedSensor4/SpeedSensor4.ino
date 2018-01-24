
volatile byte rpmcount;
unsigned int rpm;
unsigned long timeold;
const int x_size = 10;
const int y_size = 5;


int i =0;
int m[x_size][y_size] ={{0,0,2,0,0},
                        {0,1,0,1,0},
                        {99,1,1,1,0},
                        {0,0,0,0,0},
                        {0,0,0,0,0},
                        {0,0,0,0,0},
                        {0,0,0,0,0},
                        {0,0,0,0,0},
                        {0,0,0,0,0},
                        {0,0,0,0,0}};
 
#define MotorR_I1     8   
#define MotorR_I2     9   
#define MotorL_I3     10 
#define MotorL_I4     11 
#define MotorR_ENA    5   
#define MotorL_ENB    6   
#define initial_speedr 100
#define initial_speedl 100


void setup() {
  Serial.begin(9600);
  pinMode(MotorR_I1,OUTPUT);
  pinMode(MotorR_I2,OUTPUT);
  pinMode(MotorL_I3,OUTPUT);
  pinMode(MotorL_I4,OUTPUT);
  pinMode(MotorR_ENA,OUTPUT);
  pinMode(MotorL_ENB,OUTPUT);
 
attachInterrupt(0,rpm_fun,CHANGE);
attachInterrupt(1,rpm_fun2,CHANGE);
 rpmcount = 0;
  rpm = 0;
  timeold = 0;

}
void advance(int d)   
{   analogWrite(MotorR_ENA,initial_speedr); 
    analogWrite(MotorL_ENB,initial_speedl);
    digitalWrite(MotorR_I1,LOW); 
    digitalWrite(MotorR_I2,HIGH);
    digitalWrite(MotorL_I3,LOW);   
    digitalWrite(MotorL_I4,HIGH);
    delay(d * 100);
}

void turnR(int d)   
{
    analogWrite(MotorR_ENA,initial_speedr);   
    analogWrite(MotorL_ENB,initial_speedl);
    digitalWrite(MotorR_I1,HIGH);   
    digitalWrite(MotorR_I2,LOW);
    digitalWrite(MotorL_I3,LOW);   
    digitalWrite(MotorL_I4,HIGH);
    delay(d * 100);
}

void turnL(int d)   
{   analogWrite(MotorR_ENA,initial_speedr);   
    analogWrite(MotorL_ENB,initial_speedl);
    digitalWrite(MotorR_I1,LOW);   
    digitalWrite(MotorR_I2,HIGH);
    digitalWrite(MotorL_I3,HIGH);   
    digitalWrite(MotorL_I4,LOW);
   
    delay(d * 100);
}   

void stopRL(int d) 
{   
    digitalWrite(MotorR_I1,HIGH);   
    digitalWrite(MotorR_I2,HIGH);
    digitalWrite(MotorL_I3,HIGH);   
    digitalWrite(MotorL_I4,HIGH);
    delay(d * 100);
}



void moveForward(void)
{if(rpmcount!=0)rpmcount=0;
  while(rpmcount<=48)
  advance(0);
  rpmcount=0;
  stopRL(5);
 
}


void turnLeft90(void)
{if(rpmcount!=0)rpmcount=0;
  while(rpmcount<=20)
  turnL(0);
  rpmcount=0;
  stopRL(5);
 
}
void turnRight90(void)
{if(rpmcount!=0)rpmcount=0;
 while(rpmcount<=20)
  turnR(0);
  rpmcount=0;
  stopRL(5);
}
void update_map(void)
{
  for(int x=0;x<x_size;x++)
  {for(int y=0;y<y_size;y++)
    {if(m[x][y]!=1&&m[x][y]!=99&& m[x][y]!=2)
      m[x][y]=0;
     }
  }
}
void WavefrontSearch(void)
{
  int goal_x, goal_y;
  bool foundWave = true;
  int currentWave = 2; //Looking for goal first
 
  while(foundWave == true)
  {
    foundWave = false;
    for(int y=0; y < y_size; y++)
    {
      for(int x=0; x < x_size; x++)
      {
        if(m[x][y] == currentWave)
        {
          foundWave = true;
          goal_x = x;
          goal_y = y;
 
          if(goal_x > 0) //This code checks the array bounds heading WEST
            if(m[goal_x-1][goal_y] == 0)  //This code checks the WEST direction
              m[goal_x-1][goal_y] = currentWave + 1;
 
          if(goal_x < (x_size - 1)) //This code checks the array bounds heading EAST
            if(m[goal_x+1][goal_y] == 0)//This code checks the EAST direction
              m[goal_x+1][goal_y] = currentWave + 1;
 
          if(goal_y > 0)//This code checks the array bounds heading SOUTH
            if(m[goal_x][goal_y-1] == 0) //This code checks the SOUTH direction
              m[goal_x][goal_y-1] = currentWave + 1;
 
          if(goal_y < (y_size - 1))//This code checks the array bounds heading NORTH
            if(m[goal_x][goal_y+1] == 0) //This code checks the NORTH direction
              m[goal_x][goal_y+1] = currentWave + 1;
        }
      }
    }
    currentWave++;

    delay(500);
  }
}
void NavigateToGoal()
{
  //Store our Robots Current Position
 int robot_x, robot_y;
 
  //First - Find Goal and Target Locations
  for(int x=0; x < x_size; x++)
  {
    for(int y=0; y < y_size; y++)
    {
      if(m[x][y] == 99)
      {
        robot_x = x;
        robot_y = y;
      }
    }
  }
 
  //Found Goal and Target, start deciding our next path
   int current_x = robot_x;
   int current_y = robot_y;
  int current_facing = 0;
  int next_Direction = 0;
  int current_low = 99;
 
     int Next_X = 0;
    int Next_Y = 0;
  while(current_low > 2)
  {
    current_low = 99; //Every time, reset to highest number (robot)
   
    next_Direction = current_facing;
 
 
    //Check Array Bounds West
    if(current_x > 0)
      if(m[current_x-1][current_y] < current_low && m[current_x-1][current_y] != 1) //Is current space occupied?
    {
      current_low = m[current_x-1][current_y];  //Set next number
      next_Direction = 3; //Set Next Direction as West
      Next_X = current_x-1;
      Next_Y = current_y;
    }
 
    //Check Array Bounds East
    if(current_x < (x_size -1))
      if(m[current_x+1][current_y] < current_low && m[current_x+1][current_y] != 1) //Is current space occupied?
    {
      current_low = m[current_x+1][current_y];  //Set next number
      next_Direction = 1; //Set Next Direction as East
      Next_X = current_x+1;
      Next_Y = current_y;
    }
 
    //Check Array Bounds South
    if(current_y > 0)
      if(m[current_x][current_y-1] < current_low && m[current_x][current_y-1] != 1)
    {
      current_low = m[current_x][current_y-1];  //Set next number
      next_Direction = 2; //Set Next Direction as South
      Next_X = current_x;
      Next_Y = current_y-1;
    }
 
    //Check Array Bounds North
    if(current_y < (y_size - 1))
      if(m[current_x][current_y+1] < current_low && m[current_x][current_y+1] != 1) //Is current space occupied?
    {
      current_low = m[current_x][current_y+1];  //Set next number
      next_Direction = 0; //Set Next Direction as North
      Next_X = current_x;
      Next_Y = current_y+1;
    }
 
    //Okay - We know the number we're heading for, the direction and the coordinates.
    current_x = Next_X;
    current_y = Next_Y;
    //m[current_x][current_y] = '*';
 
    //Track the robot's heading
    while(current_facing != next_Direction)
    {
      if (current_facing > next_Direction)
      {  if(current_facing==3&&next_Direction==0)
           {
            turnRight90();
            current_facing=0;
           }
           else{
        turnLeft90();
        current_facing--;}
      }
      else if(current_facing < next_Direction)
      { if(current_facing==0&&next_Direction==3)
        {
          turnLeft90();
          current_facing=3;
        }
        else{
        turnRight90();
        current_facing++;
        }
      }
    }
    moveForward();
   
    delay(500);
  }
  int s;
 s=m[robot_x][robot_y];
 m[robot_x][robot_y]=m[current_x][current_y];
 m[current_x][current_y]=s;
}
void loop() {
 
  while(i==0){Serial.println("start tracing:");
WavefrontSearch();   
  NavigateToGoal();
 stopRL(5);
 update_map();
 WavefrontSearch();   
  NavigateToGoal();
i=1;}
}
void rpm_fun()
{ //Serial.println( rpmcount);
  rpmcount++;
 
}
void rpm_fun2()
{ //Serial.println( rpmcount);
  rpmcount++;
 
}
