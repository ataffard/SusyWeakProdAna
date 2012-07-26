case 
#
# Submit MajAna jobs 
# view queue 
#  qstat -r atlas -n
#  qstat |grep ataffard
# remove job: qdel <jobid>
# 
#
# ./submit_majAna <type> <pattern>
#     type=mc or data
#

#!/bin/bash

if [[ $# = 1 ]]; then
    type=$1
    if [ "$type" == "data" ]; then
	pattern="data"
    else
	pattern="mc"
    fi
    printf "No args, do all file in %s with %s\n" ${type} ${pattern}
else
    type=$1
    pattern=$2
fi

path=${WORKAREA}/SusyAna/run


date="070112_i4"
inpath=${path}/samples/r2012_15-71-01-i4



#Check point to correct file list !!!
dataset=${path}/../scripts/dataset_${type}.txt
outpath=${WORKAREA}/histoAna/FakeLepStudy/histos_${date}
output=${outpath}/logs
valLog=${outpath}/logs/jobValidation_${type}.txt

echo "Validating logs in ${output}" 

[ -f ${valLog} ] && rm -f ${valLog}

if [[ $# = 1 ]]; then
    matches=(`cat ${dataset} | tr '\t' ','`)
else 
    matches=(`cat ${dataset} | grep ${pattern} | tr '\t' ','`)
fi
echo "Using dataset from $dataset"

echo "${#matches[@]} matches"

for line in ${matches[@]}; do
    sample=(`echo $line | tr ',' ' '`)
    name=${sample[0]}
    
    if [[ $name == *#* ]]; then
	echo "Sample  $name is commented out - Skippping "
    else
	nEvt=0
	nEntries=0
	if [ -f ${output}/${name}.log ]; then
	    nEvt=$(more ${output}/${name}.log |grep 'Number of event processed' |cut -d' ' -f6-7)
	    nEntries=$( root -b -q "${WORKAREA}/SusyAna/scripts/getEntries.C(\"${inpath}/${name}.txt\")" |grep "Number of Entries" |cut -d' ' -f4-5)
	else
	    nEvt=-1
	    nEntries=-1
	fi
#	((delta = ( ${nEvt} - ${nEntries}) ))
	delta=`expr $nEvt - $nEntries`
  
	printf "Job %s \t\t\t nEvt:%i \t nEntries:%i \t delta:%i\n" ${name} ${nEvt} ${nEntries} ${delta} >> ${valLog}
	printf "%s  %i %i %i \n" ${name} $nEvt ${nEntries} ${delta}
	echo  $(grep TXNetFile ${output}/${name}.log |uniq)
    fi
done
