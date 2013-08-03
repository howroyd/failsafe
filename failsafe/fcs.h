#include <Arduino.h>
#include <Servo.h>

class Fcs{
public:
  ~Fcs();

  void connect();

  void ditch();

  Servo servo1;
  Servo servo2;
  Servo servo3;
  Servo servo4;
};

