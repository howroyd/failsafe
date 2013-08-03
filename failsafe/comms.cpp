#include "comms.h"

/* Abstract base class for receiving any mavlink datalink */
Comms::Comms(HardwareSerial &port)
{
  _Serial = &port;
  packet_drops = 0;
  mode = MAV_MODE_UNINIT; /* Defined in mavlink_types.h, which is included by mavlink.h */
}

void Comms::communication_receive(void)
{
  mavlink_message_t msg;
  mavlink_status_t status;

  // COMMUNICATION THROUGH UART0 (FROM GROUND STATION)
  while(_Serial->available() > 0)
  {
    uint8_t c = _Serial->read();
    // Try to get a new message
    if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
      // Handle message
      decode(msg);
    }

    // And get the next one
  }

  // Update global packet drops counter
  packet_drops += status.packet_rx_drop_count;
}


/* Ground Control Station Datalink */
CommsGcs::CommsGcs(HardwareSerial &port) : 
Comms(port)
{
  _Serial = &port;
  packet_drops = 0;
  mode = MAV_MODE_UNINIT; /* Defined in mavlink_types.h, which is included by mavlink.h */
}

void CommsGcs::decode(const mavlink_message_t &msg)
{
  switch(msg.msgid)
  {
  case MAVLINK_MSG_ID_HEARTBEAT:
    heartbeat_lastReceived = millis();
    mavlink_msg_heartbeat_decode(&msg, &heartbeat);
    break;
  default:
    //Do nothing
    break;
  }
}


/* UAV Datalink */
CommsUav::CommsUav(HardwareSerial &port) : 
Comms(port)
{
  _Serial = &port;
  packet_drops = 0;
  mode = MAV_MODE_UNINIT; /* Defined in mavlink_types.h, which is included by mavlink.h */
}

void CommsUav::decode(const mavlink_message_t &msg)
{
  switch(msg.msgid)
  {
  case MAVLINK_MSG_ID_HEARTBEAT:
    heartbeat_lastReceived = millis();
    mavlink_msg_heartbeat_decode(&msg, &heartbeat);
    break;
  case MAVLINK_MSG_ID_SYS_STATUS:
    sys_status_lastReceived = millis();
    mavlink_msg_sys_status_decode(&msg, &sys_status);
    break;
  case MAVLINK_MSG_ID_GPS_STATUS:
    gps_status_lastReceived = millis();
    mavlink_msg_gps_status_decode(&msg, &gps_status);
    break;
  default:
    //Do nothing
    break;
  }
}
