#!/bin/bash
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
# ./submitT3_hadd.sh pass 
#
# ./submitT3_hadd.sh mc rlep hadd; ./submitT3_hadd.sh data std hadd; ./submitT3_hadd.sh data flep hadd; 
#
# ./submitT3_hadd.sh mc rlep merge; ./submitT3_hadd.sh data std merge; ./submitT3_hadd.sh data flep merge; 
#
# ./submitT3_hadd.sh mc rlep toyNtMerger; ./submitT3_hadd.sh susy rlep toyNtMerger; 
# ./submitT3_hadd.sh data std toyNtMerger; ./submitT3_hadd.sh dataFake flep toyNtMerger; 
#
#
#

#Update for a given pass
#date="062714_21fb_n0150_DD_LFV_v1"
date="070614_21fb_n0150_DD_LFV_v1"
#date="060314_21fb_n0150_DD_WH_v8"
#date="030614_21fb_n0150_DD_FAKE_v1"


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

inToyNtPath=${HISTOANA}/SusyAna/histos_${date}/ToyNtOutputs

#toyNt_SR="DIL_optimSRjets"
#toyNt_SR="DIL_optimSRSS"
#toyNt_SR="DIL_optimSR0jet"

#toyNt_SR="WH_optimSRSS"
toyNt_SR="LFV_base"

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

	cat ${inlist}/Zjets_AlpgenPythia.txt ${inlist}/WZ_ZZ_PowHeg.txt  ${inlist}/WW_PowHeg.txt \
	    ${inlist}/top_MCNLO.txt ${inlist}/Higgs.txt > ${inlist}/Bkg_Zjets_Alpgen_WZ_ZZ_PowHeg_WW_PowHeg_TopMCNLO.txt

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
	mv ${pathRun}/batchLogs/*.log ${passDir}/logs/jobs
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



echo "Input hist directory ${inpath}" 
echo "Output merge histos located in ${outpath}"


if [ ! -d "${dir}" ]; then
    mkdir ${dir}
fi
if [ ! -d "${outpath}" ]; then
    mkdir ${outpath}
fi
#rm -f ${outpath}/*


##
## qsub_hadd.... edit between 2L 3L mode
##

#DIL=(MM)
#SR=(SRmT2a)

DIL=(EE MM EM)
#DIL=(EEE EEM EMM MMM LLL \
#    EEEE EEEM EEMM EMMM MMMM LLLL)
#<<SKIP
SR=(\
    WH_SRSS1j WH_SRSS23j WH_optimSRSS WH_CRSSZVFAKE WH_CRSSFAKE \
    WH_HighMll WH_HighPtll WH_lowMET WH_BTag \
    #ML_CF3L ML_VRWZ ML_VRZZ ML_VRemulWW \
    #SRmT2a SRmT2b SRmT2c \
    #SRWWa SRWWb SRWWc \
    #SRZjets \
    #SRZjetsb \
    #SRSSjets \
    #CRWWmet CRWWmt2 \
    #CRTOPmet CRTOPmt2 CRTOPZjets \
    #CRZVmet CRZVmt2a CRZVmt2b CRZVmt2c CRZVmt2d \
    #CRZVZjets \
    #VRSS \
    #CRZ CRZjets CRZjveto \
    #CR2LepOS CR2LepSS CR2LepSS40 \
    #preSROSjveto preSRmT2 preSRZjets preSRWW preSRSS \
    #optimSRZjets optimSRjets optimSRSS optimSR0jet \
    #CRZVZjets1 CRZVZjets2 CRZVZjets3 CRZVZjets4 CRZjetsTemplate  \
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
    #  3- MERGING TOYNT MODE
    #
    if [ "$mode" == "toyNtMerger" ]; then
	echo "reading ${inlist}/${bkgGp}.txt" 
	rm -f tmpList.txt
	while read newline; do
	    ss=(`echo ${newline} | tr '\t' ',' `)
	    if [[ $ss == *#* ]]; then
		echo "Sample  $ss is commented out - Skippping "
		continue 
	    fi
	    
	    if [ "$type" == "dataFake" -o "$type" == "data" ]; then
		ss=(`echo ${newline} | tr '\t' ',' `)
	    else
		ss=(`echo ${newline} | tr '\t' ',' |cut -d'.' -f2-2 `)
	    fi
	    if [[ "$ss" == *mll60* ]]; then
		ss=(`echo $ss |cut -d'_' -f1-1 `)
	    fi

	    name=${ss}"_"${toyNt_SR}".root"
	    if [ "$type" == "data" ]; then
		name=${ss}_"DATA_"${toyNt_SR}".root"
	    fi
	    
	    echo -e ${inToyNtPath}/${name} >> tmpList.txt
	done < ${inlist}/${bkgGp}.txt

	echo ${endpath}/toyNt_${bkgGp}_${toyNt_SR}_${mth}.root tmpList.txt | root -b ${pathScript}/../macros/mergeToyNtFile.C+ |tee test.log
	rm -f tmpList.txt
    fi


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
		echo "    --> ${sr}_${dil}"
		
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
		
		#jobName=${bkgGp}_${mth}_${sr}_${dil}
	    #echo "${dir}/hadd_${bkgGp}_${mth}_${sr}_${dil}.sh"
	#echo "qsub -j oe -V -v script=${dir}/hadd_${bkgGp}_${mth}_${sr}_${dil}.sh,jobName=${jobName} -N ${jobName} -o ${pathRun}/batchLogs  ${pathScript}/qsub_hadd.sh"
		#qsub -j oe -d ${PWD} -V -v script=${dir}/hadd_${bkgGp}_${mth}_${sr}_${dil}.sh,jobName=${jobName} -N ${jobName} -o ${pathRun}/batchLogs  ${pathScript}/qsub_hadd.sh
	  done
	  export jobName=${bkgGp}_${mth}_${sr}
	  echo "Submitting ${jobName}"
	  export script=${dir}/hadd_${bkgGp}_${mth}_${sr}
	  sbatch --job-name=${jobName}  --output=${pathRun}/batchLogs/${name}_${method}${suff}.log --error=${pathRun}/batchLogs/${name}_${method}${suff}.log  ${pathScript}/qsub_hadd.sh "script=$script, jobName=$jobName"
	  #qsub -j oe -d ${PWD} -V -v script=${dir}/hadd_${bkgGp}_${mth}_${sr},jobName=${jobName} -N ${jobName} -o ${pathRun}/batchLogs  ${pathScript}/qsub_hadd.sh
	  sleep 0.5
		

          done
	
    fi
       
    
done<${list}


