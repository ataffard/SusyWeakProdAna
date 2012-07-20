#
# Submit MajAna jobs 
# view queue 
#  qstat -r atlas -n
#  qstat |grep ataffard
# remove job: qdel <jobid>
# 
#
# ./submitT3_SusyAna <type> <pattern>
#     type=mc or data
# ./submitT3_majAna mc12                   // submit all MC
# ./submitT3_majAna data12                 // submit all data
# ./submitT3_majAna mc12 ttbar             // submit all mc ttbar
# ./submitT3_majAna data12 Egamma.periodB1 // submit all data: periodE muon Dil
#

#!/bin/bash


if [[ $# = 1 ]]; then
    type=$1
    pattern=$1
    printf "No args, do all file from %s\n" ${type} 
else
    type=$1
    pattern=$2
    printf "Do all file in %s with %s\n" ${type} ${pattern}
fi

pathRun=${WORKAREA}/SusyWeakProdAna/run
pathScript=${WORKAREA}/SusyWeakProdAna/scripts
ana=SusyAnaLooperExec
anaOpt=doAll
nEvt=-1


dataset=${pathScript}/${type}_sampleList.txt

#filter samples by pattern
cat ${dataset} | grep $pattern >tmp.txt

while read line; do
    #echo $line
    #put each line into array
    sample=(`echo ${line} | tr '\t' ','`)
    sName=${sample[0]}
    sDir=${sample[1]}
    if [[ $sName == *#* ]]; then
	echo "Sample  $sName is commented out - Skippping "
    else
	echo
	date
	echo "Submitting job $ana"
	echo "  name:       $sName"
	echo "  fileDir:    $sDir"
	echo "  nEvt:       $nEvt"
	echo "  anaOpt:     $anaOpt"

    # run the job - submit with qsub
	cd ${pathRun}
	qsub -j oe -V -v ana=$ana,anaOpt=$anaOpt,nEvt=$nEvt,name=$sName,fileDir=$sDir -o ${pathRun}/batchLogs ${pathScript}/batchSubmit.sh
	echo ""
	echo "qsub -j oe -V -v ana=$ana,anaOpt=$anaOpt,nEvt=$nEvt,name=$sName,fileDir=$sDir -N $sName -o ${pathRun}/batchLogs ${pathScript}/batchSubmit.sh "
	cd ${pathScript}
	echo ""
	sleep 3
    fi

done <tmp.txt

qstat |grep ataffard
rm -f tmp.txt