// Thu May 21 22:31:03 UTC 2020 // STM32F407 Discovery running C.H. Ting's eForth

// Wed Jun 17 08:49:45 UTC 2020 // Okay for Lumex 96x8 display - not great.
// kidari nefalo

// gelron pibulatto

// STATUS: very good responsiveness, except a
// possible typing feedback latency (slight if even present).

// Today's changes seem to have helped but were done in
// such a way that tracking which changes were best, was
// difficult.

// So, some changes may not be necessary or desirable.

// Still, improved behavior .. getting difficult to tell
// if further improvement can be appreciated or not.

// 21 May 2020, 22:33:55z

// Mon Dec 23 13:59:52 UTC 2019 // STM32F405 Express running C.H. Ting's eForth
// Sat Jun  8 00:38:57 UTC 2019
// fyndov pdxva

// jifx

// #define WEIGHT=1400
// #define WEIGHT 3400
// may 21 2221z working with 3400 #define WEIGHT 3400
// #define WEIGHT 1200
#define WEIGHT 1800
// discussion: less than 1200 created input echo blocking stuff.
// was necessary to space bar flog to get complete feedback,
// so this WEIGHT must be > 800 or so (probably 1200 or so).
// NOTE: seems to have an absurd upper bound (tried 9600 and
// program continued to function about the same).

#include <Arduino.h>
// #include "compatibility.h"

extern void blink_m(void);
extern void setup_dotstar(void); // dotstar.cpp

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

void seriAvail() { // may very well be responsible for the eForth bug
  for (int i = 1; i < WEIGHT; i++) {
      if (SERIAL.available()) {
        printing();
        if (!SERIAL.available()) delay(1);
        // delay(1); // makes a huge difference to have this tiny delay
        // break;
    }
  }
}

void local_echo(void) {
  // Serial.print("DEBUG aa");
  // if (ch == '\n') { Serial.print(' '); return; }
  // WAS ENABLED:
  Serial.print(ch);     // TODO: only if 'echo ON'
  // Serial.print('\n');
/*
  if (ch == '\n')     Serial.print("\r");
  if (ch == '\r') Serial.print("\n"); // way over
*/
}

byte reading() {
  if (!Serial.available()) return 1;
  ch = Serial.read();   // local keystroke is read
  // delay(3);
  local_echo();         // echo that character, locally // KLUDGE

  // translate return to newline only just before sending out the UART:
  // WAS ENABLED:
  // if (ch == '\r') ch = '\n'; // Control M becomes Control J - for convenience of human (REMAP KEYSTROKE)

  SERIAL.write(ch);     // immediately send it out the UART port

  // prod eForth:
  if (ch == '\t') { // supposed to be '\r' tnr 17 June KludgeWerks
    // HERE HERE 21 MAY
  while (!SERIAL.available()); // new blocking 21 may 2207z
  // discussion - this infinite while didn't break anything obvious.
  // seems to resolve the issue nicely.  The delay below (24 atm)
  // may now be superfluous, and other delays are also now suspect.
  // even 'WEIGHT' may be suspect.

  // This is big news!

    // delay(24); // originally '12' not '1' but '1' seems to work. try 12 if unresponsive.

    // discussion: this 'delay(24)' allows an activity on the STM32F407 to complete.

    Serial.print("");
    SERIAL.print(""); // prod eForth maybe
    // FINDINGS: more is less - do NOT delay or output will be lost.
  }
  // ^^ eForth doesn't like to report, unprodded,
  //    on most (but not all) words.
  //    .S is an exception, for example.
  //    Had suspected eForth itself was
  //    coded somewhat unintuitively; now
  //    it seems more likely that serial
  //    timing is responsible for the 'bug'
  //    ('failure to complete without an
  //    '.. additional space/other char
  //    '.. sent to eForth, after having
  //    '.. pressed ENTER').

  seriAvail();
  // seriAvail();
}

void readword() {
  while (reading());
  delay(1);
}

/* Arduino main loop */

void setup() {
  // Serial.begin(38400); // USB
  Serial.begin(115200); // USB
  setup_dotstar(); // turn off dotstar (apa-102 RGB LED)

  /*
  while (!SERIAL);  // alternate syntax when an empty test is wanted
  */
  while (!Serial) {
    blink_m();
  }

  Serial.println ("terminal - based on the Forth-like interpreter\n");
  Serial.println ("    16 SEP 2020     cc388e60943bfb9 ");
  // delay(100);
  // SERIAL.begin(38400); // TX/RX pair
  // worked well with eForth // SERIAL.begin(115200); // TX/RX pair
  // SERIAL.begin(115200); // TX/RX pair
  SERIAL.begin(115200); // TX/RX pair
  // SERIAL.begin(57600); // assume half clock speed mecrisp // TX/RX pair
}

void loop() {
  readword();
}
