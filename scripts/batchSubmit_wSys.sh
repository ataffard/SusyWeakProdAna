#!/bin/bash

#
# Batch node submission script
# Called by submitT3_SusyAna.sh
#

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
echo "  ana           $ana"
echo "  AnaOpt1       $anaOpt1"
echo "  AnaOpt2       $anaOpt2"
echo "  Method        $method"
echo "  Sys1:         $sys1"
echo "  Sys2:         $sys2"
echo "  Name:         $name"
echo "  FileDir:      $fileDir"
echo "  nEvt:         $nEvt"

cd ${SLURM_SUBMIT_DIR}
echo "Starting job on $HOSTNAME: "

echo ${PWD} 
date +"%F__%T"

suff=""
if [ -n "$sys1" ]; then
    suff="_"${sys1}
    if [ -n "$sys2" ]; then
	suff="_"${sys1}"-"${sys2}
    fi
fi

${ana}  -${anaOpt1} -sys1 ${sys1} -sys2 ${sys2} -doMll ${anaOpt2} -method ${method} -s ${name} -n ${nEvt} -D ${fileDir} 2>&1 |tee jobLogs/${name}_${method}${suff}.log

echo "Ending: " 
date +"%F__%T" 

