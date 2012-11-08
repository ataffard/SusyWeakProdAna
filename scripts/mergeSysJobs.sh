#!/bin/bash

if [[ $# = 1 ]]; then
    sample=$1
fi

path=${WORKAREA}/histoAna/SusyAna/histos_110812_13fb_n0111_DD_MMtrial9_SYS_HFT

if [ "$sample" == "EG" ] ; then
    hadd -f ${path}/histo_Egamma.periodA_flep.root ${path}/histo_Egamma.periodA_flep_*
    hadd -f ${path}/histo_Egamma.periodB_flep.root ${path}/histo_Egamma.periodB_flep_*
    hadd -f ${path}/histo_Egamma.periodC_flep.root ${path}/histo_Egamma.periodC_flep_*
    hadd -f ${path}/histo_Egamma.periodD_flep.root ${path}/histo_Egamma.periodD_flep_*
    hadd -f ${path}/histo_Egamma.periodE_flep.root ${path}/histo_Egamma.periodE_flep_*
elif [ "$sample" == "MU" ] ; then
    hadd -f ${path}/histo_Muons.periodA_flep.root ${path}/histo_Muons.periodA_flep_*
    hadd -f ${path}/histo_Muons.periodB_flep.root ${path}/histo_Muons.periodB_flep_*
    hadd -f ${path}/histo_Muons.periodC_flep.root ${path}/histo_Muons.periodC_flep_*
    hadd -f ${path}/histo_Muons.periodD_flep.root ${path}/histo_Muons.periodD_flep_*
    hadd -f ${path}/histo_Muons.periodE_flep.root ${path}/histo_Muons.periodE_flep_*
elif [ "$sample" == "WW" ] ; then
    hadd -f ${path}/histo_llnunu_WW.126892_rlep.root ${path}/histo_llnunu_WW.126892_rlep_*
elif [ "$sample" == "TOP" ] ; then
    hadd -f ${path}/histo_TtbarLeptLept.117800_rlep.root ${path}/histo_TtbarLeptLept.117800_rlep_*
elif [ "$sample" == "ZEE" ] ; then
    hadd -f ${path}/histo_Zee.147770_rlep.root ${path}/histo_Zee.147770_rlep_*
elif [ "$sample" == "ZMM" ] ; then
    hadd -f ${path}/histo_Zmumu.147771_rlep.root ${path}/histo_Zmumu.147771_rlep_*
fi







