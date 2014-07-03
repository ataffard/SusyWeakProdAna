#!/bin/bash

if [[ $# = 1 ]]; then
    job=$1
    echo "Requested to kill " ${job}
fi

#matches=(`squeue |grep ataffard |cut -d' ' -f 1-1`)
matches=(`squeue -u ataffard -o %i`)

for line in ${matches[@]}; do
    idx=(`echo $line | tr ',' ' '`)
    echo "Idx " ${idx}
    if [[ "$job" == "all" ]] ; then
        scancel ${idx}
        echo "Killing ${idx}"
    else
        if [[ ${job} -eq ${idx} ]] ; then
            scancel ${idx}
            echo "Killing ${idx}"
        fi
    fi
done

squeue |grep ataffard
