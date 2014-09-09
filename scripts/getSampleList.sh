#!/bin/bash

#
# sed 's/^/#/' mc12_sampleList.txt >mc12_sampleList_primary.txt
#

sub=_n0155

InPath=/gdata/atlas/ucintprod/SusyNt
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

    #list=(`ls -d ${InPath}/$type${sub}/user* |sort`)
    list=(`ls -d ${InPath}/$type${sub}/group* |sort`)
    for iline in ${list[@]}; do
	dir=(`echo $iline | tr ',' ' '`)
	
        #echo "Directory ${dir}"
	DIS=( `echo $dir |cut -d'.' -f4-4`)
	if [[ ${DIS} -ge 146830 && ${DIS} -le 146835 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f4-5`)
	elif [[ ${DIS} -ge 146840 && ${DIS} -le 146855 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f3-5`)
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
	elif [[ ${DIS} -eq 119583 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f4-4`)
	elif [[ ${DIS} -ge 129134 && ${DIS} -le 129139 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f4-4`)
	elif [[ ${DIS} -ge 147666 && ${DIS} -le 147668 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f4-4`)
	elif [[ ${DIS} -ge 147025 && ${DIS} -le 147126 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f4-4`)
	elif [[ ${DIS} -ge 200056 && ${DIS} -le 200451 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f4-4`)
	elif [[ ${DIS} -ge 147900 && ${DIS} -le 147907 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f3-5`)
	elif [[ ${DIS} -eq 158344 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f4-5`)
	elif [[ ${DIS} -eq 164439 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f2-2`)
	elif [[ ${DIS} -ge 169704 && ${DIS} -le 169706 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f4-5`)
	elif [[ ${DIS} -ge 129477 && ${DIS} -le 129494 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f2-3`)
	elif [[ ${DIS} -eq 177997 || ${DIS} -eq 177999 || ${DIS} -eq 179974 || ${DIS} -eq 179975 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f3-5`)
	elif [[ ${DIS} -ge 183585 && ${DIS} -le 183592 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f3-5`)
	elif [[ ${DIS} -ge 183734 && ${DIS} -le 183739 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f3-5`)
	elif [[ ${DIS} -ge 179993 && ${DIS} -le 179998 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f3-4`)
	elif [[ ${DIS} -ge 179991 && ${DIS} -le 179992 ]]; then
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f4-6`)

	else
	    name=( `echo $dir |cut -d'.' -f5-5 |cut -d'_' -f3-3`)
	fi
	printf "${name}.${DIS} \t ${dir}/ \n" >>${log}
	
    done
    n=(`more mc12_sampleList.txt |wc -l `)
    printf "Number of MC12 samples ${n} \n" 

elif [ "$type" == "susy" ]; then
    log=${OutPath}/${type}_sampleList.txt
    [ -f ${log} ] && rm -f ${log}

    #list=(`ls -d ${InPath}/$type${sub}/user* |sort`)
    list=(`ls -d ${InPath}/$type${sub}/group* |sort`)
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

    #list=(`ls -d ${InPath}/$type${sub}/user* |grep Egamma`)
    list=(`ls -d ${InPath}/$type${sub}/group* |grep Egamma`)
    for iline in ${list[@]}; do
	dir=(`echo $iline | tr ',' ' '`)
	name="Egamma"
	DIS=( `echo $dir |cut -d'.' -f4-4`)
	printf "${name}.${DIS} \t ${dir}/ \n" >>${log}
    done
    
    #list=(`ls -d ${InPath}/$type${sub}/user* |grep Muons`)
    list=(`ls -d ${InPath}/$type${sub}/group* |grep Muons`)
    for iline in ${list[@]}; do
	dir=(`echo $iline | tr ',' ' '`)
	name="Muons"
	DIS=( `echo $dir |cut -d'.' -f4-4`)
	printf "${name}.${DIS} \t ${dir}/ \n" >>${log}
    done

    n=(`more data12_sampleList.txt |wc -l `)
    printf "Number of DATA12 samples ${n} \n" 

elif [ "$type" == "debug" ]; then
    log=${OutPath}/${type}_sampleList.txt
    [ -f ${log} ] && rm -f ${log}

    #list=(`ls -d ${InPath}/$type${sub}/user* |grep debug`)
    list=(`ls -d ${InPath}/$type${sub}/group* |grep debug`)
    for iline in ${list[@]}; do
	dir=(`echo $iline | tr ',' ' '`)
	name="debug"
	DIS=( `echo $dir |cut -d'.' -f4-4`)
	printf "${name}.${DIS} \t ${dir}/ \n" >>${log}
    done


fi
