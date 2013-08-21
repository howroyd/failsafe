#ifndef COMMS_H_
#define COMMS_H_

#include <Arduino.h>
#include "bodge.h"
#include "C:/Users/Simon/Documents/GitHub/failsafe/libraries/mavlink/include/include/mavlink/v1.0/ardupilotmega/mavlink.h"
#include "errors.h"

/* Abstract base class for mavlink datalink */
class Comms{
public:
  Comms(HardwareSerial* port);

  void communication_receive(void);
  virtual void decode(const mavlink_message_t &msg) = 0;

  int packet_drops;
  int mode; /* Defined in mavlink_types.h, which is included by mavlink.h */

  HardwareSerial* _Serial;

  // Track which MAVLink port we are using in each instance
  static mavlink_channel_t next_free_channel;
  mavlink_channel_t my_channel;

  /* MESSAGES */
  mavlink_heartbeat_t heartbeat;
  unsigned long heartbeat_lastReceived;

  mavlink_sys_status_t sys_status;
  unsigned long sys_status_lastReceived;

  mavlink_gps_raw_int_t gps_raw_int;
  unsigned long gps_raw_int_lastReceived;

  mavlink_attitude_t attitude;
  unsigned long attitude_lastReceived;
};

/* Ground Control Station Datalink */
class CommsGcs : 
public Comms
{
public:
  CommsGcs(HardwareSerial* port);

  void decode(const mavlink_message_t &msg);
};


/* UAV Datalink */
class CommsUav : 
public Comms
{
public:
  CommsUav(HardwareSerial* port);

  void decode(const mavlink_message_t &msg);
};

#endif


