#!/bin/bash

histPath=${PWD}

if [ -d plots ]; then rm -rf plots; fi

cp -r ${histPath}/Plots plots


