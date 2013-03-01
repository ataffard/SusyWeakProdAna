#
#  cat Zjets_AlpgenPythia.txt WZ_ZZ_Sherpa.txt > ZX_AlpgenPythia.txt
#  cat Zjets_Sherpa.txt WZ_ZZ_Sherpa.txt > ZX_Sherpa.txt
#  cat Zjets_SherpaLFHF.txt WZ_ZZ_Sherpa.txt >ZX_SherpaLFHF.txt
#
#  cat ZX_AlpgenPythia.txt top_MCNLO.txt WW_Sherpa.txt ZTauTaujets_AlpgenPythia.txt >Bkg_ZXAlpgen_TopMCNLO.txt
#  cat ZX_AlpgenPythia.txt top_Alpgen.txt WW_Sherpa.txt ZTauTaujets_AlpgenPythia.txt >Bkg_ZXAlpgen_TopAlpgen.txt
#  cat ZX_AlpgenPythia.txt top_PowHeg.txt WW_Sherpa.txt ZTauTaujets_AlpgenPythia.txt >Bkg_ZXAlpgen_TopPowHeg.txt
#
#  cat ZX_Sherpa.txt top_MCNLO.txt WW_Sherpa.txt ZTauTaujets_Sherpa.txt >Bkg_ZXSherpa_TopMCNLO.txt
#  cat ZX_Sherpa.txt top_Alpgen.txt WW_Sherpa.txt ZTauTaujets_Sherpa.txt >Bkg_ZXSherpa_TopAlpgen.txt
#  cat ZX_Sherpa.txt top_PowHeg.txt WW_Sherpa.txt ZTauTaujets_Sherpa.txt >Bkg_ZXSherpa_TopPowHeg.txt
#
# check log for error grep -i error  *_SROSjveto_EE*  |grep -i cannot
#
#

#!/bin/bash

if [[ $# = 3 ]]; then
    type=$1
    mth=$2
    mode=$3
    echo "Request ${type} method: ${mth}"
else
    echo "Give background type to hadd eg: mc data" 
    echo " method: rlep flep/std"
    echo " mode: hadd or merge"
    exit 1;
fi


pathScript=${WORKAREA}/SusyWeakProdAna/scripts
pathRun=${WORKAREA}/SusyWeakProdAna/run

inlist=${WORKAREA}/SusyWeakProdAna/scripts/inputHistoList
dir=${WORKAREA}/SusyWeakProdAna/scripts/haddJobs

inpath=${WORKAREA}/histoAna/SusyAna/histOutputs
outpath=${WORKAREA}/histoAna/SusyAna/hadd_histOutputs
endpath=${WORKAREA}/histoAna/SusyAna/


echo "Input history directory ${inpath}" 
echo "Output merge histos located in ${outpath}"


if [ ! -d "${dir}" ]; then
    mkdir ${dir}
fi
if [ ! -d "${outpath}" ]; then
    mkdir ${outpath}
fi
#rm -f ${outpath}/*


#DIL=(EE)
#SR=(SROSjveto)

#<<SKIP
DIL=(EE MM EM)
SR=(SROSjveto SRmT2a SRmT2b \
    SR2jets SRZjets SRSSjets \
    SRWWa SRWWb SRWWc \
    ZXCRjveto ZXCR2jets \
    ZXCRmT2a ZXCRmT2b ZXCRWW \
    CRTOP CRTOPWWa CRTOPWWb CRTOPWWc \
    CRWW CRWWa CRWWb CRWWc \
    VRSS VRSSbtag \  
    CRZ CRZjveto CR2LepOS CR2LepSS CR2LepSS40 \
    preSROSjveto preSRmT2 \
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
		
		sleep 0.1
		
	    done
          done
	
    fi
    


    
    
done<${list}

qstat |grep ataffard
