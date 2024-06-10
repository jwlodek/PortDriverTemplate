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
    asynPrint(pasynUserSelf, ASYN_TRACEIO_DRIVER, "%s::%s: %s\n", driverName, functionName, msg)

#define LOG_ARGS(fmt, ...)                                                                       \
    asynPrint(pasynUserSelf, ASYN_TRACEIO_DRIVER, "%s::%s: " fmt "\n", driverName, functionName, \
              __VA_ARGS__);


using namespace std;

const char* driverName = "{{ cookiecutter.driver_name }}";

/**
 * @brief External configuration function for {{ cookiecutter.driver_name }}.
 *
 * Envokes the constructor to create a new {{ cookiecutter.driver_name }} object
 * This is the function that initializes the driver, and is called in the IOC startup script
 *
 * NOTE: When implementing a new driver with ADDriverTemplate, your camera may use a different
 * connection method than a const char* connectionParam. Just edit the param to fit your device, and
 * make sure to make the same edit to the constructor below
 *
 */
extern "C" int {{ cookiecutter.driver_name }}Config(const char* portName) {
    new {{ cookiecutter.driver_name }}(portName);
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


/*
 * Function overwriting asynPortDriver base function.
 * Takes in a function (PV) changes, and a value it is changing to, and processes the input
 *
 * @params[in]: pasynUser       -> asyn client who requests a write
 * @params[in]: value           -> int32 value to write
 * @return: asynStatus      -> success if write was successful, else failure
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

/*
 * Function overwriting asynPortDriver base function.
 * Takes in a function (PV) changes, and a value it is changing to, and processes the input
 * This is the same functionality as writeInt32, but for processing doubles.
 *
 * @params[in]: pasynUser       -> asyn client who requests a write
 * @params[in]: value           -> int32 value to write
 * @return: asynStatus      -> success if write was successful, else failure
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

/*
 * Function used for reporting {{ cookiecutter.driver_name }} device and library information to a external
 * log file. The function first prints all libuvc specific information to the file,
 * then continues on to the base asynPortDriver 'report' function
 *
 * @params[in]: fp      -> pointer to log file
 * @params[in]: details -> number of details to write to the file
 * @return: void
 */
void {{ cookiecutter.driver_name }}::report(FILE* fp, int details) {
    const char* functionName = "report";
    LOG("Reporting to external log file");
    if (details > 0) {
        fprintf(fp, " Connected Device Information\n");

        asynPortDriver::report(fp, details);
    }
}


{{ cookiecutter.driver_name }}::{{ cookiecutter.driver_name }}(const char* portName)
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

    createParam({{ cookiecutter.driver_name }}VersionString, asynParamOctet, &{{ cookiecutter.driver_name }}Version);

    // when epics is exited, delete the instance of this class
    epicsAtExit(exitCallbackC, (void*)this);
}

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


/* Array of config args */
static const iocshArg* const {{ cookiecutter.driver_name }}ConfigArgs[] = {&{{ cookiecutter.driver_name }}ConfigArg0};

/* what function to call at config */
static void config{{ cookiecutter.driver_name }}CallFunc(const iocshArgBuf* args) {
    {{ cookiecutter.driver_name }}Config(args[0].sval);
}

/* information about the configuration function */
static const iocshFuncDef config{{ cookiecutter.driver_name }} = {"{{ cookiecutter.driver_name }}Config", 1, {{ cookiecutter.driver_name }}ConfigArgs};

/* IOC register function */
static void {{ cookiecutter.driver_name }}Register(void) { iocshRegister(&config{{ cookiecutter.driver_name }}, config{{ cookiecutter.driver_name }}CallFunc); }

/* external function for IOC register */
extern "C" {
epicsExportRegistrar({{ cookiecutter.driver_name }}Register);
}