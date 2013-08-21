#include "comms.h"

/* Abstract base class for receiving any mavlink datalink */
Comms::Comms(HardwareSerial* port)
{
  _Serial = port;
  packet_drops = 0;
  my_channel = next_free_channel;
  next_free_channel = mavlink_channel_t(int(next_free_channel) + 1);
}

mavlink_channel_t Comms::next_free_channel = MAVLINK_COMM_0;

void Comms::communication_receive()
{
  mavlink_message_t msg;
  mavlink_status_t status;

  if(_Serial->available()) {
    Serial.print("\t\tReading some bytes: ");
    Serial.println(_Serial->available());
  }

  // COMMUNICATION THROUGH UART
  while(_Serial->available() > 0)
  {
    uint8_t c = _Serial->read();
    //Serial.print(char(c));

    // Try to get a new message
    if(mavlink_parse_char(my_channel, c, &msg, &status)) {
      Serial.print("\t\t***Message received! id:");
      Serial.println(msg.msgid);

      // Handle message
      decode(msg);
    }

    // And get the next one
  }
  // Update global packet drops counter
  packet_drops += status.packet_rx_drop_count;
}


/* Ground Control Station Datalink */
CommsGcs::CommsGcs(HardwareSerial* port) : 
Comms(port)
{
  _Serial = port;
  packet_drops = 0;
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
CommsUav::CommsUav(HardwareSerial* port) : 
Comms(port)
{
  _Serial = port;
  packet_drops = 0;
}

void CommsUav::decode(const mavlink_message_t &msg)
{
  switch(msg.msgid)
  {
  case MAVLINK_MSG_ID_HEARTBEAT:
    Serial.println("         Decoding Heartbeat");
    heartbeat_lastReceived = millis();
    mavlink_msg_heartbeat_decode(&msg, &heartbeat);
    break;
  case MAVLINK_MSG_ID_SYS_STATUS:
    Serial.println("         Decoding Sys Status");
    sys_status_lastReceived = millis();
    mavlink_msg_sys_status_decode(&msg, &sys_status);
    break;
  case MAVLINK_MSG_ID_GPS_RAW_INT:
    Serial.println("         Decoding GPS Status");
    gps_raw_int_lastReceived = millis();
    mavlink_msg_gps_raw_int_decode(&msg, &gps_raw_int);
    break;
  case MAVLINK_MSG_ID_ATTITUDE:
    Serial.println("         Decoding Attitude");
    attitude_lastReceived = millis();
    mavlink_msg_attitude_decode(&msg, &attitude);
    break;
  default:
    //Do nothing
    break;
  }
}


