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
echo "analysis options"
echo "  ana           hadd"
echo "  Outfile       $outfile"
echo "  List          $list"


cd ${SLURM_SUBMIT_DIR}
echo "Starting job on $HOSTNAME: "

echo ${PWD} 
date +"%F__%T"

echo "hadd -f  ${outfile} ${list}"

hadd -f  ${outfile} ${list}

echo "Ending: " 
date +"%F__%T" 

