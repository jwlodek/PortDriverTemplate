# PortDriverTemplate

This repository contains a pre-organized file structure for writing asyn port drivers.
To use it, start by installing the `cookiecutter` utility. The simplest way to do this is to use `pip`:

```
pip install cookiecutter
```

Next, simply run cookiecutter with the url to this repository as an argument:

```
cookiecutter https://github.com/jwlodek/PortDriverTemplate
```

Follow the prompts, and you should have all the base files and directories for a new driver created!

## Development Tooling

Generated projects include a [pixi](https://pixi.sh) environment with the following tools:

* **Linting** - `pre-commit` hooks with `clang-format` for C/C++ and `ruff` for Python
* **Parameter auto-generation** - `epicsdbtools` generates `ParamDefs.h` and `ParamDefs.cpp` from `.template` files
* **BOB file auto-generation** - `epicsdb2bob` generates Phoebus `.bob` files from `.template` files
* **CI** - GitHub Actions workflow for running lint checks

To set up the development environment after generating a project:

```
cd <driver_name>
pixi install
```

Common tasks:

```
pixi run lint          # Run pre-commit linting checks
pixi run build         # Build the driver
pixi run ioc            # Start the example IOC
pixi run make-paramdefs  # Re-generate ParamDefs from .template files
pixi run make-bobfiles   # Re-generate .bob files from .template files
```

