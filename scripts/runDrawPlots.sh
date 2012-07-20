#
# Run all the Data/MC prediction plots
#
#

#!/bin/bash

if [[ $# = 1 ]]; then
    mode=$1
fi

OPT="-logy"


if [ "$mode" == "DG2L" ] || [ "$mode" == "ALL" ] ; then
    
    SR=(SRjveto SRSSjveto SR2jet SRmT2 SR5  CRZ, NTOP NWW1 NWW2 CR2LepOS CR2LepSS)
    DIL=(EE MM EM)
    PLOTS=(mll etmiss metrel mt2 nJets nBJets npv mu \
	ptl1 ptl2 etal1 etal2 d0Sl1 d0Sl2 z0sinthetal1 z0sinthetal2 \
	ptj1 ptj2 ptj3 ptj4 etaj1 etaj3 etaj4 ptbj etabj )
       
    for sr in ${SR[@]}; do
	for dil in ${DIL[@]}; do
	    for plots in ${PLOTS[@]}; do
		name=${sr}_${dil}_DG2L_${plots}
		./DrawPlotsExec -pred -p ${name} ${OPT}
	    done
	done
    done
fi


