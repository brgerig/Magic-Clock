#include <Arduino.h>
#include "ClockHand.h"
#include <AccelStepper.h>
#include <EEPROM.h>


ClockHand::ClockHand(bool invert, int pos, int sp, int st, int m, int c1, int c3, int c2, int c4): AccelStepper(m, c1, c3, c2, c4) {
  positions = pos;
  steps = st;
  speed = sp;
  scale = (invert ? -1 : 1);
  current_position = 0;
  memaddr = c1;

  int stored_pos = EEPROM.read(memaddr);
  if(stored_pos != 255) { 
    current_position = stored_pos;
  }

  setMaxSpeed(1000);
  setSpeed(speed);
  setAcceleration(speed*10);
}

void ClockHand::run() {
  AccelStepper::run();
  if(!isRunning()) {
    disableOutputs();
  }
}

void ClockHand::move(long relative) {
  enableOutputs();
  AccelStepper::move(relative);
}

void ClockHand::setNewPosition(int newpos) {
  if(newpos != current_position) {
    long stepPos = ((long)current_position * (float)steps / (float)positions) + distanceToGo();
    stop();
    long newStepPos = (long)newpos * (float)steps / (float)positions;
    long change = newStepPos - stepPos;
    int threshold = steps / 2;
    if (change > threshold) { change -= steps; }
    if (change < -threshold) { change += steps; }
    move(scale*change);

    current_position = newpos;
    EEPROM.update(memaddr,current_position);
  }  
}

int ClockHand::getPosition() {
  return current_position;
}
