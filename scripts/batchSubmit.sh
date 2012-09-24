#
# Batch node submission script
# Called by submitT3_SusyAna.sh
#

#!/bin/bash

#PBS -q atlas
#PBS -l nodes=1:ppn=2

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
echo "  ana           $ana"
echo "  AnaOpt1       $anaOpt1"
echo "  AnaOpt2       $anaOpt2"
echo "  Name:         $name"
echo "  FileDir:      $fileDir"
echo "  nEvt:         $nEvt"

cd $PBS_O_WORKDIR

echo "Starting: "
echo ${PWD} 
date +"%F__%T"

./${ana}  -${anaOpt1} -doMll ${anaOpt2} -s ${name} -n ${nEvt} -D ${fileDir} >& jobLogs/${name}.log

echo "Ending: " 
date +"%F__%T" 

