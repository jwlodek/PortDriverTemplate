TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#

LIBRARY_IOC += {{ cookiecutter.driver_name }}

INC += {{ cookiecutter.driver_name }}.hpp

LIB_SRCS += {{ cookiecutter.driver_name }}.cpp

DBD += {{ cookiecutter.driver_name }}Support.dbd


# #=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE