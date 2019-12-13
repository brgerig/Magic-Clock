#ifndef ClockHand_h
#define ClockHand_h

#include <Arduino.h>
#include <AccelStepper.h>

class ClockHand: public AccelStepper
{
  private:
    long steps;
    int speed;
    int scale;
    int positions;
    int current_position;
    int memaddr;

  public:
    ClockHand(bool invert, int p, int sp, int st, int m, int c1, int c3, int c2, int c4);

    void run();
    void move(long relative);
    void setNewPosition(int newpos);
    int getPosition();
};

#endif
