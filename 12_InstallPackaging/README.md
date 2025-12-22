\page installation Installation Guide

This page describes how to install and uninstall the `guesser` project using CMake and the provided installation script.

# Installation

The repository includes a script `install.sh` that automates building and installing.

## Usage

At first, `cd` into the project's source directory. Then run `install.sh` script; the first argument specifies a directory to install:

> `./install.sh /path/to/install`

If omitted, the project is installed into `/usr/local`:

> `./install.sh`

## Script contents

```bash
#!/bin/bash
set -euo pipefail

INSTALL_DIR=${1:-/usr/local}

# Configure the project
cmake -S . -B build/ --install-prefix "$INSTALL_DIR"

# Build the project
cmake --build build/

# Install the project
cmake --install build/ --prefix "$INSTALL_DIR"
```

# Uninstallation

The repository includes a script `uninstall.sh` that automates uninstalling.

## Usage

> `./uninstall.sh`

## Script contents

```bash
#!/bin/bash

set -euo pipefail

# Call uninstall target
cmake --build build/ --target uninstall
```
