#include <Servo.h>
#include "bodge.h"
#include "errors.h"
#include "C:/Users/Simon/Documents/GitHub/failsafe/libraries/mavlink/include/include/mavlink/v1.0/ardupilotmega/mavlink.h" 
#include "comms.h"
#include "fcs.h"

#define MIN_PWM 1000
#define MAX_PWM 2000
#define HEARTBEAT_TIMEOUT 10000
#define GPS_TIMEOUT 20000

boolean LED = LOW;
int loopCount = 0;

/* Attached Devices */
CommsUav UAV(&Serial1);
//CommsGcs GCS(&Serial2);
Fcs controller;
int KILLSWITCH = 0;
int killPulse = 0;

Errors errors;

/* System States */
// Working, Holding, Ditching

/* System Errors */
// Comms, Gps, Apm, Kill

/* System Methods */
void doNothing();
void doHold();
void doDitch();

void setup() {
  // Setup UART 0
  Serial.begin(57600);
  Serial.print("\n\n**********SETUP**********\n\n");

  Serial.print("Connecting to UART........");
  do {
    // Setup UART 1
    Serial1.begin(57600);
    // Setup UART 2
    Serial2.begin(57600);
    // Setup UART 3
    Serial3.begin(57600);
    Serial.print(',');
  } 
  while(!(Serial && Serial1 && Serial2 && Serial3));
  Serial.println("connected");

  //Serial.print("UAV on MAVLink port ");
  //Serial.println(int(UAV.my_channel));
  //Serial.print("GCS on MAVLink port ");
  //Serial.println(int(GCS.my_channel));

  // Setup PWMIN 1
  Serial.print("Connecting to killswitch..");
  do {
    killPulse = pulseIn(KILLSWITCH, HIGH);
    Serial.print(',');
  } 
  while (killPulse < MIN_PWM && killPulse > MAX_PWM);
  Serial.println("connected");

  // Setup PWMOUT
  Serial.print("Connecting to servos......");
  do {
    controller.connect();
    Serial.print(',');
  }
  while (!controller.connected());
  Serial.println("connected");

  // Setup LED
  pinMode(13, OUTPUT);

  Serial.println("\n**********DONE**********\n\n");
}

void loop() {
  // Flash LED
  digitalWrite(13, LED=!LED);

  // User information (USB & UART0)
  Serial.print("Loop "); 
  Serial.println(loopCount++);
  //Serial.print("\t");
  //Serial.print(Serial1.available() + Serial2.available() + Serial3.available()); 
  //Serial.println(" bytes in buffers");
  Serial.print("\tlast UAV heartbeat at: "); 
  Serial.println(UAV.heartbeat_lastReceived);

  // Killswitch
  /*
  killPulse = pulseIn(KILLSWITCH, HIGH); // Timeout 1s = 0 return
   if (killPulse > 1500 && killPulse < 2000) {
   if (errors.getError(ERROR_KILL) == false) Serial.println("*NEW KILLSWITCH ERROR DETECTED*");
   errors.setError(ERROR_KILL, true);
   }
   else
   errors.setError(ERROR_KILL, false);
   */

  // MAVLink sniffer
  //GCS.communication_receive();
  UAV.communication_receive();
/*
  // UAV Comms
  unsigned long heartbeat_uav_delta = millis()-UAV.gps_raw_int_lastReceived;
  if (heartbeat_uav_delta > HEARTBEAT_TIMEOUT) {
    if (errors.getError(ERROR_APM) == false) Serial.println("*NEW UAV HEARTBEAT ERROR DETECTED*");
    errors.setError(ERROR_APM, true);
  }
  else {
    errors.setError(ERROR_APM, false);
  }

  // GPS Comms
  unsigned long gps_delta = millis()-UAV.gps_raw_int_lastReceived;
  if (gps_delta > GPS_TIMEOUT) {
    if (errors.getError(ERROR_GPS) == false) Serial.println("*NEW GPS ERROR DETECTED*");
    errors.setError(ERROR_GPS, true);
  }
  else {
    errors.setError(ERROR_GPS, false);
  }

  // GCS Comms
  unsigned long heartbeat_gcs_delta = millis()-GCS.heartbeat_lastReceived;
  if (heartbeat_gcs_delta > HEARTBEAT_TIMEOUT) {
    if (errors.getError(ERROR_COMMS) == false) Serial.println("*NEW GCS HEARTBEAT ERROR DETECTED*");
    errors.setError(ERROR_COMMS, true);
  }
  else {
    errors.setError(ERROR_COMMS, false);
  }

  // Error detector
  switch (errors.getError()) {
  case ERROR_NONE:
    errors.setState(SYS_STATUS_WORKING);
    break;
  case ERROR_COMMS:
    Serial.println("ERROR_COMMS");
    errors.setState(SYS_STATUS_HOLDING);
    break;
  case ERROR_GPS:
    Serial.println("ERROR_GPS");
    errors.setState(SYS_STATUS_HOLDING);
    break;
  case ERROR_APM:
    Serial.println("ERROR_APM");
    errors.setState(SYS_STATUS_HOLDING);
    break;
  case ERROR_KILL:
    Serial.println("ERROR_KILL");
    errors.setState(SYS_STATUS_DITCHING);
    break;
  }

  // Failsafe action
  switch (errors.getState()) {
  case SYS_STATUS_WORKING: // Everything working OK
    doNothing();
    break;
  case SYS_STATUS_HOLDING: // Error detected, but do nothing to see if it gets fixed
    Serial.println("*HOLDING*");
    doHold();
    break;
  case SYS_STATUS_DITCHING: // Error critical, ditch the aircraft
    Serial.println("*DITCHING*");
    doDitch();
    break;
  }
*/
}

// Do nothing method
void doNothing() {
  return;
}

// Holding method
void doHold() {
  controller.hold();
}

// Ditching method
void doDitch() {
  controller.ditch();
}




