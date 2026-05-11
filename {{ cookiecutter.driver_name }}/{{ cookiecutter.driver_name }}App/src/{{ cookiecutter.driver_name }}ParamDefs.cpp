// This file is auto-generated. Do not edit directly.
// Generated from {{ cookiecutter.driver_name }}.template

#include "{{ cookiecutter.driver_name }}.h"

void {{ cookiecutter.driver_name }}::createAllParams() {
    createParam({{ cookiecutter.driver_name }}_VersionString, asynParamOctet, &{{ cookiecutter.driver_name }}_Version);
}
