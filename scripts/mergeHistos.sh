#!/bin/bash

if [[ $# = 2 ]]; then
    job=$1
    mode=$2
fi


if [ "$job" == "DATA"  -a  "$mode" ==  "ALL" ] ; then
    MergeJobsScripts/data12_fake_job.sh
#    MergeJobsScripts/data12_std_job.sh


elif [ "$job" == "TOP" -a  "$mode" ==  "ALL" ] ; then
    MergeJobsScripts/top_MCNLO_${mth}_job.sh
    MergeJobsScripts/top_PowHeg_${mth}_job.sh
    MergeJobsScripts/top_Sherpa_${mth}_job.sh
    MergeJobsScripts/topDil_Sherpa_${mth}_job.sh

elif [ "$job" == "WW"  -a  "$mode" ==  "ALL" ] ; then
    MergeJobsScripts/WW_Powheg_${mth}_job.sh
    MergeJobsScripts/WW_Sherpa_${mth}_job.sh

elif [ "$job" == "DIB"  -a  "$mode" ==  "ALL" ] ; then
    MergeJobsScripts/diBZX_Sherpa_${mth}_job.sh
    MergeJobsScripts/diBoson_Sherpa_${mth}_job.sh

elif [ "$job" == "ZJ"  -a  "$mode" ==  "ALL" ] ; then
    MergeJobsScripts/Zjets_Alpgen_${mth}_job.sh
    MergeJobsScripts/Zjets_SherpaLFHF_${mth}_job.sh
    MergeJobsScripts/Zjets_Sherpa_${mth}_job.sh

elif [ "$job" == "ZTT"  -a  "$mode" ==  "ALL" ] ; then
    MergeJobsScripts/ZTauTaujets_Alpgen_${mth}_job.sh
    MergeJobsScripts/ZTauTaujets_SherpaLFHF_${mth}_job.sh
    MergeJobsScripts/ZTauTaujets_Sherpa_${mth}_job.sh

elif [ "$job" == "WJ"  -a  "$mode" ==  "ALL" ] ; then
    MergeJobsScripts/Wjets_Alpgen_${mth}_job.sh
    MergeJobsScripts/Wjets_Sherpa_${mth}_job.sh


elif [ "$job" == "MODA"  -a  "$mode" ==  "SUSY" ] ; then
    MergeJobsScripts/SimplifiedModel_wA_noslep_lepW_${mth}_job.sh
    MergeJobsScripts/SimplifiedModel_wA_slep_${mth}_job.sh
elif [ "$job" == "MODC"  -a  "$mode" ==  "SUSY" ] ; then
    MergeJobsScripts/SimplifiedModel_wC_noslep_lepW_${mth}_job.sh
    MergeJobsScripts/SimplifiedModel_wC_slep_${mth}_job.sh
elif [ "$job" == "PMSSM"  -a  "$mode" ==  "SUSY" ] ; then
    MergeJobsScripts/pMSSM_DGemtR50_TB6_${mth}_job.sh
    MergeJobsScripts/pMSSM_DGstauR_TB50_${mth}_job.sh


elif [ "$job" == "FAKE"  -a  "$mode" ==  "MERGE" ] ; then
    MergeJobsScripts/mcFake_Alpgen_${mth}_job.sh
    MergeJobsScripts/mcFake_Sherpa_${mth}_job.sh
elif [ "$job" == "ZX"  -a  "$mode" ==  "MERGE" ] ; then
    MergeJobsScripts/ZX_SherpaLFHF_${mth}_job.sh
    MergeJobsScripts/ZX_Sherpa_${mth}_job.sh
fi




#---
#---
#---
#--- Must be run once all the other are done

