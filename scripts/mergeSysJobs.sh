#!/bin/bash

if [[ $# = 1 ]]; then
    sample=$1
fi

#path=${WORKAREA}/histoAna/SusyAna/histos_112812_13fb_n0111_DD_MMtrial9_SYS
path=${WORKAREA}/histoAna/SusyAna/

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

elif [ "$sample" == "TOP105200" ] ; then
    hadd -f ${path}/histo_ttbar.105200_rlep.root ${path}/histo_ttbar.105200_rlep_*
elif [ "$sample" == "TOP117210" ] ; then
    hadd -f ${path}/histo_ttbar.117210_rlep.root ${path}/histo_ttbar.117210_rlep_*
elif [ "$sample" == "ZEEHF" ] ; then
    hadd -f ${path}/histo_ZeeHeavyJets.128975_rlep.root ${path}/histo_ZeeHeavyJets.128975_rlep_*
elif [ "$sample" == "ZMMHF" ] ; then
    hadd -f ${path}/histo_ZmumuHeavyJets.128976_rlep.root ${path}/histo_ZmumuHeavyJets.128976_rlep_*
elif [ "$sample" == "ZEENP0" ] ; then
    hadd -f ${path}/histo_ZeeNp0.117650_rlep.root ${path}/histo_ZeeNp0.117650_rlep_*
    hadd -f ${path}/histo_ZeeNp0.107650_rlep.root ${path}/histo_ZeeNp0.107650_rlep_*
elif [ "$sample" == "ZMMNP0" ] ; then
    hadd -f ${path}/histo_ZmumuNp0.107660_rlep.root ${path}/histo_ZmumuNp0.107660_rlep_*
    hadd -f ${path}/histo_ZmumuNp0.117660_rlep.root ${path}/histo_ZmumuNp0.117660_rlep_*

fi



#
# Add other MC 
#



