#ifndef FCS_H_
#define FCS_H_

#include <Arduino.h>
#include <Servo.h>
#include "errors.h"

class Fcs{
public:
  ~Fcs();

  void connect();

  boolean connected();

  void hold();
  void ditch();

  Servo servo1;
  Servo servo2;
  Servo servo3;
  Servo servo4;
};

#endif

