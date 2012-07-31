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
    
    SR=(SRjveto SRSSjveto SR2jet SRmT2 SR5 CRZ NTOP NWW1 NWW2 CR2LepOS CR2LepSS)
    DIL=(EE MM EM)
    PLOTS=(mll etmiss metrel mt2 nJets nBJets npv mu \
	ptl1 ptl2 etal1 etal2 d0Sl1 d0Sl2 z0sinthetal1 z0sinthetal2 \
	ptj1 ptj2 ptj3 ptj4 etaj1 etaj3 etaj4 ptbj etabj )
       
    for sr in ${SR[@]}; do
	for dil in ${DIL[@]}; do
	    if [ "$sr" == "SR2jet" -a "$dil" == "EM" ]; then continue; fi
	    if [ "$sr" == "CRZ" -a "$dil" == "EM" ]; then continue; fi
	    for plots in ${PLOTS[@]}; do
		name=DG2L_${sr}_${dil}_DG2L_${plots}
		./DrawPlotsExec -pred -p ${name} ${OPT}
	    done
	done
    done
fi


if [ "$mode" == "ML" ] || [ "$mode" == "ALL" ] ; then

    SR=(SR3Lep SRB SR1a SR1b SR2 VR0 VR1 VR2 VR3 VRWZ NRWZ SR4lep SR4lepNoZ)
    PLOTS=(evtCatgUnOrdered evtCatgOSpair nLep nJets nBJets SFOSMll SFOSMlll AllMll AllMlll SFOSMT ptl1 ptl2 ptl3 ptl4 etal1 etal2 etal3 etal4 d0Sl1 d0Sl2 d0Sl3 d0Sl4 z0sinthetal1 z0sinthetal2 z0sinthetal3 z0sinthetal4 ptj1 ptj2 ptj3 ptj4 etaj1 etaj2 etaj3 ptbj etabj)
    for sr in ${SR[@]}; do
	for plots in ${PLOTS[@]}; do
	    name=ML_${sr}_ML_${plots}
	    ./DrawPlotsExec -pred -p ${name} ${OPT}
	done
    done


fi
