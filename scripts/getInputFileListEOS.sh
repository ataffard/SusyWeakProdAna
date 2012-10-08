#!/bin/bash
eos='/afs/cern.ch/project/eos/installation/0.1.0-22d/bin/eos.select'
prefix='root://eosatlas'
localDir='SusyWeakProdAna/scripts/EOSFiles'
outDir=$WORKAREA/$localDir 

if [[ $# = 3 ]]; then
    sample=$1
    dir=$2
    method=$3
    echo "Generating sample list for $sample in $dir"
fi


if [ ! -d "${outDir}" ]; then
    mkdir ${outDir}
fi


outFile=${outDir}/${sample}_${method}.txt
[ -f ${outFile} ] && rm -f ${outFile}

for i in $($eos ls $dir);
  do
  echo $prefix/$dir/$i >> ${outFile}
done


