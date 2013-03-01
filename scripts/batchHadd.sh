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
echo "  ana           hadd"
echo "  Outfile       $outfile"
echo "  List          $list"

cd $PBS_O_WORKDIR

echo "Starting: "
echo ${PWD} 
date +"%F__%T"

echo "hadd -f  ${outfile} ${list}"

hadd -f  ${outfile} ${list}

echo "Ending: " 
date +"%F__%T" 

