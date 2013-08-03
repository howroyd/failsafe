#include <Arduino.h>
#include "C:/Users/Simon/Documents/GitHub/failsafe/libraries/mavlink/include/mavlink.h" 

/* Abstract base class for mavlink datalink */
class Comms{
  public:
  Comms(HardwareSerial &port);
  
  void communication_receive(void);
  virtual void decode(const mavlink_message_t &msg){};
  
  int packet_drops;
  int mode; /* Defined in mavlink_types.h, which is included by mavlink.h */
  
  HardwareSerial* _Serial;
  
  mavlink_heartbeat_t heartbeat;
  int heartbeat_lastReceived;
};

/* Ground Control Station Datalink */
class CommsGcs : public Comms
{
public:
  CommsGcs(HardwareSerial &port);
  
  void decode(const mavlink_message_t &msg);
};


/* UAV Datalink */
class CommsUav : public Comms
{
public:
  CommsUav(HardwareSerial &port);
  
  void decode(const mavlink_message_t &msg);
  
  mavlink_sys_status_t sys_status;
  int sys_status_lastReceived;
  
  mavlink_gps_status_t gps_status;
  int gps_status_lastReceived;
};


