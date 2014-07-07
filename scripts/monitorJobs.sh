#!/bin/bash

squeue -i 10 -o "%.18i %.9P %.30j %.8u %.2t %.10M %.6D %R"
