#ifndef ERRORS_H_
#define ERRORS_H_

#include <Arduino.h>

/* Error */
class Error
{
  boolean error;
  unsigned long time;

public:
  void setError(boolean state);
  boolean getError();
  unsigned long getTime();
};

/* System States */
enum Sys_status {
  SYS_STATUS_WORKING, SYS_STATUS_HOLDING, SYS_STATUS_DITCHING};

/* System Errors */
enum ErrorName {
  ERROR_NONE, ERROR_COMMS, ERROR_GPS, ERROR_APM, ERROR_KILL};

/* Error Handler */
class Errors{
  // List of errors that could happen
  Error Comms;
  Error Gps;
  Error Apm;
  Error Kill;
  Sys_status sys_status;
public:
  // Constructor
  Errors() : 
  Comms(), Gps(), Apm(), Kill() {
  }

  ErrorName getError();
  boolean getError(const ErrorName error);
  unsigned long getTime(const ErrorName error);
  void setError(const ErrorName error, const boolean state);

  Sys_status getState();
  void setState(const Sys_status sys_status);
};

#endif
