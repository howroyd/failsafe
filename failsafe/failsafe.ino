#include "C:/Users/Simon/Documents/GitHub/failsafe/libraries/mavlink/include/mavlink.h"        // Mavlink interface
#include <Servo.h>
#include "comms.h"

/* Attached Devices */
CommsGcs GCS(Serial);
CommsUav UAV(Serial1);

/* System States */
enum State {
  Working, Holding, Ditching};
State state = Working;

/* System Errors */
enum Error {
  None, Comms, Gps, Apm};
Error error = None;

/* System Methods */
void doNothing();
void doHold();
void doDitch();

void setup() {
  // Setup UART 0
  Serial.begin(57600);
  // Setup UART 1
  Serial1.begin(57600);
  // Setup UART 2
  // Setup UART 3

  // Setup PWMIN 1

  // Setup PWMOUT 0
  // Setup PWMOUT 1
  // Setup PWMOUT 2
  // Setup PWMOUT 3
}

void loop() {
  GCS.communication_receive();
  UAV.communication_receive();

  switch (error) { // Detect if we have any errors
  case None:
    state = Working;
    break;

  case Comms:
    state = Holding;
    break;

  case Gps:
    state = Holding;
    break;

  case Apm:
    state = Holding;
    break;
  }



  switch (state) { // Decide what to do about the errors
  case Working: // Everything working OK
    doNothing();
    break;

  case Holding: // Error detected, but do nothing to see if it gets fixed
    doHold();
    break;

  case Ditching: // Error critical, ditch the aircraft (infinite loop)
    while (true) {
      doDitch();
    }
    break;
  }
}


void doNothing() {

}

void doHold() {

}

void doDitch() {

}


