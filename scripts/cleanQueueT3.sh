#!/bin/bash

if [[ $# = 1 ]]; then
    job=$1
    echo "Requested to kill " ${job}
fi


matches=(`qstat |grep ataffard |cut -d' ' -f 1-1`)

for line in ${matches[@]}; do
    idx=(`echo $line | tr ',' ' '`)
    echo "Idx " ${idx}
    if [[ "$job" == "all" ]] ; then
        qdel ${idx}
        echo "Killing ${idx}"
    else
        if [[ ${job} -eq ${idx} ]] ; then
            qdel ${idx}
            echo "Killing ${idx}"
        fi
    fi
done


qstat |grep ataffard
