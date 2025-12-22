#!/bin/bash

set -euo pipefail

INSTALL_DIR=${1:-/usr/local}

cmake -S . -B build/ --install-prefix "$INSTALL_DIR"
cmake --build build/
cmake --install build/ --prefix "$INSTALL_DIR"
