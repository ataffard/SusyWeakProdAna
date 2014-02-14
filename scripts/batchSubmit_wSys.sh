#
# Batch node submission script
# Called by submitT3_SusyAna.sh
#

#!/bin/bash

#PBS -q atlas
####PBS -q atlas_slow
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
echo "  Method        $method"
echo "  Sys1:         $sys1"
echo "  Sys2:         $sys2"
echo "  Name:         $name"
echo "  FileDir:      $fileDir"
echo "  nEvt:         $nEvt"

cd $PBS_O_WORKDIR

echo "Starting: "
echo ${PWD} 
date +"%F__%T"

suff=""
if [ -n "$sys1" ]; then
    suff="_"${sys1}
    if [ -n "$sys2" ]; then
	suff="_"${sys1}"-"${sys2}
    fi
fi

./${ana}  -${anaOpt1} -sys1 ${sys1} -sys2 ${sys2} -doMll ${anaOpt2} -method ${method} -s ${name} -n ${nEvt} -D ${fileDir} >& jobLogs/${name}_${method}${suff}.log

echo "Ending: " 
date +"%F__%T" 

