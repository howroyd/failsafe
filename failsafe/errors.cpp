#include "errors.h"

/* Generic Error */
void Error::setError(const boolean state) {
  switch (state) {
  case true:
    if (error) 
      break;
    else {
      //Serial.println("*NEW ERROR DETECTED*");
      error = true;
      time = millis();
      break;
    }
  case false:
    error = false;
    break;
  }
}

boolean Error::getError() {
  return error;
}

unsigned long Error::getTime() {
  return time;
}
/* END Generic Error */

/* Error Handler */
void Errors::setError(const ErrorName error, const boolean state) {
  switch (error) {
  case ERROR_COMMS:
    Comms.setError(state);
    break;
  case ERROR_GPS:
    Gps.setError(state);
    break;
  case ERROR_APM:
    Apm.setError(state);
    break;
  case ERROR_KILL:
    Kill.setError(state);
    break;
  }
};

ErrorName Errors::getError() {
  if (Kill.getError()) return ERROR_KILL;
  else if (Apm.getError()) return ERROR_APM;
  else if (Gps.getError()) return ERROR_GPS;
  else if (Comms.getError()) return ERROR_COMMS;
  else return ERROR_NONE;
}

boolean Errors::getError(const ErrorName error) {
  switch (error) {
  case ERROR_COMMS:
    return Comms.getError();
    break;
  case ERROR_GPS:
    return Gps.getError();
    break;
  case ERROR_APM:
    return Apm.getError();
    break;
  case ERROR_KILL:
    return Kill.getError();
    break;
  default:
    return ERROR_NONE;
  }
};

unsigned long Errors::getTime(const ErrorName error) {
  switch (error) {
  case ERROR_COMMS:
    return Comms.getTime();
    break;
  case ERROR_GPS:
    return Gps.getTime();
    break;
  case ERROR_APM:
    return Apm.getTime();
    break;
  case ERROR_KILL:
    return Kill.getTime();
    break;
  default:
    return 0;
  }
};

Sys_status Errors::getState() {
  return sys_status;
};

void Errors::setState(const Sys_status sys_status) {
  this->sys_status = sys_status;
};
/* END Error Handler */
