#!/bin/bash

#Usage 
# ./run.sh 105922
# ./run.sh Egamma.periodB3 data12
# ./run.sh 144874 susy


if [[ $# -eq 1 ]]; then
    DS=$1
    type=mc12
elif [[ $# -eq 2 ]]; then
    DS=$1
    type=$2
fi

if [ "$type" == "mc12" ]; then
    if [ "${DS}" == "dummy" ] ; then
	sample=(`more ../scripts/mc12_sampleList.txt |grep 105200 |cut -d" " -f 3-4`)
	./SusyAnaLooperExec -doAll -s  dummy -n 1 -D ${sample} |tee jobLogs/dummy.log
    else
	name=(`more ../scripts/mc12_sampleList.txt |grep ${DS} |cut -d" " -f 1-1`)
	sample=(`more ../scripts/mc12_sampleList.txt |grep ${DS} |cut -d" " -f 3-4`)
	./SusyAnaLooperExec -doAll -s  ${name}  -D ${sample} |tee jobLogs/${name}.log
    fi
elif [ "$type" == "data12" ]; then
    name=(`more ../scripts/data12_sampleList.txt |grep ${DS} |cut -d" " -f 1-1`)
    sample=(`more ../scripts/data12_sampleList.txt |grep ${DS} |cut -d" " -f 3-4`)
    ./SusyAnaLooperExec -doAll -s  ${name}  -D ${sample} |tee jobLogs/${name}.log
elif [ "$type" == "susy" ]; then
    name=(`more ../scripts/susy_sampleList.txt |grep ${DS} |cut -d" " -f 1-1`)
    sample=(`more ../scripts/susy_sampleList.txt |grep ${DS} |cut -d" " -f 3-4`)
    ./SusyAnaLooperExec -doAll -s  ${name}  -D ${sample} |tee jobLogs/${name}.log
fi


