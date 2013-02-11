#
# Submit SusyAna jobs 
# view queue 
#  qstat -r atlas -n
#  qstat |grep ataffard
# remove job: qdel <jobid>
# 
#
# ./submitT3_SusyAna <type> <pattern>
#     type=mc or data
#     mode= STD or DD
# ./submitT3_SusyAna mc12                   // submit all MC
# ./submitT3_SusyAna data12                 // submit all data
# ./submitT3_SusyAna mc12 ttbar             // submit all mc ttbar
# ./submitT3_SusyAna data12 Egamma.periodB1 // submit all data: periodE muon Dil
#./submitT3_SusyAna.sh mc12 Mll10to60 DD    //to submit just the alpgen low mass
#
#
# Quick way to check all run
# grep "events processed"  jobLogs/*.log |wc -l
# output # = number of submitted jobs 
# 
# Submit low mass alpge w/ Mll cut - change anaOpt2 to true !!!!
#./submitT3_SusyAna.sh mc12 STD
#./submitT3_SusyAna.sh mc12 105200 STD

#!/bin/bash


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

pathRun=${WORKAREA}/SusyWeakProdAna/run
pathScript=${WORKAREA}/SusyWeakProdAna/scripts
ana=SusyAnaLooperExec
#anaOpt1=doAll
anaOpt1=do2L
#anaOpt1=doFake
#anaOpt2=true #doMll 
anaOpt2=false #doMll 
methodMC=std
sys1="\"\""
sys2="\"\""
nEvt=-1

#doSys=true

##
## Ana mode STD or DD
##
if [ "$mode" == "DD" ]; then
    methodMC=rlep
#    methodMC=std
fi



dataset=${pathScript}/${type}_sampleList.txt

#filter samples by pattern
cat ${dataset} | grep $pattern >tmp.txt

while read line; do
    #echo $line
    #put each line into array
    sample=(`echo ${line} | tr '\t' ','`)
    sName=${sample[0]}
    sDir=${sample[1]}
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

	if [ "$type" == "mc12" -o "$type" == "susy" ]; then
	    if [ $doSys ]; then
	        #For large sample - split the sys
		if [ "$sName" == "Zee.147770" -o \
"$sName" == "Zmumu.147771" -o \
"$sName" == "TtbarLeptLept.117800" -o \
"$sName" == "llnunu_WW.126892" -o \
"$sName" == "ttbar.117209" -o \
"$sName" == "ttbar.117210" -o \
"$sName" == "ZeeNp0.117650" -o \
"$sName" == "ZmumuNp0.117660" -o \
"$sName" == "ZeeHeavyJets.128975" -o \
"$sName" == "ZmumuHeavyJets.128976"  -o \
"$sName" == "ttbar.105200" -o \
"$sName" == "ttbar.105861" -o \
"$sName" == "Zee.147806" -o \
"$sName" == "Zmumu.147807" \
]; then
		    SYS1=("NOM"      "EES_Z_DN"    "EES_MAT_DN" "EES_PS_DN"  "EES_LOW_DN"  "EER_DN" "MS_DN" "ID_DN" "JES_DN" "SCALEST_UP" \
"TRIGSF_EL_UP" "TRIGSF_MU_UP" "BTag_BJet_DN"  "BTag_LJet_DN" "BTag_CJet_UP" "BKGMETHOD_UP" "XS_UP")
		    SYS2=("EES_Z_UP" "EES_MAT_UP"  "EES_PS_UP"  "EES_LOW_UP" "EER_UP"      "MS_UP"  "ID_UP" "JES_UP" "JER"   "RESOST"     \
"TRIGSF_EL_DN" "TRIGSF_MU_DN"  "BTag_CJet_DN" "BTag_BJet_UP" "BTag_LJet_UP" "BKGMETHOD_DN" "XS_DN" )
		    
		    echo "Submitting MC using Sys Range "
		    index=0
		    for sys1 in ${SYS1[@]}; do
			sys2=${SYS2[$index]}
			echo "Submitting MC "
			echo "  sys $sys1 --> $sys2" 
			echo "  method:     $methodMC"
      	                    # run the job - submit with qsub
			cd ${pathRun}
			qsub -j oe -V -v ana=$ana,sys1=$sys1,sys2=$sys2,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodMC,nEvt=$nEvt,name=$sName,fileDir=$sDir -N $sName -o ${pathRun}/batchLogs ${pathScript}/batchSubmit_wSys.sh
			echo ""
			echo "qsub -j oe -V -v ana=$ana,sys1=$sys1,sys2=$sys2,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodMC,nEvt=$nEvt,name=$sName,fileDir=$sDir -N $sName -o ${pathRun}/batchLogs ${pathScript}/batchSubmit_wSys.sh "
			cd ${pathScript}
			echo ""
			sleep 1
			    
			((index++))
		    done
		else
		    echo "Submitting MC "
		    echo "  method:     $methodMC"
 	            # run the job - submit with qsub
		    cd ${pathRun}
		    qsub -j oe -V -v ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodMC,nEvt=$nEvt,name=$sName,fileDir=$sDir -N $sName -o ${pathRun}/batchLogs ${pathScript}/batchSubmit.sh
		    echo ""
		    echo "qsub -j oe -V -v ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodMC,nEvt=$nEvt,name=$sName,fileDir=$sDir -N $sName -o ${pathRun}/batchLogs ${pathScript}/batchSubmit.sh "
		    cd ${pathScript}
		    echo ""
		    sleep 1  
		fi

	    else 
		echo "Submitting MC "
		echo "  method:     $methodMC"
 	        # run the job - submit with qsub
		cd ${pathRun}
		qsub -j oe -V -v ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodMC,nEvt=$nEvt,name=$sName,fileDir=$sDir -N $sName -o ${pathRun}/batchLogs ${pathScript}/batchSubmit.sh
		echo ""
		echo "qsub -j oe -V -v ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodMC,nEvt=$nEvt,name=$sName,fileDir=$sDir -N $sName -o ${pathRun}/batchLogs ${pathScript}/batchSubmit.sh "
		cd ${pathScript}
		echo ""
		sleep 1
	    fi
	fi

	if [ "$type" == "data12" ]; then
	    echo "Submitting data STD "
	    methodData=std
	    cd ${pathRun}
	    qsub -j oe -V -v ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir -N $sName -o ${pathRun}/batchLogs ${pathScript}/batchSubmit.sh
	    echo ""
	    echo "qsub -j oe -V -v ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir -N $sName -o ${pathRun}/batchLogs ${pathScript}/batchSubmit.sh "
	    cd ${pathScript}
	    echo ""
	    sleep 1

	    if [ "$mode" == "DD" ]; then
		if [ $doSys ]; then
		    echo "Submitting data DD fake "
		    SYS1=("NOM" "FAKE_EL_RE_UP" "FAKE_EL_FR_UP" "FAKE_MU_RE_UP" "FAKE_MU_FR_UP")
		    SYS2=("NOM" "FAKE_EL_RE_DN" "FAKE_EL_FR_DN" "FAKE_MU_RE_DN" "FAKE_MU_FR_DN")
		    methodData=flep
		    
		    echo "Submitting Data fake using Sys Range "
		    index=0
		    for sys1 in ${SYS1[@]}; do
			sys2=${SYS2[$index]}
			echo "Submitting DATA FAKE "
			echo "  sys $sys1 --> $sys2" 
			echo "  method:     $methodData"
			
			cd ${pathRun}
			qsub -j oe -V -v ana=$ana,sys1=$sys1,sys2=$sys2,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir -N $sName -o ${pathRun}/batchLogs ${pathScript}/batchSubmit_wSys.sh
			echo ""
			echo "qsub -j oe -V -v ana=$ana,sys1=$sys1,sys2=$sys2,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir -N $sName -o ${pathRun}/batchLogs ${pathScript}/batchSubmit_wSys.sh "
			cd ${pathScript}
			echo ""
			sleep 1
			((index++))
		    done  
		    
		else
		    methodData=flep
		    echo "Submitting DATA FAKE "
		    echo "  method:     $methodData"
		    
		    cd ${pathRun}
		    qsub -j oe -V -v ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir -N $sName -o ${pathRun}/batchLogs ${pathScript}/batchSubmit.sh
		    echo ""
		    echo "qsub -j oe -V -v ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir -N $sName -o ${pathRun}/batchLogs ${pathScript}/batchSubmit.sh "
		    cd ${pathScript}
		    echo ""
		    sleep 1
		    
		fi
		
	    fi
	fi

	if [ "$type" == "debug" ]; then
	    echo "Submitting debug STD "
	    methodData=std
	    cd ${pathRun}
	    qsub -j oe -V -v ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir -N $sName -o ${pathRun}/batchLogs ${pathScript}/batchSubmit.sh
	    echo ""
	    echo "qsub -j oe -V -v ana=$ana,anaOpt1=$anaOpt1,anaOpt2=$anaOpt2,method=$methodData,nEvt=$nEvt,name=$sName,fileDir=$sDir -N $sName -o ${pathRun}/batchLogs ${pathScript}/batchSubmit.sh "
	    cd ${pathScript}
	    echo ""
	    sleep 1

	fi


    fi

done <tmp.txt

qstat |grep ataffard
rm -f tmp.txt

#if [ "$type" == "mc12" ]; then
#    cd  ${pathRun}
#    ./run.sh mc12 dummy ${mode}
#fi
