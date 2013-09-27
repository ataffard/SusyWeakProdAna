#!/bin/bash
#
## PBS -q  c6145
#PBS -q atlas
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


## 2L
${script}_EE.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_EE_hadd.log 
${script}_MM.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_MM_hadd.log 
${script}_EM.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_EM_hadd.log 

##3L 3L
#${script}_EEE.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_EEE_hadd.log 
#${script}_EEM.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_EEM_hadd.log 
#${script}_EMM.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_EMM_hadd.log 
#${script}_MMM.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_MMM_hadd.log 
#${script}_LLL.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_LLL_hadd.log 


#${script}_EEEE.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_EEEE_hadd.log 
#${script}_EEEM.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_EEEM_hadd.log 
#${script}_EEMM.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_EEMM_hadd.log 
#${script}_EMMM.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_EMMM_hadd.log 
#${script}_MMMM.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_MMMM_hadd.log 
#${script}_LLLL.sh >& ${WORKAREA}/SusyWeakProdAna/run/jobLogs/${jobName}_LLLL_hadd.log 

echo "Ending: " 
date +"%F__%T" 



