#ifndef {{ cookiecutter.driver_name.upper() }}_H
#define {{ cookiecutter.driver_name.upper() }}_H
#include <asynPortDriver.h>
#include <epicsExport.h>
#include <iocsh.h>


// Error message formatters
#define ERR(msg)                                                                                 \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "ERROR | %s::%s: %s\n", driverName, __func__, \
              msg)

#define ERR_ARGS(fmt, ...)                                                              \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "ERROR | %s::%s: " fmt "\n", driverName, \
              __func__, __VA_ARGS__);

// Warning message formatters
#define WARN(msg) \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "WARN | %s::%s: %s\n", driverName, __func__, msg)

#define WARN_ARGS(fmt, ...)                                                            \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "WARN | %s::%s: " fmt "\n", driverName, \
              __func__, __VA_ARGS__);

// Log message formatters
#define LOG(msg) \
    asynPrint(pasynUserSelf, ASYN_TRACE_FLOW, "%s::%s: %s\n", driverName, __func__, msg)

#define LOG_ARGS(fmt, ...)                                                                       \
    asynPrint(pasynUserSelf, ASYN_TRACE_FLOW, "%s::%s: " fmt "\n", driverName, __func__, \
              __VA_ARGS__);

#define {{ cookiecutter.driver_name.upper() }}_VERSION_MAJOR 0
#define {{ cookiecutter.driver_name.upper() }}_VERSION_MINOR 0
#define {{ cookiecutter.driver_name.upper() }}_VERSION_PATCH 1


// Defines of strings that map Params to Records
#define {{ cookiecutter.driver_name }}_VersionString "{{ cookiecutter.driver_name.upper() }}_VERSION"


class {{ cookiecutter.driver_name }} : public asynPortDriver {

public:
   {{ cookiecutter.driver_name }}(const char* portName);
   ~{{ cookiecutter.driver_name }}();

   /* These are the methods that we override from asynPortDriver as needed*/
   //virtual asynStatus readInt32(asynUser *pasynUser, epicsInt32 *value);
   virtual asynStatus writeInt32(asynUser* pasynUser, epicsInt32 value);
   //virtual asynStatus readFloat64(asynUser *pasynUser, epicsFloat64 *value);
   virtual asynStatus writeFloat64(asynUser* pasynUser, epicsFloat64 value);
   //virtual asynStatus readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason);
   //virtual asynStatus writeOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason);
   //virtual asynStatus connect(asynUser* pasynUser);
   //virtual asynStatus disconnect(asynUser* pasynUser);
   virtual void report(FILE* fp, int details);

protected:

#include "{{ cookiecutter.driver_name }}ParamDefs.h"

private:

{% if cookiecutter.use_tcp_port %}
    asynUser* pasynUserTCPPort;
    asynStatus writeReadSocket(const char* cmd, char* resp, size_t maxChars, double timeout);
{% endif %}

};

#endif