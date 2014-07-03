#!/bin/bash

#
# Submit SusyAna jobs 
# view queue 
#  qstat -r atlas -n
#  qstat |grep ataffard
# remove job: qdel <jobid>
# 
#
# ./submitT3_SusyAna <type> <pattern>
#     type=mc12 or data12
#     mode= STD or DD
# ./submitT3_SusyAna mc12 DD                // submit all MC
# ./submitT3_SusyAna data12                 // submit all data
# ./submitT3_SusyAna mc12 ttbar             // submit all mc ttbar
# ./submitT3_SusyAna data12 Egamma.periodB1 // submit all data: periodE muon Dil
#./submitT3_SusyAna.sh mc12 Mll10to60 DD    //to submit just the alpgen low mass
#
#
# ./submitT3_SusyAna.sh susy wA_noslep DD;  
# ./submitT3_SusyAna.sh susy wA_slep DD;  
# ./submitT3_SusyAna.sh susy wC_slep DD;  
# ./submitT3_SusyAna.sh susy WH DD;  
# ./submitT3_SusyAna.sh mc12 DD; 
# ./submitT3_SusyAna.sh data12 DD;

# Quick way to check all run
# grep "events processed"  jobLogs/*.log |wc -l
# output # = number of submitted jobs 
# 
# Submit low mass alpge w/ Mll cut - change anaOpt2 to true !!!!
#./submitT3_SusyAna.sh mc12 STD
#./submitT3_SusyAna.sh mc12 105200 STD


if [[ $# = 2 ]]; then
    type=$1
    pattern=$1
    mode=$2
    printf "No args, do all file from %s\n" ${type} 
else
    type=$1
    pattern=$2
    mode=$3
    printf "Do all file in %s with %s\n" ${type} ${pattern}
fi


###################################################################################
# SET ANALYSIS FLAGS
###################################################################################


#
# Which analysis to run
#
#anaOpt1=doAll
#anaOpt1=do2L
#anaOpt1=doWH
anaOpt1=doLFV
#anaOpt1=do3L
#anaOpt1=doFake

#
# Number of events to process
#
nEvt=-1

#
# Mll cut for Zjets - kind of obsolete
#
anaOpt2=true #doMll 
#anaOpt2=false #doMll 
methodMC=std

#
# Systematics
#   comment doSys to turn this off
# 
doSys=$FALSE
sys1="\"\""
sys2="\"\""


###################################################################################

pathRun=${WORKAREA}/SusyWeakProdAna/run
pathScript=${WORKAREA}/SusyWeakProdAna/scripts
ana=SusyAnaLooperExec

##
## Ana mode STD or DD
##
if [ "$mode" == "DD" ]; then
    methodMC=rlep
    #methodMC=std
    if [ $anaOpt1 == doFake ]; then
	methodMC=flep
    fi
fi

#
# Dataset Databse
#
dataset=${pathScript}/${type}_sampleList.txt

if [[ $anaOpt1 == doFake  &&  "$type" != "data12" ]]; then
    dataset=${pathScript}/${type}_sampleList_fake.txt
    echo "Using dataset list from " $dataset 
fi
#filter samples by pattern
cat ${dataset} | grep $pattern >tmp.txt

#
# Function - test if dataset is a large one
#
isLargeDataset(){
    local ss="$1"
    if [ "$ss" == "Zee.147770" -o\
 "$ss" == "Zmumu.147771" -o\
 "$ss" == "llnunu_WW.126892" -o\
 "$ss" == "ZeeNp0Excl.178369" -o\
 "$ss" == "ZeeNp0.117650" -o\
 "$ss" == "ZmumuNp0.117660" -o\
 "$ss" == "llll_ZZ.126894" -o\
 "$ss" == "ZeeNp1.117651" -o\
 "$ss" == "ZmumuNp1.117661" -o\
 "$ss" == "DYeeM15to40.173042" -o\
 "$ss" == "DYmumuM08to15.173043" -o\
 "$ss" == "DYmumuM15to40.173044" -o\
 "$ss" == "llnunu_WW_MassiveCB.177997" -o\
 "$ss" == "ttbar.105200" -o\
 "$ss" == "TtbarLeptLept.117800" -o\
 "$ss" == "ttbar.110001" -o\
 "$ss" == "ttbar.117209" -o\
 "$ss" == "ttbar.117210" -o\
 "$ss" == "ZeeNp0.147105" -o\
 "$ss" == "ZeeNp1.147106" -o\
 "$ss" == "ZmumuNp0.147113" -o\
 "$ss" == "ZmumuNp1.147114" -o\
 "$ss" == "ZeeHeavyJets.128975" -o\
 "$ss" == "ZmumuHeavyJets.128976"  -o\
 "$ss" == "ZeeNp0.107650" -o\
 "$ss" == "ZmumuNp0.107660" -o\
 "$ss" == "ttbar.105861" -o\
 "$ss" == "ttbar.105860" -o\
 "$ss" == "Zee.147806" -o\
 "$ss" == "Zmumu.147807" -o\
 "$ss" == "ZeeLightJets.146820" -o\
 "$ss" == "ZmumuLightJets.146821" \
	]; then
	return $TRUE
    else 
	return $FALSE
    fi
}


#
#Export vars for sbatch
#
export ana=${WORKAREA}/SusyWeakProdAna/run/$ana
export anaOpt1=$anaOpt1
export anaOpt2=$anaOpt2
export nEvt=$nEvt


#######################################################################
#   Submission loop
#######################################################################
while read line; do
    #put each line into array
    sample=(`echo ${line} | tr '\t' ','`)
    sName=${sample[0]}
    sDir=${sample[1]}
    export name=$sName
    export fileDir=$sDir
    if [[ $sName == *#* ]]; then
	echo "Sample  $sName is commented out - Skippping "
    else
	echo
	date
	echo "Submitting job $ana"
	echo "  name:       $sName"
	echo "  fileDir:    $sDir"
	echo "  nEvt:       $nEvt"
	echo "  anaOpt1:    $anaOpt1"
	echo "  doMll:      $anaOpt2"
	echo 
	
	#
	#  --- MC ---- 
	#
 	if [ "$type" == "mc12"  -o "$type" == "susy" ]; then
	    export method=$methodMC
	    #
            # Submit MC with systematics
            #
	    isLarge=$(isLargeDataset "$sName")
	    if [[ $doSys ]] && [[ isLarge ]]; then
		echo "Large dataset - splitting systematics"
		
		SYS1=("NOM"      "EES_Z_DN"    "EES_MAT_DN" "EES_PS_DN"  "EES_LOW_DN"  "EER_DN" "MS_DN" "ID_DN" "JES_DN" "SCALEST_UP" 
		    "TRIGSF_EL_UP" "TRIGSF_MU_UP" "TES_UP" "BTag_BJet_DN"  "BTag_LJet_DN" "BTag_CJet_UP" "BKGMETHOD_UP" "XS_UP" "GEN")
		SYS2=("EES_Z_UP" "EES_MAT_UP"  "EES_PS_UP"  "EES_LOW_UP" "EER_UP"      "MS_UP"  "ID_UP" "JES_UP" "JER"   "RESOST"     
		    "TRIGSF_EL_DN" "TRIGSF_MU_DN" "TES_DN" "BTag_CJet_DN" "BTag_BJet_UP" "BTag_LJet_UP" "BKGMETHOD_DN" "XS_DN" "Pileup_DN")
		
		echo "Submitting MC using Sys Range "
		index=0
		for sys1 in ${SYS1[@]}; do
		    sys2=${SYS2[$index]}
		    export sys1=$sys1
		    export sys2=$sys2
		    echo "Submitting MC "
		    echo "  sys $sys1 --> $sys2" 
		    echo "  method:     $methodMC"
		    cd ${pathRun}
		    suff="_"${sys1}"-"${sys2}
		    sbatch --job-name=$name --output=${pathRun}/batchLogs/${name}_${method}${suff}.log --error=${pathRun}/batchLogs/${name}_${method}${suff}.log ${pathScript}/batchSubmit_wSys.sh "ana=$ana,sys1=$sys1,sys2=$sys2,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodMC,nEvt=$nEvt,name=$sName,fileDir=$sDir" 
		    echo ""
		    echo "sbatch --job-name=$name --output=${pathRun}/batchLogs/${name}_${method}.log --error=${pathRun}/batchLogs/${name}_${method}.log\
 ${pathScript}/batchSubmit_wSys.sh " ana=$ana,sys1=$sys1,sys2=$sys2,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodMC,nEvt=$nEvt,name=$sName,fileDir=$sDir""
		    cd ${pathScript}
		    echo ""
		    sleep 1
		    
		    ((index++))
		done
	    else 
		echo "Submitting MC (no Sys and all Sys)"
		echo "  method:     $methodMC"
		cd ${pathRun}
		sbatch --job-name=$name --output=${pathRun}/batchLogs/${name}_${method}.log --error=${pathRun}/batchLogs/${name}_${method}.log ${pathScript}/batchSubmit.sh "ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodMC,nEvt=$nEvt,name=$sName,fileDir=$sDir" 
		echo ""
		echo "sbatch --job-name=$name --output=${pathRun}/batchLogs/${name}_${method}.log --error=${pathRun}/batchLogs/${name}_${method}.log\
 ${pathScript}/batchSubmit.sh "ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodMC,nEvt=$nEvt,name=$sName,fileDir=$sDir" "
		cd ${pathScript}
		echo $PWD
		echo ""
		sleep 1
	    fi
	fi
	
	#
	#  --- DATA ---- 
	#
	if [ "$type" == "data12" ]; then
	    if [ $anaOpt1 != doFake ]; then
		echo "Submitting data STD "
		methodData=std
		export method=$methodData
		cd ${pathRun}
		sbatch --job-name=$name --output=${pathRun}/batchLogs/${name}_${method}.log --error=${pathRun}/batchLogs/${name}_${method}.log ${pathScript}/batchSubmit.sh "ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir" 
		echo ""
		echo "sbatch --job-name=$name --output=${pathRun}/batchLogs/${name}_${method}.log --error=${pathRun}/batchLogs/${name}_${method}.log\
 ${pathScript}/batchSubmit.sh "ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir" "
		cd ${pathScript}
		echo ""
		sleep 1
	    fi
	    
	    if [ "$mode" == "DD" ]; then
		if [ $doSys ]; then
		    echo "Submitting data DD fake "
		    SYS1=("NOM" "FAKE_EL_RE_UP" "FAKE_EL_FR_UP" "FAKE_MU_RE_UP" "FAKE_MU_FR_UP")
		    SYS2=("NOM" "FAKE_EL_RE_DN" "FAKE_EL_FR_DN" "FAKE_MU_RE_DN" "FAKE_MU_FR_DN")
		    methodData=flep
		    export method=$methodData
		    
		    echo "Submitting Data fake using Sys Range "
		    index=0
		    for sys1 in ${SYS1[@]}; do
			sys2=${SYS2[$index]}
			echo "Submitting DATA FAKE "
			echo "  sys $sys1 --> $sys2" 
			echo "  method:     $methodData"
			
			cd ${pathRun}
			sbatch --job-name=$name --output=${pathRun}/batchLogs/${name}_${method}.log --error=${pathRun}/batchLogs/${name}_${method}.log ${pathScript}/batchSubmit_wSys.sh "ana=$ana,sys1=$sys1,sys2=$sys2,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir" 
			echo ""
			echo "sbatch --job-name=$name --output=${pathRun}/batchLogs/${name}_${method}.log --error=${pathRun}/batchLogs/${name}_${method}.log\
 ${pathScript}/batchSubmit.sh "ana=$ana,sys1=$sys1,sys2=$sys2,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir" "
			cd ${pathScript}
			echo ""
			sleep 1
			((index++))
		    done  
		else
		    methodData=flep
		    export method=$methodData
		    echo "Submitting DATA FAKE "
		    echo "  method:     $methodData"
		    
		    cd ${pathRun}
		    sbatch --job-name=$name --output=${pathRun}/batchLogs/${name}_${method}.log --error=${pathRun}/batchLogs/${name}_${method}.log ${pathScript}/batchSubmit.sh "ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir" 
		    echo ""
		    echo "sbatch --job-name=$name --output=${pathRun}/batchLogs/${name}_${method}.log --error=${pathRun}/batchLogs/${name}_${method}.log\
 ${pathScript}/batchSubmit.sh "ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir" "
		    cd ${pathScript}
		    echo ""
		    sleep 1
		fi
	    fi
	fi

	
	#
	#  --- DEBUG ---- 
	#	
	if [ "$type" == "debug" ]; then
	    echo "Submitting debug STD "
	    methodData=std
	    export method=$methodData
	    cd ${pathRun}
	    sbatch --job-name=$name --output=${pathRun}/batchLogs/${name}_${method}.log --error=${pathRun}/batchLogs/${name}_${method}.log ${pathScript}/batchSubmit.sh "ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir" 
	    echo ""
	    echo "sbatch --job-name=$name --output=${pathRun}/batchLogs/${name}_${method}.log --error=${pathRun}/batchLogs/${name}_${method}.log\
 ${pathScript}/batchSubmit.sh "ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir" "
	    cd ${pathScript}
	    echo ""
	    sleep 1
	fi
    fi
    
done < tmp.txt
		
squeue -l |grep ataffard
rm -f tmp.txt
		
if [ "$type" == "mc12" ]; then
    cd  ${pathRun}
#    ./run3L.sh mc12 dummy ${mode}
#    ./runWH.sh mc12 dummy ${mode}

#    ./run.sh mc12 dummy ${mode}
##    ./run.sh susy 166614 ${mode}
#    ./run.sh susy 175447 ${mode}
#    ./run.sh susy 175510 ${mode}
##    ./run.sh susy 175517 ${mode}
##    ./run.sh susy 175580 ${mode}
##    ./run.sh susy 175581 ${mode}
#    ./run.sh susy 144917 ${mode}
##    ./run.sh susy 144921 ${mode}
#    ./run.sh susy 144922 ${mode}
##    ./run.sh susy 144923 ${mode}
##    ./run.sh susy 176322 ${mode}
##    ./run.sh susy 176325 ${mode}
##    ./run.sh susy 176480 ${mode}

#    ./run.sh susy 177502 ${mode}
#    ./run.sh susy 177503 ${mode}
#    ./run.sh susy 177506 ${mode}
#    ./run.sh susy 177508 ${mode}
#    ./run.sh susy 177509 ${mode}
#    ./run.sh susy 177510 ${mode}
#    ./run.sh susy 177512 ${mode}
#    ./run.sh susy 177513 ${mode}
#    ./run.sh susy 177514 ${mode}
#    ./run.sh susy 177517 ${mode}
#    ./run.sh susy 177521 ${mode}
#    ./run.sh susy 177522 ${mode}
#    ./run.sh susy 177523 ${mode}
#    ./run.sh susy 177524 ${mode}
#    ./run.sh susy 177525 ${mode}
#    ./run.sh susy 177527 ${mode}

fi



