#!/bin/bash

# Grab plots
./grabPlots.sh

# Generate latex code
./makePlotTex.sh

# Compile latex code
./compile.sh
./compile.sh
