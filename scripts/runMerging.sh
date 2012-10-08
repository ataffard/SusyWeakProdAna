#!/bin/bash

#
# Run merging scripts on lxplus batch
#
localDir='SusyWeakProdAna/scripts/MergeJobsScripts'
dir=$WORKAREA/$localDir

while read line; do
   text=(`echo ${line} | tr '\t' ','`)
   SCRIPT=${text[0]}
   if [[ $text == *#Merge* ]]; then
       echo "Script  $SCRIPT is commented out - Skippping "
   elif [[ $text == *Merge* ]]; then
       #bsub -q 1nh ${PWD}/$SCRIPT
       echo "Run merging $SCRIPT"
   fi
   
done <mergeScripts.txt

bjobs


echo 
echo "Grep for error: grep -i error LSFJOB_*/STDOUT "
echo 
