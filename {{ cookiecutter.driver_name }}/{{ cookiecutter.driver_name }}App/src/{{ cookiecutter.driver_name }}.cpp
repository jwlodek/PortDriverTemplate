/**
 * Main source file for the {{ cookiecutter.driver_name }} EPICS driver
 *
 * Author: {{ cookiecutter.author }}
 *
 * Copyright (c) : {{ cookiecutter.institution }}, {% now 'local', '%Y' %}
 *
 */

// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

// EPICS includes
#include <epicsExit.h>
#include <epicsExport.h>
#include <epicsStdio.h>
#include <epicsString.h>
#include <epicsThread.h>
#include <epicsTime.h>
#include <iocsh.h>

#include "{{ cookiecutter.driver_name }}.hpp"


// Error message formatters
#define ERR(msg)                                                                                 \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "ERROR | %s::%s: %s\n", driverName, functionName, \
              msg)

#define ERR_ARGS(fmt, ...)                                                              \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "ERROR | %s::%s: " fmt "\n", driverName, \
              functionName, __VA_ARGS__);

// Warning message formatters
#define WARN(msg) \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "WARN | %s::%s: %s\n", driverName, functionName, msg)

#define WARN_ARGS(fmt, ...)                                                            \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "WARN | %s::%s: " fmt "\n", driverName, \
              functionName, __VA_ARGS__);

// Log message formatters
#define LOG(msg) \
    asynPrint(pasynUserSelf, ASYN_TRACE_FLOW, "%s::%s: %s\n", driverName, functionName, msg)

#define LOG_ARGS(fmt, ...)                                                                       \
    asynPrint(pasynUserSelf, ASYN_TRACE_FLOW, "%s::%s: " fmt "\n", driverName, functionName, \
              __VA_ARGS__);


using namespace std;

const char* driverName = "{{ cookiecutter.driver_name }}";

/**
 * @brief External configuration function for {{ cookiecutter.driver_name }}.
 *
 * Envokes the constructor to create a new {{ cookiecutter.driver_name }} object
 * This is the function that initializes the driver, and is called in the IOC startup script
 *
 * NOTE: When implementing a new driver with PortDriverTemplate, your device may require additional
 * inputs from the user for connection (ex: IP address, serial number, etc.). These should be added
 * as additional IOC shell arguments for this function.
 *
 * @param portName Asyn port name for the {{ cookiecutter.driver_name }} object instance.
{% if cookiecutter.use_tcp_port %}
 * @param tcpPortName Asyn port name for the TCP socket opened with drvAsynIpPortConfigure
 * @return asynSuccess
 */
extern "C" int {{ cookiecutter.driver_name }}Config(const char* portName, const char* tcpPortName) {
    new {{ cookiecutter.driver_name }}(portName);
{% else %}
 * @return asynSuccess
 */
extern "C" int {{ cookiecutter.driver_name }}Config(const char* portName) {
    new {{ cookiecutter.driver_name }}(portName, tcpPortName);
{% endif %}
    return (asynSuccess);
}


/**
 * @brief Callback function called when IOC is terminated.
 *
 * @param pPvt Pointer to {{ cookiecutter.driver_name }} object
 */
static void exitCallbackC(void* pPvt) {
    {{ cookiecutter.driver_name }}* p{{ cookiecutter.driver_name }} = ({{ cookiecutter.driver_name }}*)pPvt;
    delete p{{ cookiecutter.driver_name }};
}

{% if cookiecutter.use_tcp_port %}
/**
 * @brief Function that sends command to the open tcp socket, and waits for a response.
 * 
 * @param cmd Command to send to the socket (w/o terminator - set in IOC startup)
 * @param ret Character buffer into which response from socket should be inserted
 * @param maxChars Maximum number of characters in the response.
 * @param timeout Max seconds to wait for timeout.
 * @return asynStatus of write/read command to the socket.
 */
asynStatus {{ cookiecutter.driver_name }}::writeReadSocket(const char* cmd, char* ret, size_t maxChars, double timeout){

    size_t nwrite, nread;
    asynStatus status;
    int eomReason;
    const char *functionName="writeReadSocket";
    pasynOctetSyncIO->flush(this->pasynUserTCPPort);
    status = pasynOctetSyncIO->writeRead(this->pasynUserTCPPort, cmd,
                                      strlen(cmd), ret, maxChars, timeout,
                                      &nwrite, &nread, &eomReason);
    return status;
}
{% endif %}


/**
 * @brief Handles write events to integer parameters
 * 
 * @param pasynUser Pointer to asynUser for {{ cookiecutter.driver_name }} instance
 * @param value Value written to the integer parameter
 * @return asynSuccess if write was successful, asynError otherwise
 */
asynStatus {{ cookiecutter.driver_name }}::writeInt32(asynUser* pasynUser, epicsInt32 value) {
    int function = pasynUser->reason;
    int status = asynSuccess;
    static const char* functionName = "writeInt32";

    if (function < FIRST_{{ cookiecutter.driver_name.upper() }}_PARAM) {
        status = asynPortDriver::writeInt32(pasynUser, value);
    }

    if (status) {
        ERR_ARGS("ERROR status=%d, function=%d, value=%d", status, function, value);
        return asynError;
    } else {  // Don't log period checkStatus PV processing
        status = setIntegerParam(function, value);
        LOG_ARGS("function=%d value=%d", function, value);
    }
    callParamCallbacks();
    return asynSuccess;
}

/**
 * @brief Handles write events to float parameters
 * 
 * @param pasynUser Pointer to asynUser for {{ cookiecutter.driver_name }} instance
 * @param value Value written to the double parameter
 * @return asynSuccess if write was successful, asynError otherwise
 */
asynStatus {{ cookiecutter.driver_name }}::writeFloat64(asynUser* pasynUser, epicsFloat64 value) {
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    static const char* functionName = "writeFloat64";
    if (function < FIRST_{{ cookiecutter.driver_name.upper() }}_PARAM) {
        status = asynPortDriver::writeFloat64(pasynUser, value);
    }

    if (status) {
        ERR_ARGS("ERROR status=%d, function=%d, value=%f", status, function, value);
    } else {
        status = setDoubleParam(function, value);
        LOG_ARGS("function=%d value=%f", function, value);
    }

    callParamCallbacks();
    return status;
}

/**
 * @brief Function used for reporting {{ cookiecutter.driver_name }} info to a log.
 *
 * @param fp Open file pointer to log file
 * @param details Level of details to write to the file
 */
void {{ cookiecutter.driver_name }}::report(FILE* fp, int details) {
    const char* functionName = "report";
    LOG("Reporting to external log file");
    if (details > 0) {
        fprintf(fp, " Connected Device Information\n");
        asynPortDriver::report(fp, details);
    }
}

/**
 * @brief Constructor for {{ cookiecutter.driver_name }}
 * 
 * Responsible for initial connection to the device - instance initialized in
 * {{ cookiecutter.driver_name }}Config, called at IOC startup.
 * 
 * @param portName Asyn port name for the {{ cookiecutter.driver_name }} object instance.
{% if cookiecutter.use_tcp_port %}
 * @param tcpPortName Asyn port name for the TCP socket opened with drvAsynIpPortConfigure
{{ cookiecutter.driver_name }}::{{ cookiecutter.driver_name }}(const char* portName, const char* tcpPortName)
{% else %}
 */
{{ cookiecutter.driver_name }}::{{ cookiecutter.driver_name }}(const char* portName)
{% endif %}
    : asynPortDriver(
          portName, 1, /* maxAddr */
          (int)NUM_{{ cookiecutter.driver_name.upper() }}_PARAMS,
          asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask | asynDrvUserMask |
              asynOctetMask, /* Interface mask */
          asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask |
              asynOctetMask, /* Interrupt mask */
          0, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
          1, /* Autoconnect */
          0, /* Default priority */
          0) /* Default stack size*/
{
    static const char* functionName = "{{ cookiecutter.driver_name }}";

    // Create any driver specific parameters
    createParam({{ cookiecutter.driver_name }}_VersionString, asynParamOctet, &{{ cookiecutter.driver_name }}_Version);

{% if cookiecutter.use_tcp_port %}
    // Connect to the TCP port configured at IOC startup
    pasynOctetSyncIO->connect(tcpPortName, 0, &this->pasynUserTCPPort, NULL);

{% endif %}
    // When epics is exited, delete the instance of this class
    epicsAtExit(exitCallbackC, (void*)this);
}

/**
 * @brief Destructor for {{ cookiecutter.driver_name }}
 * 
 * Called at IOC exit.
 */
{{ cookiecutter.driver_name }}::~{{ cookiecutter.driver_name }}() {
    const char* functionName = "~{{ cookiecutter.driver_name }}";
    LOG("Disconnecting {{ cookiecutter.driver_name }} device...");
    LOG("Shutdown complete.");
}

//-------------------------------------------------------------
// {{ cookiecutter.driver_name }} ioc shell registration
//-------------------------------------------------------------

/* {{ cookiecutter.driver_name }}Config -> These are the args passed to the constructor in the epics config function */
static const iocshArg {{ cookiecutter.driver_name }}ConfigArg0 = {"portName", iocshArgString};
{% if cookiecutter.use_tcp_port %}
static const iocshArg {{ cookiecutter.driver_name }}ConfigArg1 = {"tcpPortName", iocshArgString};
{% endif %}


/* Array of config args */
{% if cookiecutter.use_tcp_port %}
static const iocshArg* const {{ cookiecutter.driver_name }}ConfigArgs[] = {&{{ cookiecutter.driver_name }}ConfigArg0};
{% else %}
static const iocshArg* const {{ cookiecutter.driver_name }}ConfigArgs[] = {&{{ cookiecutter.driver_name }}ConfigArg0,
                                                    &{{ cookiecutter.driver_name }}ConfigArg1};
{% endif %}

/**
 * @brief Call function pointer for IOC shell.
 *
 * @param args Array of IOC shell arguments parsed during IOC startup
 */
static void config{{ cookiecutter.driver_name }}CallFunc(const iocshArgBuf* args) {
    {% if cookiecutter.use_tcp_port %}
    {{ cookiecutter.driver_name }}Config(args[0].sval);
    {% else %}
    {{ cookiecutter.driver_name }}Config(args[0].sval, args[1].sval);
    {% endif %}
}

/* information about the configuration function */
static const iocshFuncDef config{{ cookiecutter.driver_name }} = {"{{ cookiecutter.driver_name }}Config", {% if cookiecutter.use_tcp_port %}2{% else %}1{% endif %}, {{ cookiecutter.driver_name }}ConfigArgs};

/* IOC register function */
static void {{ cookiecutter.driver_name }}Register(void) { iocshRegister(&config{{ cookiecutter.driver_name }}, config{{ cookiecutter.driver_name }}CallFunc); }

/* external function for IOC register */
extern "C" {
epicsExportRegistrar({{ cookiecutter.driver_name }}Register);
}