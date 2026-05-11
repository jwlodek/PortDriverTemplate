TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#

# Default to C++11
USR_CPPFLAGS += -std=c++11

LIBRARY_IOC += {{ cookiecutter.driver_name }}

INC += {{ cookiecutter.driver_name }}.h {{ cookiecutter.driver_name }}ParamDefs.h

LIB_SRCS += {{ cookiecutter.driver_name }}.cpp

DBD += {{ cookiecutter.driver_name }}Support.dbd

# #=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE