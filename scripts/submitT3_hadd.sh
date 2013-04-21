#
#
# check log for errors:
#          grep -i error hadd/*.log |grep -i cannot |cut -d" " -f1-1
#
#

#
#
# ./submitT3_hadd.sh cat
#
# ./submitT3_hadd.sh pass + mv histOutputs
#
# ./submitT3_hadd.sh mc rlep hadd; ./submitT3_hadd.sh data std hadd; ./submitT3_hadd.sh data flep hadd; 
#
# ./submitT3_hadd.sh mc rlep merge; ./submitT3_hadd.sh data std merge; ./submitT3_hadd.sh data flep merge; 
#
#
#
#!/bin/bash

#Update for a given pass
date="041813_21fb_n0139_Moriond_DD_v3"
#date="041413_21fb_n0139_Moriond_DD_v2"
#date="040613_21fb_n0138_Moriond_DD_v1"
#date="033113_21fb_n0135_Moriond_DD_v6"
#date="032913_21fb_n0135_Moriond_DD_v5"
#date="032813_21fb_n0135_Moriond_DD_v4"
#date="031513_21fb_n0135_Moriond_DD_v3"
#date="031313_21fb_n0135_Moriond_DD_v1"
#date="030813_21fb_n0127_Moriond_DD_v8"
#date="030613_21fb_n0127_Moriond_DD_v7"
#date="030213_21fb_n0135_Moriond_DD_v7"


pathScript=${WORKAREA}/SusyWeakProdAna/scripts
pathRun=${WORKAREA}/SusyWeakProdAna/run

inlist=${WORKAREA}/SusyWeakProdAna/scripts/inputHistoList
dir=${WORKAREA}/SusyWeakProdAna/scripts/haddJobs

#inpath=${WORKAREA}/histoAna/SusyAna/histOutputs
#outpath=${WORKAREA}/histoAna/SusyAna/hadd_histOutputs
#endpath=${WORKAREA}/histoAna/SusyAna

inpath=${HISTOANA}/SusyAna/histos_${date}/histOutputs
outpath=${HISTOANA}/SusyAna/histos_${date}/hadd_histOutputs
endpath=${HISTOANA}/SusyAna/histos_${date}

passDir=${HISTOANA}/SusyAna/histos_${date}


if [[ $# = 3 ]]; then
    type=$1
    mth=$2
    mode=$3
    echo "Request ${type} method: ${mth}"
elif [[ $# = 1 ]]; then
    mode=$1

    if [ "$mode" == "cat" ]; then
	echo "Remerging Zjets, ZX & Bkg  grouping"

	#Various Z+jets
	#cat ${inlist}/Zjets_AlpgenPythia.txt ${inlist}/WZ_ZZ_Sherpa.txt > ${inlist}/ZX_AlpgenPythia_WZ_ZZ_Sherpa.txt
	#cat ${inlist}/Zjets_AlpgenPythia.txt ${inlist}/WZ_ZZ_PowHeg.txt > ${inlist}/ZX_AlpgenPythia_WZ_ZZ_PowHeg.txt

	#cat ${inlist}/Zjets_Sherpa.txt ${inlist}/WZ_ZZ_Sherpa.txt > ${inlist}/ZX_Sherpa_WZ_ZZ_Sherpa.txt
	#cat ${inlist}/Zjets_Sherpa.txt ${inlist}/WZ_ZZ_PowHeg.txt > ${inlist}/ZX_Sherpa_WZ_ZZ_PowHeg.txt

	#cat ${inlist}/Zjets_SherpaLFHF.txt ${inlist}/WZ_ZZ_Sherpa.txt > ${inlist}/ZX_SherpaLFHF_WZ_ZZ_Sherpa.txt
	

	#All Bkg grouping - variations
	cat ${inlist}/Zjets_SherpaAlpgenPythia.txt ${inlist}/WZ_ZZ_Sherpa.txt  ${inlist}/WW_Sherpa.txt \
	    ${inlist}/top_MCNLO.txt ${inlist}/Higgs.txt > ${inlist}/Bkg_Zjets_SherpaAlpgen_WZ_ZZ_Sherpa_WW_Sherpa_TopMCNLO.txt

	cat ${inlist}/Zjets_SherpaAlpgenPythia.txt ${inlist}/WZ_ZZ_PowHeg.txt  ${inlist}/WW_PowHeg.txt \
	    ${inlist}/top_MCNLO.txt ${inlist}/Higgs.txt > ${inlist}/Bkg_Zjets_SherpaAlpgen_WZ_ZZ_PowHeg_WW_PowHeg_TopMCNLO.txt

	#cat ${inlist}/ZX_AlpgenPythia_WZ_ZZ_Sherpa.txt ${inlist}/top_MCNLO.txt ${inlist}/WW_Sherpa.txt \
	#  ${inlist}/ZTauTaujets_AlpgenPythia.txt ${inlist}/Higgs.txt > ${inlist}/Bkg_ZXAlpgen_WZ_ZZ_Sherpa_WW_Sherpa_TopMCNLO.txt

	#cat ${inlist}/ZX_AlpgenPythia_WZ_ZZ_PowHeg.txt ${inlist}/top_MCNLO.txt ${inlist}/WW_PowHeg.txt \
	#    ${inlist}/ZTauTaujets_AlpgenPythia.txt ${inlist}/Higgs.txt > ${inlist}/Bkg_ZXAlpgen_WZ_ZZ_PowHeg_WW_PowHeg_TopMCNLO.txt


	#cat ${inlist}/ZX_AlpgenPythia_WZ_ZZ_Sherpa.txt ${inlist}/top_Alpgen.txt ${inlist}/WW_Sherpa.txt \
	#    ${inlist}/ZTauTaujets_AlpgenPythia.txt ${inlist}/Higgs.txt > ${inlist}/Bkg_ZXAlpgen_diB_Sherpa_TopAlpgen.txt
	#cat ${inlist}/ZX_AlpgenPythia_WZ_ZZ_Sherpa.txt ${inlist}/top_PowHeg.txt ${inlist}/WW_Sherpa.txt \
	#    ${inlist}/ZTauTaujets_AlpgenPythia.txt ${inlist}/Higgs.txt > ${inlist}/Bkg_ZXAlpgen_diB_Sherpa_TopPowHeg.txt


#	cat ${inlist}/ZX_Sherpa.txt ${inlist}/top_MCNLO.txt ${inlist}/WW_Sherpa.txt \
#	    ${inlist}/ZTauTaujets_Sherpa.txt ${inlist}/Higgs.txt > ${inlist}/Bkg_ZXSherpa_TopMCNLO.txt

#	cat ${inlist}/ZX_Sherpa.txt ${inlist}/top_Alpgen.txt ${inlist}/WW_Sherpa.txt \
#	    ${inlist}/ZTauTaujets_Sherpa.txt ${inlist}/Higgs.txt > ${inlist}/Bkg_ZXSherpa_TopAlpgen.txt
#	cat ${inlist}/ZX_Sherpa.txt ${inlist}/top_PowHeg.txt ${inlist}/WW_Sherpa.txt \
#	    ${inlist}/ZTauTaujets_Sherpa.txt ${inlist}/Higgs.txt > ${inlist}/Bkg_ZXSherpa_TopPowHeg.txt

#	cat ${inlist}/ZX_AlpgenPythia.txt ${inlist}/top_MCNLO.txt ${inlist}/WW_PowHeg.txt \
#	    ${inlist}/ZTauTaujets_AlpgenPythia.txt ${inlist}/Higgs.txt >${inlist}/Bkg_ZXAlpgen_TopMCNLO_WWPowHeg.txt

#	cat ${inlist}/ZX_AlpgenPythia_diBPowHeg.txt ${inlist}/top_MCNLO.txt ${inlist}/WW_PowHeg.txt \
#	    ${inlist}/ZTauTaujets_AlpgenPythia.txt ${inlist}/Higgs.txt >${inlist}/Bkg_ZXAlpgen_diBPowHeg_TopMCNLO_WWPowHeg.txt


    elif  [ "$mode" == "pass" ]; then
	endpath=${HISTOANA}/SusyAna

	if [ ! -d "${passDir}" ]; then
	    mkdir ${passDir}
	fi
	mkdir -p ${passDir}/logs
	mkdir -p ${passDir}/logs/hadd
	mkdir -p ${passDir}/logs/jobs

	mv ${pathRun}/jobLogs/*hadd.log ${passDir}/logs/hadd
	mv ${pathRun}/jobLogs/*.log ${passDir}/logs/jobs
	mv ${endpath}/HFTOutputs ${passDir}
	mv ${endpath}/ToyNtOutputs ${passDir}
	mv ${endpath}/hadd_histOutputs ${passDir}
	mv ${endpath}/*.root ${passDir}
	mv ${endpath}/histOutputs ${passDir}

    elif  [ "$mode" == "move" ]; then

	inDir=${HISTOANA}/SusyAna/histOutputs
	#inDir=/data7/atlas/ataffard/SUSYAna_8TeV2012ALL/histoAna/SusyAna/histos_030213_21fb_n0135_Moriond_DD_v7/histOutputs/
	outDir=${HISTOANA}/SusyAna/histos_${date}/histOutputs
	
	ls -1 ${inDir} >tmp.txt
	
	while read line; do 
	    var=(`echo ${line} | tr '\t' ','`)
	    subdir=${var[0]}
	    mv ${inDir}/${subdir}/* ${outDir}/${subdir}
	    #echo "cp ${inDir}/${subdir}/*126941* ${outDir}/${subdir}"
	    #cp ${inDir}/${subdir}/*126941* ${outDir}/${subdir}
	done <tmp.txt
	rm -f tmp.txt

    fi

    exit 1;
else
    echo "Give background type to hadd eg: mc data" 
    echo " method: rlep flep/std"
    echo " mode: hadd or merge"
    exit 1;
fi



echo "Input history directory ${inpath}" 
echo "Output merge histos located in ${outpath}"


if [ ! -d "${dir}" ]; then
    mkdir ${dir}
fi
if [ ! -d "${outpath}" ]; then
    mkdir ${outpath}
fi
#rm -f ${outpath}/*


#DIL=(MM)
#SR=(CRWW6)
#SR=(preSRmT2 SRmT2a SRmT2b \
#    ZXCRpremT2 ZXCRmT2a ZXCRmT2b ZXCRWW ZXCRWW2  \
#    CRTOP \
#    CRWW CRWW2 CRWW3 CRWW4   )


DIL=(EE MM EM)
#<<SKIP
SR=(preSRmT2 SRmT2a 
    SRmT2b \
    ZXCRpremT2 ZXCRmT2a ZXCRmT2b ZXCRWW ZXCRWW2  \
    CRTOP \
    CRWW CRWW2 CRWW3 CRWW4 CRWW5 CRWW6 \
    SROSjveto  \
    SR2jets SRZjets SRSSjets \
    SRWWa SRWWb SRWWc \
    ZXCRjveto ZXCR2jets \
    CRTOPWWa CRTOPWWb CRTOPWWc \
    CRWWa CRWWb CRWWc \
    VRSS VRSSbtag \  
    CRZ CRZjveto CR2LepOS CR2LepSS CR2LepSS40 \
    preSROSjveto  \
    preSR2jets preSRZjets preSRSS  
)
#SKIP

list=${inlist}/${type}List.txt

while read line; do
    var=(`echo ${line} | tr '\t' ','`)
    bkgGp=${var[0]}
    if [[ $bkgGp == *#* ]]; then
	continue
    fi

    echo "Submitting ${bkgGp} "

    

    #
    #  2- MERGING MODE
    #
    if [ "$mode" == "merge" ]; then
	echo ${endpath}/histo_${bkgGp}_${mth}.root ${outpath}/hist_${bkgGp}_*_${mth}.root | root -b ${pathScript}/../macros/mergeRootFile.C+ |tee test.log
    fi

    #
    #  1- HADD MODE
    #
    if [ "$mode" == "hadd" ]; then
	
	fileList=${inlist}/${bkgGp}.txt
	
	for sr in ${SR[@]}; do
	    for dil in ${DIL[@]}; do
		echo " --> ${sr}_${dil}"
		
		[ -f ${dir}/hadd_${bkgGp}_${mth}_${sr}_${dil}.sh ] && rm -f ${dir}/hadd_${bkgGp}_${mth}_${sr}_${dil}.sh
		subdir=${sr}_${dil}
		
		echo -e "#!/bin/bash" >>  ${dir}/hadd_${bkgGp}_${mth}_${sr}_${dil}.sh 
		echo -e hadd -f ${outpath}/hist_${bkgGp}_${sr}_${dil}_${mth}.root '\' >>  ${dir}/hadd_${bkgGp}_${mth}_${sr}_${dil}.sh 
		
		nSample=(`wc -l $fileList`)
		nRead=0
		while read line; do
		    nRead=`expr $nRead + 1` 
		    array=(`echo ${line} | tr '\t' ','`)
		    sample=${array[0]}
		    
		    if [[ $sample == *#* ]]; then
			echo "Sample  $name is commented out - Skippping "
		    else
		        #build the name for a given sample
			name=histo_${sample}_${sr}_${dil}_${mth}*.root
		        #append to file
			if [ "$nRead" -lt "$nSample" ]; then 
			    echo -e    ${inpath}/${subdir}/${name} '\' >> ${dir}/hadd_${bkgGp}_${mth}_${sr}_${dil}.sh 
			else
			    echo -e    ${inpath}/${subdir}/${name}  >> ${dir}/hadd_${bkgGp}_${mth}_${sr}_${dil}.sh 
			fi
		    fi
		    
		done <${fileList}
		
		chmod 755 ${dir}/hadd_${bkgGp}_${mth}_${sr}_${dil}.sh 
		
		jobName=${bkgGp}_${mth}_${sr}_${dil}
	    #echo "${dir}/hadd_${bkgGp}_${mth}_${sr}_${dil}.sh"
	#echo "qsub -j oe -V -v script=${dir}/hadd_${bkgGp}_${mth}_${sr}_${dil}.sh,jobName=${jobName} -N ${jobName} -o ${pathRun}/batchLogs  ${pathScript}/qsub_hadd.sh"
		qsub -j oe -d ${PWD} -V -v script=${dir}/hadd_${bkgGp}_${mth}_${sr}_${dil}.sh,jobName=${jobName} -N ${jobName} -o ${pathRun}/batchLogs  ${pathScript}/qsub_hadd.sh
		
		sleep 0.5
		
	    done
          done
	
    fi
       
    
done<${list}


