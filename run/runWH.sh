
#!/bin/bash

#Usage 
# ./run.sh mc12 105200 STD
# ./run.sh data12 Egamma.periodB3 STD
# ./run.sh susy 144874 STD

#
#./SusyAnaLooperExec -do2L -doMll true -method std -s SAMPLE -D DIR
#./SusyAnaLooperExec -dbgEvt -d 5 -do2L -doMll true -method rlep -s SAMPLE -D DIR
#

Opt1=doWH
#mll40 cut on low mass alpgen
Opt2=true
#Opt2=false

#####
# Run flags
#####
#dbgEvt=""
dbgEvt="-dbgEvt"
#debug=0
debug=6
nEvt="-n -1"
#nEvt="-n 100"


if [[ $# -eq 3 ]]; then
    type=$1
    DS=$2
    mode=$3
elif [[ $# -eq 4 ]]; then
    type=$1
    DS=$2
    mode=$3
    Opt2=$4
fi

NOM=""
#NOM="-sys1 NOM"
#NOM="-sys1 MS_UP"
#NOM="-sys1 NOM -sys2 Pileup_DN"

methodMC=std
methodData=std
if [ "$mode" == "DD" ]; then
    methodMC=rlep
    methodData=flep
fi

if [ "$type" == "mc12" ]; then
    if [ "${DS}" == "dummy" ] ; then
	sample=(`more ../scripts/mc12_sampleList.txt |grep 167007 |cut -d" " -f 3-4`)
	./SusyAnaLooperExec ${NOM} -${Opt1} -doMll ${Opt2} -method ${methodMC} -s  dummy -n 1 -D ${sample} 2>&1 |tee jobLogs/dummy_${methodMC}.log
    else
	name=(`more ../scripts/mc12_sampleList.txt |grep ${DS} |cut -d" " -f 1-1`)
	#sample=(`more ../scripts/mc12_sampleList.txt |grep ${DS} |cut -d" " -f 3-4`)
	sample=(`more ../scripts/mc12_sampleList.txt |grep ${DS} |cut -d" " -f 2-4`)
	echo "Submitting " ${name} 
	echo " " ${sample}
        ./SusyAnaLooperExec  ${dbgEvt} ${nEvt} -d ${debug} ${NOM} -${Opt1} -doMll ${Opt2} -method ${methodMC} -s ${name} -D ${sample} 2>&1 |tee jobLogs/${name}_${methodMC}.log
    fi
elif [ "$type" == "data12" ]; then
    name=(`more ../scripts/data12_sampleList.txt |grep ${DS} |cut -d" " -f 1-1`)
    sample=(`more ../scripts/data12_sampleList.txt |grep ${DS} |cut -d" " -f 3-4`)
    ./SusyAnaLooperExec  ${dbgEvt} ${nEvt} -d ${debug} ${NOM} -${Opt1} -doMll ${Opt2} -method ${methodData} -s  ${name} -D ${sample} 2>&1 |tee jobLogs/${name}_${methodData}.log
elif [ "$type" == "susy" ]; then
    name=(`more ../scripts/susy_sampleList.txt |grep ${DS} |cut -d" " -f 1-1`)
    sample=(`more ../scripts/susy_sampleList.txt |grep ${DS} |cut -d" " -f 3-4`)
    ./SusyAnaLooperExec  ${dbgEvt} ${nEvt} -d ${debug} ${NOM} -${Opt1} -doMll ${Opt2} -method ${methodMC} -s ${name}  -D ${sample} 2>&1 |tee jobLogs/${name}_${methodMC}.log
fi


