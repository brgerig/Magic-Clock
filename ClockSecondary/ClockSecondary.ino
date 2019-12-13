// Board: Arduino Pro Mini, ATmega328P, 3.3V, 8MHz

#include "ClockHand.h"
#include <SoftwareSerial.h>

#define STEPS 4096
#define MODE 8
#define SPEED 500
#define POSITIONS 13

#define SERBUFFER 8

// Change the first parameter to switch the direction of rotation
ClockHand personA = ClockHand(true, POSITIONS, SPEED, STEPS, MODE, 2, 4, 3, 5);
ClockHand personB = ClockHand(false, POSITIONS, SPEED, STEPS, MODE, 6, 8, 7, 9);
ClockHand personC = ClockHand(true, POSITIONS, SPEED, STEPS, MODE, 10, 12, 11, 13);
ClockHand personD = ClockHand(false, POSITIONS, SPEED, STEPS, MODE, 14, 16, 15, 17);
ClockHand *active;

SoftwareSerial ser(A5, A4);
char cmd[SERBUFFER];
boolean newData = false;

void setup() {
  Serial.begin(9600);
  ser.begin(9600);
  Serial.println("Startup complete.");
}

static unsigned long lastTime = 0;

void loop() {
  serReceive();
  if(newData) {
    processCommand();
    newData = false;
  }

  personA.run();
  personB.run();
  personC.run();
  personD.run();
}

void serReceive() {
  static byte ndx = 0;
  char endMarker = '#';
  char rc;

  while (ser.available() > 0 && newData == false) {
    rc = ser.read();

    if (rc != endMarker) {
      cmd[ndx] = rc;
      ndx++;
      if (ndx >= SERBUFFER) {
        ndx = SERBUFFER - 1;
      }
    }
    else {
      cmd[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void processCommand() {
  Serial.print("Command received for ");
  switch (cmd[0]) {
    case 'A':
      active = &personA;
      Serial.print("Person A: ");
      break;
    case 'B':
      active = &personB;
      Serial.print("Person B: ");
      break;
    case 'C':
      active = &personC;
      Serial.print("Person C: ");
      break;
    case 'D':
      active = &personD;
      Serial.print("Person D: ");
      break;
    default:
      Serial.print("Invalid");
      break;
  }

  int p = cmd[1];

  active->setNewPosition(p);
  Serial.print("Position ");
  Serial.println(p);
}
