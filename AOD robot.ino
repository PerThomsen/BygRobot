#include <Scheduler.h>
#include <Scheduler/Semaphore.h>
Semaphore mutex;
int value;
/* Tasks */
void first_task() {
static int i = 1;
Serial.print(�first_task �);
mutex.wait();
value = i;
Serial.println(i);
mutex.signal();
delay(1000);
i++;
}
//Se også:  Sikker programudvikling med C++
void second_task() {
Serial.print(�second_task �);
mutex.wait();
int i = value;
mutex.signal();
Serial.println(i);
delay(2000);
}
void setup() {
Serial.begin(115200);
Serial.println(�setup�);
Scheduler.start(NULL, second_task);
}
void loop() {
first_task();
}
