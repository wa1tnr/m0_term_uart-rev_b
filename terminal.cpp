

#include <Arduino.h>
// #include "compatibility.h"

extern void blink_m(void);

#define SERIAL Serial1  // the UART is on Serial1

const int STKSIZE = 8;
const int STKMASK = 7;
int stack[STKSIZE];
int p = 0;

/* TOS is Top Of Stack */
#define TOS stack[p]
/* NAMED creates a string in flash */
#define NAMED(x, y) const char x[]=y

char ch;
char chI;

/* push n to top of data stack */
void push(int n) {
  p = (p + 1)& STKMASK;
  TOS = n;
}

/* return top of stack */
int pop() {
  int n = TOS;
  p = (p - 1)& STKMASK;
  return n;
}

void throw_() {
  SERIAL.print("\r\n        THROW:  -1 \r\n");
}


void printing() {
  while (SERIAL.available()) {
    chI = SERIAL.read();
    Serial.print(chI);
  }
}

void seriAvail() {
  for (int i = 1; i < 1400; i++) {
      if (SERIAL.available()) {
        printing();
        delay(1); // makes a huge difference to have this tiny delay
        // break;
    }
  }
}

byte reading() {
  if (!Serial.available()) return 1;
  ch = Serial.read();   // local keystroke is read
  delay(3);
  SERIAL.print(ch);     // immediately send it out the UART port
  seriAvail();
  // seriAvail();
}

void readword() {
  while (reading());
}

/* Arduino main loop */

void setup() {
  SERIAL.begin(38400); // TX/RX pair
  Serial.begin(38400); // USB

  /*
  while (!SERIAL);  // alternate syntax when an empty test is wanted
  */
  while (!Serial) {
    blink_m();
  }

  Serial.println ("terminal - based on the Forth-like interpreter");
}

void loop() {
  readword();
}