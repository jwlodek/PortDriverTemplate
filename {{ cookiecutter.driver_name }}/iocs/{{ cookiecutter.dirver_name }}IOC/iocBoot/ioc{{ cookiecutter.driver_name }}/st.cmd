#!../../bin/linux-x86_64/{{ cookiecutter.driver_name }}App

errlogInit(20000)
< envPaths

dbLoadDatabase("$(TOP)/dbd/{{ cookiecutter.driver_name }}App.dbd")

{{ cookiecutter.driver_name }}App_registerRecordDeviceDriver(pdbbase)

# Define asyn port name
epicsEnvSet("PORT", "DRV1")
epicsEnvSet("PREFIX", "{{ pv_prefix }}")

{{ cookiecutter.driver_name }}Config("$(PORT)")


#asynSetTraceMask("$(PORT)", -1, 0x0)
#asynSetTraceMask("$(PORT)", -1, 0x1)

# Enables both log and error messages
asynSetTraceMask("$(PORT)", -1, 0x9)
#asynSetTraceMask("$(PORT)", -1, 0xF)
#asynSetTraceMask("$(PORT)", -1, 0x11)
#asynSetTraceMask("$(PORT)", -1, 0xFF)
#asynSetTraceIOMask("$(PORT)", -1, 0x0)
#asynSetTraceIOMask("$(PORT)", -1, 0x2)

dbLoadRecords("$({{ cookiecutter.driver_name.upper() }})/db/{{ cookiecutter.driver_name }}.template", "PREFIX=$(PREFIX), PORT=$(PORT), ADDR=0, TIMEOUT=1")
dbLoadRecords("$(ASYN)/db/asynRecord.template", "P=$(PREFIX), R=AsynIO, ADDR=0, TIMEOUT=1, OMAX=0, IMAX=0")
dbLoadRecords("$(DEVIOCSTATS)/db/iocAdminSoft.db", "IOC=$(PREFIX)")