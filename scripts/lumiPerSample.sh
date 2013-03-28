#!/bin/bash

# This script will calculate the lumi per sample for all D3PD datasets in a provided sample list
#
# Use text input files from
# 
# /home/sfarrell/workarea/SusyAna_2012_n0135/SusyCommon/grid
#

if [ $# -lt 1 ]; then
        echo "Provide input sample list for lumi calculation"
        exit 1
fi

#susyXsecFile=$ROOTCOREDIR/data/SUSYTools/susy_crosssections_8TeV.txt
susyXsecFile=$WORKAREA/SusyWeakProdAna/data/susy_crosssections_8TeV.txt

dsFile=$1

# Optional grep pattern
pattern="mc12"
if [ $# -gt 1 ]; then
        pattern=$2
fi

# Function to extract cross section from SUSYTools file
function getSusyXsec {
        # Extract the run number
        mcID=${1#mc12_8TeV.}
        mcID=${mcID%%.*}
        xsec=`cat $susyXsecFile | grep "^$mcID" | awk '{print $3}'`
        echo $xsec
}

function getSusykFac {
        # Extract the run number
        mcID=${1#mc12_8TeV.}
        mcID=${mcID%%.*}
        kfac=`cat $susyXsecFile | grep "^$mcID" | awk '{print $4}'`
        echo $kfac
}

function getSusyEff {
        # Extract the run number
        mcID=${1#mc12_8TeV.}
        mcID=${mcID%%.*}
        eff=`cat $susyXsecFile | grep "^$mcID" | awk '{print $5}'`
        echo $eff
}


# A trick to parse the text, first separate columns by commas
matches=(`cat $dsFile | grep "$pattern" | tr '\t' ',' | tr ' ' ','`)
echo "${#matches[@]} matches"

echo
echo -e "\tsumw     \txsec     \tkFac     \tEff \tlumi [1/fb]"

# Loop over samples
for line in ${matches[@]}; do

        # Now replace the commas with normal whitespace
        info=(`echo $line | tr ',' ' '`)

        # Extract dataset info
        # By default, no cross section is needed and the xsec from SUSYTools will be used
        # However, if the xsec column exists in the input file, that xsec will be used
        inDS=${info[0]}
        sumw=${info[1]}
        xsec=-1
	kfac=-1
	eff=-1
        if [ ${#info[@]} -gt 2 ]; then
                xsec=${info[2]}
        fi

        if [[ $xsec == "-1" ]]; then
                xsec=`getSusyXsec $inDS`
		kfac=`getSusykFac $inDS`
		eff=`getSusyEff $inDS`
        fi

        sumw=${sumw/e+/*10^}
	tot=`echo "$xsec*$kfac*$eff" | bc `
        lumi=`echo "scale=1; $sumw / ($xsec*$kfac*$eff * 1000.)" | bc`

	mcID=${inDS#mc12_8TeV.}
        mcID=${mcID%%.merge*}     
        echo -e "$mcID   \t$sumw    \t$xsec \t$kfac \t$eff   \t$lumi \t\t$tot"

done
