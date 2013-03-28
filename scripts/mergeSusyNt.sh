#!/bin/bash

sub=n0135

InPath=/gdata/atlas/ucintprod/SusyNt
OutDir=$WORKAREA/histoAna/SusyAna/SusyNt_merge/${sub}

sample="mc12_${sub}/user.gerbaudo.mc12_8TeV.117661.AlpgenPythia_P2011C_ZmumuNp1.SusyNt.e1477_s1499_s1504_r3658_r3549_p1328_n0135d/"
name="ZmumuNp1.117661"

if [ ! -d "${OutDir}" ]; then
    mkdir ${OutDir}
fi


hadd -f ${OutDir}/${name}.merge.root ${InPath}/${sample}/*

<<SKIP
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
SKIP

