#!/bin/bash

sub=n0127

InPath=/gdata/atlas/ucintprod/SusyNt
OutDir=$WORKAREA/histoAna/SusyAna/SusyNt_merge/${sub}


if [ ! -d "${OutDir}" ]; then
    mkdir ${OutDir}
fi


list=(`ls -d ${InPath}/data12_${sub}/*period*.physics_Egamma*`)
for iline in ${list[@]}; do
    dir=(`echo $iline | tr ',' ' '`)
    name="Egamma"
    DIS=( `echo $dir |cut -d'.' -f6-6`)
    hadd -f ${OutDir}/${name}.${DIS}_merge.root ${dir}/*
done

list=(`ls -d ${InPath}/data12_${sub}/*period*.physics_Muons*`)
for iline in ${list[@]}; do
    dir=(`echo $iline | tr ',' ' '`)
    name="Muons"
    DIS=( `echo $dir |cut -d'.' -f6-6`)
    hadd -f ${OutDir}/${name}.${DIS}_merge.root ${dir}/*
done
