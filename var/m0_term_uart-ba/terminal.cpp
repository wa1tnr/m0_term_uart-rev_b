// Wed Sep 16 02:26:06 UTC 2020 lumex-bb- // converse with STM32F407

// chvargu  kidari  nefalo  gelron  pibulatto

#include <Arduino.h>

#define LOCAL_ECHO_ON
#undef LOCAL_ECHO_ON

#define WEIGHT 1800
#define SERIAL Serial1  // the UART is on Serial1

extern void blink_m(void);
extern void setup_dotstar(void); // dotstar.cpp

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

void seriAvail() { // may very well be responsible for the eForth bug
  for (int i = 1; i < WEIGHT; i++) {
      if (SERIAL.available()) {
        printing();
        if (!SERIAL.available()) delay(1);
    }
  }
}

void local_echo(void) {
  Serial.print(ch);     // TODO: only if 'echo ON'
}

byte reading() {
  if (!Serial.available()) return 1;
  ch = Serial.read();   // local keystroke is read
#ifdef LOCAL_ECHO_ON
  local_echo();         // echo that character, locally // KLUDGE
#endif

  SERIAL.write(ch);     // immediately send it out the UART port

  if (ch == '\r') { // supposed to be '\r' tnr 17 June KludgeWerks
  while (!SERIAL.available()); // new blocking 21 may 2207z

    Serial.print("");
    SERIAL.print(""); // prod eForth maybe
  }
  seriAvail();
}

void readword() {
  while (reading());
  delay(1);
}

/* Arduino main loop */

void setup() {
  Serial.begin(115200); // USB
  setup_dotstar(); // turn off dotstar (apa-102 RGB LED)

  while (!Serial) {
    blink_m();
  }

  Serial.println ("terminal - based on the Forth-like interpreter\n");
  Serial.println ("    16 SEP 2020     cc388e60943b-b01 ");
  SERIAL.begin(115200); // TX/RX pair
}

void loop() {
  readword();
}
