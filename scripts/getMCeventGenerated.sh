#!/bin/bash

if [[ $# = 1 ]]; then
    dataset=$1
fi

echo "Run the 1st time "
echo "  localSetupPyAMI"
echo "  voms-proxy-init -voms atlas"
echo ""
echo "If D3PD not available:"
echo "  Substitute NTUP_SUSY -> AOD"
echo "  Remove pXXXX"

ami dataset info $1 | grep totalEvents