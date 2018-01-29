#include <Scheduler.h>
#include <Scheduler/Semaphore.h>
//https://www.altomdata.dk/byg-en-robot-2/4

Semaphore mutex;
int value;
int value2;

/* Tasks */
void first_task() {
  static int i = 1;
  Serial.print("first_task: ");
  mutex.wait();
  value = i;
  Serial.println(i);
  mutex.signal();
  delay(1000);
  i++;
}
//Se også:  Sikker programudvikling med C++
void second_task() {
  Serial.print("second_task: ");
  mutex.wait();
  int j = value2;
  mutex.signal();
  Serial.println(j);
  delay(2000);
  value2=j+10;
}
void setup() {
  //Serial.begin(115200);
  Serial.begin(9600);
  Serial.println("setup");
  Scheduler.start(NULL, second_task);
}
void loop() {
  first_task();
}

