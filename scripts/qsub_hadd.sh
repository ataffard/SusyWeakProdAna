#!/bin/bash

#SBATCH -p atlas_all
#SBATCH --mem-per-cpu=1gb
#SBATCH --time=8:00:00
#SBATCH --job-name=${name}
#SBATCH --get-user-env


echo
echo "**********************"
echo "**  SLURM OPTIONS   **"
echo "**********************"
echo "  Queue:        ${SLURM_QUEUE}"
echo "  JobId         ${SLURM_JOBID}"
echo "  JobName       ${SLURM_JOB_NAME}"
echo "  Run dir:      ${SLURM_SUBMIT_DIR}"
echo
echo "********************"
echo "**  ANA OPTIONS   **"
echo "********************"
echo
echo "analysis options"
echo "  Script:       ${script}"
echo "  Name :        ${jobName}"
echo

cd ${SLURM_SUBMIT_DIR}
echo "Starting job on $HOSTNAME: "

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



