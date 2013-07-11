#
# Make the histograms to run the SROptimization code
# Make sure to run 
#    make the ToyNt changing the selection in SusyAnaCommon
#    submitT3_hadd.sh mc rlep toyNtMerger  (merge the toyNt)
#
# Update the Dir in ToyNtAnaExec
# Run  
# ./SROptimExec -d 1 -r 2 -p 3 -sig -tabB -tabS >> jobLogs/SROptim_Andreas_wAnoslep.txt
#
#
#
#

#!/bin/bash

date="061413_21fb_n0140_Optim_DD_v3"

pathScript=${WORKAREA}/SusyWeakProdAna/scripts
pathRun=${WORKAREA}/SusyWeakProdAna/run
inToyNtPath=${HISTOANA}/SusyAna/histos_${date}/ToyNtOutputs
#toyNt_SR="DIL_optimSRjets"
toyNt_SR="DIL_optimSRSS"


cd ${pathRun}

if [[ $# = 2 ]]; then
    mode=$1
    gp=$2
elif [[ $# = 1 ]]; then
    mode=$1
fi

if [ "$mode" == "BKG" ]; then
    ./ToyNtAnaExec -f toyNtLists/Zjets.txt  -s Zjets
    ./ToyNtAnaExec -f toyNtLists/top.txt  -s top
    ./ToyNtAnaExec -f toyNtLists/WW.txt  -s WW
    ./ToyNtAnaExec -f toyNtLists/ZV.txt  -s ZV
    ./ToyNtAnaExec -f toyNtLists/higgs.txt  -s higgs
    ./ToyNtAnaExec -f toyNtLists/fake.txt  -s fake
    ./ToyNtAnaExec -f toyNtLists/allBkg.txt  -s allBkg

elif  [ "$mode" == "wA_noslep_hadW" ]; then
    while read line; do
	ss=(`echo ${line} | tr '\t' ',' |cut -d'.' -f2-2 `)
	name=${ss}"_"${toyNt_SR}".root"
	echo ">>> Submitting $name"
	./ToyNtAnaExec -F ${inToyNtPath}/${name}  -s wA_noslep_hadW_${ss}

	echo ""
	echo ""
    done < ${pathScript}/inputHistoList/wA_noslep_hadW.txt

elif  [ "$mode" == "wA_noslep_WH_2Lep" ]; then
    while read line; do
	ss=(`echo ${line} | tr '\t' ',' |cut -d'.' -f2-2 `)
	name=${ss}"_"${toyNt_SR}".root"
	echo ">>> Submitting $name"
	./ToyNtAnaExec -F ${inToyNtPath}/${name}  -s wA_noslep_WH_2Lep_${ss}

	echo ""
	echo ""
    done < ${pathScript}/inputHistoList/wA_noslep_WH_2Lep.txt

elif  [ "$mode" == "wA_slep" ]; then
    while read line; do
	ss=(`echo ${line} | tr '\t' ',' |cut -d'.' -f2-2 `)
	name=${ss}"_"${toyNt_SR}".root"
	echo ">>> Submitting $name"
	./ToyNtAnaExec -F ${inToyNtPath}/${name}  -s wA_slep_${ss}

	echo ""
	echo ""
    done < ${pathScript}/inputHistoList/wA_slep.txt


fi