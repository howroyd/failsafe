#include <Servo.h>

#include "C:/Users/Simon/Documents/GitHub/failsafe/libraries/mavlink/include/ardupilotmega/mavlink.h" 
#include "comms.h"
#include "fcs.h"

/* Attached Devices */
CommsGcs GCS(Serial2);
CommsUav UAV(Serial1);
Fcs controller;
int KILLSWITCH = 0;
int killPulse = 0;

/* System States */
enum State {
  Working, Holding, Ditching};
State state = Working;

/* System Errors */
enum Error {
  None, Comms, Gps, Apm, Kill};
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
  Serial2.begin(57600);
  // Setup UART 3

  // Setup PWMIN 1
  do {
    killPulse = pulseIn(KILLSWITCH, HIGH);
  } 
  while (killPulse > 1500 && killPulse < 1000);

  // Setup PWMOUT
  controller.connect();
  
  Serial.println("Setup complete");
}

void loop() {
  //delay(100);
  Serial.println("Start loop");
  
  killPulse = pulseIn(KILLSWITCH, HIGH); // Timeout 1s = 0 return
  if (killPulse > 1500 && killPulse < 1000)
    error = Kill; // TODO comms loss triggers this

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

  case Kill:
    state = Ditching;
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
  controller.ditch();
}



