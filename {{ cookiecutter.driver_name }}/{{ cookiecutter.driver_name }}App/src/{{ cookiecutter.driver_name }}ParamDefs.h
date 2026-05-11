#ifndef {{ cookiecutter.driver_name.upper() }}_PARAM_DEFS_H
#define {{ cookiecutter.driver_name.upper() }}_PARAM_DEFS_H

// This file is auto-generated. Do not edit directly.
// Generated from {{ cookiecutter.driver_name }}.template

// String definitions for parameters
#define {{ cookiecutter.driver_name }}_VersionString "{{ cookiecutter.driver_name.upper() }}_VERSION"

// Parameter index definitions
int {{ cookiecutter.driver_name }}_Version;

#define {{ cookiecutter.driver_name.upper() }}_FIRST_PARAM {{ cookiecutter.driver_name }}_Version
#define {{ cookiecutter.driver_name.upper() }}_LAST_PARAM {{ cookiecutter.driver_name }}_Version

#define NUM_{{ cookiecutter.driver_name.upper() }}_PARAMS 1

#endif
