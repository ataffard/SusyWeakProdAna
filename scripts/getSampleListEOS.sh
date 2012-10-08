#!/bin/bash
eos='/afs/cern.ch/project/eos/installation/0.1.0-22d/bin/eos.select'
prefix='root://eosatlas/'
location='/eos/atlas/user/a/amete/SUSY2012/n0105'



InPath=${location}
OutPath=$WORKAREA/SusyWeakProdAna/scripts

if [[ $# = 1 ]]; then
    type=$1
    echo "Generating sample list for $type"
else
    echo "Give argument mc12, susy, data12"
    exit 1;
fi


if [[ $type == "mc12" ]]; then
    log=${OutPath}/${type}_sampleList.txt
    [ -f ${log} ] && rm -f ${log}

    list=(`$eos ls -d ${InPath}/ |grep mc12 |sort`)
    for iline in ${list[@]}; do
	dir=(`echo $iline | tr ',' ' '`)
	
        #echo "Directory ${dir}"
	DIS=( `echo $dir |cut -d'.' -f4-4`)
	if [[ ${DIS} -ge 146830 && ${DIS} -le 146855 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f4-5`)
	elif [[ ${DIS} -ge 126892 && ${DIS} -le 126895 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f3-4`)
	elif [[ ${DIS} -ge 126928 && ${DIS} -le 126936 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f3-4`)
	elif [[ ${DIS} -ge 126937 && ${DIS} -le 126951 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f3-4`)
	elif [[ ${DIS} -ge 117360 && ${DIS} -le 117362 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f3-5`)
	elif [[ ${DIS} -ge 117209 && ${DIS} -le 117210 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f4-4`)
	else
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f3-3`)
	fi
	printf "${name}.${DIS} \t ${InPath}/${dir}/ \n" >>${log}
	
    done
    n=(`more mc12_sampleList.txt |wc -l `)
    printf "Number of MC12 samples ${n} \n" 
elif [ "$type" == "susy" ]; then
    log=${OutPath}/${type}_sampleList.txt
    [ -f ${log} ] && rm -f ${log}

    list=(`ls -d ${InPath}/$type${sub}/user* |sort`)
    for iline in ${list[@]}; do
	dir=(`echo $iline | tr ',' ' '`)
	
        #echo "Directory ${dir}"
	DIS=( `echo $dir |cut -d'.' -f4-4`)
	name=( `echo $dir |cut -d'.' -f5-5`)
	printf "${name}.${DIS} \t ${dir}/ \n" >>${log}
    done
    n=(`more susy_sampleList.txt |wc -l `)
    printf "Number of SUSY samples ${n} \n" 
elif [ "$type" == "data12" ]; then
    log=${OutPath}/${type}_sampleList.txt
    [ -f ${log} ] && rm -f ${log}

    list=(`$eos ls -d ${InPath}/ |grep Egamma`)
    for iline in ${list[@]}; do
	dir=(`echo $iline | tr ',' ' '`)
	name="Egamma"
	DIS=( `echo $dir |cut -d'.' -f6-6`)
	printf "${name}.${DIS} \t ${InPath}/${dir}/ \n" >>${log}
    done
    
    list=(`$eos ls -d ${InPath}/ |grep Muons`)
    for iline in ${list[@]}; do
	dir=(`echo $iline | tr ',' ' '`)
	name="Muons"
	DIS=( `echo $dir |cut -d'.' -f6-6`)
	printf "${name}.${DIS} \t ${InPath}/${dir}/ \n" >>${log}
    done


    n=(`more data12_sampleList.txt |wc -l `)
    printf "Number of DATA12 samples ${n} \n" 

fi
