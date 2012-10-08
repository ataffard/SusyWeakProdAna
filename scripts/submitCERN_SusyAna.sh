#
# Submit SusyAna jobs to CERN batch queues
# view queue 
# bsub -q 1nh job.sh  
# bsub -q 1nd job.sh  
# bjobs
# bqueues -u ataffard
#

# ./submitCERN_SusyAna <type> <mode>
# ./submitCERN_SusyAna <type> <pattern> <mode>
#     type=mc or data
# ./submitCERN_SusyAna mc12 STD                   // submit all MC STD mode
# ./submitCERN_SusyAna data12 DD                  // submit all data DD mode
# ./submitCERN_SusyAna mc12 ttbar                 // submit all mc ttbar
# ./submitCERN_SusyAna data12 Egamma.periodB1 STD // submit all data: periodE muon Dil


if [[ $# = 2 ]]; then
    type=$1
    pattern=$1
    mode=$2
    printf "No args, do all file from %s\n" ${type} 
else
    type=$1
    pattern=$2
    mode=$3
    printf "Do all file in %s with %s\n" ${type} ${pattern}
fi


pathRun=${WORKAREA}/SusyWeakProdAna/run
pathScript=${WORKAREA}/SusyWeakProdAna/scripts
ana=SusyAnaLooperExec
anaOpt1=doAll
anaOpt2=true #doMll 
methodMC=std
nEvt=-1

##
## Ana mode STD or DD
##
if [ "$mode" == "DD" ]; then
#    methodMC=rlep
    methodMC=std
fi


dataset=${pathScript}/${type}_sampleList.txt

#filter samples by pattern
cat ${dataset} | grep $pattern >tmp.txt

#create dir for submission scripts
batchJobDir="${pathScript}/BatchJobsScripts"
echo ${batchJobDir}
if [ ! -d "${batchJobDir}" ]; then
    mkdir ${batchJobDir}
fi

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
	echo "  anaOpt1:    $anaOpt1"
	echo "  doMll:      $anaOpt2"
	
        ##MC12
	if [ "$type" == "mc12" -o "$type" == "susy" ]; then
	    method=${methodMC}
	    echo "  method:     $method"
	    echo "  Submitting MC "

	    #create submission scripts
	    SCRIPT=${batchJobDir}/${sName}_${method}_job.sh
	    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
	    
	    echo '#!/bin/bash' > $SCRIPT;
	    echo "${pathRun}/${ana} -${anaOpt1} -doMll ${anaOpt2} -method ${method} -s ${sName} -n ${nEvt} -D ${sDir} >& ${pathRun}/jobLogs/${sName}_${method}.log">> $SCRIPT;
	    chmod 755 $SCRIPT

	    #submit script to batch
	    bsub -q 1nh $SCRIPT

	fi
        
        ##DATA12
	if [ "$type" == "data12" ]; then
	    method=std
	    echo "Submitting data STD "
	    
            #create submission scripts
	    SCRIPT=${batchJobDir}/${sName}_${method}_job.sh
	    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
	    
	    echo '#!/bin/bash' > $SCRIPT;
	    echo "${pathRun}/${ana} -${anaOpt1} -doMll ${anaOpt2} -method ${method} -s ${sName} -n ${nEvt} -D ${sDir} >& ${pathRun}/jobLogs/${sName}_${method}.log">> $SCRIPT;
	    chmod 755 $SCRIPT

	    #submit script to batch
	    bsub -q 1nh $SCRIPT
	    
	    #Data driven 
	    if [ "$mode" == "DD" ]; then
		method=flep
		echo "Submitting data DD fake "

                #create submission scripts
		SCRIPT=${batchJobDir}/${sName}_${method}_job.sh
		[ -f ${SCRIPT} ] && rm -f ${SCRIPT}

		echo '#!/bin/bash' > $SCRIPT;
		echo "${pathRun}/${ana} -${anaOpt1} -doMll ${anaOpt2} -method ${method} -s ${sName} -n ${nEvt} -D ${sDir} >& ${pathRun}/jobLogs/${sName}_${method}.log">> $SCRIPT;
		chmod 755 $SCRIPT
		
	        #submit script to batch
	        bsub -q 1nh $SCRIPT
	    fi
	fi
	

    fi   
done <tmp.txt

bjobs
rm -f tmp.txt

#if [ "$type" == "mc12" ]; then
#    cd  ${pathRun}
#    ./run.sh dummy
#fi
