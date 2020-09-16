/* Sun Feb 26 04:53:32 UTC 2017 */

#include <Arduino.h>
// #include "compatibility.h"

extern void push(int n);
extern int pop(); // {

void d13OUTPUT(void) {
  int a = 0; 
  a =  1;  push(a);
  a = 13;  push(a); 
  pinMode(pop(), pop()); 
}

void ledON(void) {
  int a = 0; 
  a =  1;  push(a);
  a = 13;  push(a); 
  digitalWrite(pop(), pop()); 
}

void ledOFF(void) {
  int a = 0; 
  a =  0;  push(a);
  a = 13;  push(a); 
  digitalWrite(pop(), pop()); 
}

void blink_m(void) {
  int a = 0; 
  d13OUTPUT();
  ledON();
  delay(30); 
  ledOFF();
  delay(2200); 
}
