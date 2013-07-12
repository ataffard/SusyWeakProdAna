#!/bin/bash
#
#PBS -q  c6145
## PBS -q atlas
#PBS -l nodes=1:ppn=1

echo
echo "**************"
echo "**   qsub   **"
echo "**************"

echo
echo "qsub options"
echo "  Host:         $HOSTNAME"
echo "  Queue:        $PBS_O_QUEUE"
echo "  Sub dir:      $PBS_O_WORKDIR"
echo "  Run dir:      $scratch"
echo
echo "analysis options"
echo "  Script:       ${script}"
echo "  Name :        ${jobName}"
echo


cd $PBS_O_WORKDIR

echo "Starting: "
echo ${PWD} 
date +"%F__%T"

echo "${script} >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_hadd.log  "


#${script} >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_hadd.log 

${script}_EE.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_EE_hadd.log 
${script}_MM.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_MM_hadd.log 
${script}_EM.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_EM_hadd.log 

echo "Ending: " 
date +"%F__%T" 



