#
# Run all the Data/MC prediction plots
#
# ./DrawPlotsExec -pred -p DG2L_CR2LepOS_EE_DG2L_metrel -logy -ZJet histo_Zjets_Sherpa.root
#

#!/bin/bash

if [[ $# = 1 ]]; then
    mode=$1
fi

#Update SusyAnaCommon.h to pick correct path for .root files

#OPT="-logy -mode STD"
OPT="-logy -mode DD"
#OPT="-logy -mode STD -ZX histo_ZX_SherpaLFHF -Top histo_top_PowHeg  "
#OPT="-logy -mode DD -Top histo_topDil_Sherpa -WW histo_WW_Sherpa -ZX histo_ZX_Sherpa -Ztt histo_ZTauTaujets_Sherpa "
#OPT="-logy -mode STD -ZX histo_ZX_SherpaLFHF -Fake histo_mcfake_Sherpa"
#OPT="-logy -ZJet histo_Zjets_Alpgen -Fake histo_mcFake_Alpgen -Top histo_top_MCNLO"

pathRun=${WORKAREA}/SusyWeakProdAna/run

if [ "$mode" == "DG2L" ] || [ "$mode" == "ALL" ] ; then

#for testing
   SR=(CR2LepSS VR1SS preSRSSjveto)
   DIL=(EE MM EM)
   PLOTS=(mll metrel etmiss)

#    SR=(CRZ ZXCR1 ZXCR3 ZXCR4)
#    DIL=(EE MM EM)
#    PLOTS=(mll metrel etmiss mt2 )
#    PLOTS=(metRefEle metRefGam metRefMuo metRefJet metRefSJet metCellout)
 
#     SR=(CR2LepOS CR2LepSS CRZ  NTOP NWW1 NWW2 NWW3 ZXCR1 ZXCR3 ZXCR4 ZXCR5 SRjveto SRSSjveto SR2jets SRmT2 SRmT2b)
#    SR=(SRjveto)
#    PLOTS=(mll metrel etmiss)
#    SR=(SRjveto SRSSjveto SR2jets SRmT2 SRmT2b )
#    DIL=(EE MM EM)
#    PLOTS=(mll pTll mjj dPhill etmiss metrel mt2 nJets nCJets nFJets nBJets npv mu \
#	ptl1 ptl2 etal1 etal2 d0Sl1 d0Sl2 z0sinthetal1 z0sinthetal2 \
#	ptj1 ptj2 ptj3 ptj4 etaj1 etaj3 etaj4 ptbj etabj \
#	metRefEle metRefGam metRefMuo metRefJet metRefSJet metCellout)      

    for sr in ${SR[@]}; do
	for dil in ${DIL[@]}; do
	    if [ "$sr" == "SR2jet" -a "$dil" == "EM" ]; then continue; fi
	    if [ "$sr" == "CRZ" -a "$dil" == "EM" ]; then continue; fi
	    for plots in ${PLOTS[@]}; do
		name=DG2L_${sr}_${dil}_DG2L_${plots}
		${pathRun}/DrawPlotsExec -pred -p ${name} ${OPT}
	    done
	done
    done
fi


if [ "$mode" == "ML" ] || [ "$mode" == "ALL" ] ; then

    SR=(SR3Lep SRB SR1a SR1b SR2 VR0 VR1 VR2 VR3 VRWZ NRWZ SR4lep SR4lepNoZ)
    PLOTS=(evtCatgUnOrdered evtCatgOSpair nLep nJets nBJets \
	SFOSMll SFOSMlll AllMll AllMlll SFOSMT \
	ptl1 ptl2 ptl3 ptl4 etal1 etal2 etal3 etal4 \
	d0Sl1 d0Sl2 d0Sl3 d0Sl4 z0sinthetal1 z0sinthetal2 z0sinthetal3 z0sinthetal4 \
	ptj1 ptj2 ptj3 ptj4 etaj1 etaj2 etaj3 ptbj etabj \
	metRefEle metRefGam metRefMuo metRefJet metRefSJet metCellout)
    for sr in ${SR[@]}; do
	for plots in ${PLOTS[@]}; do
	    name=ML_${sr}_ML_${plots}
	    ${pathRun}/DrawPlotsExec -pred -p ${name} ${OPT}
	done
    done


fi
