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
#OPT="-mode DD"
#OPT="-logy -mode STD -ZX histo_ZX_SherpaLFHF -Top histo_top_PowHeg  "
#OPT="-logy -mode DD -Top histo_topDil_Sherpa -WW histo_WW_Sherpa -ZX histo_ZX_Sherpa -Ztt histo_ZTauTaujets_Sherpa "
#OPT="-logy -mode STD -ZX histo_ZX_SherpaLFHF -Fake histo_mcfake_Sherpa"
#OPT="-logy -ZJet histo_Zjets_Alpgen -Fake histo_mcFake_Alpgen -Top histo_top_MCNLO"

pathRun=${WORKAREA}/SusyWeakProdAna/run

if [ "$mode" == "DG2L" ] || [ "$mode" == "ALL" ] ; then

#Add back tothe list 

#    SR=(CR2LepOS CR2LepSS CR2LepOS40 CR2LepSS40 CRZ CRZjveto VR1SS NTOP NWW1 \
#	ZXCR1 ZXCR3 ZXCR4 ZXCR5 ZXCR6 ZXCR7 \
#	SRjveto SRSSjveto SR2jets SRmT2 SRmT2b \
#        preSRjveto preSRSSjveto preSR2jets preSRmT2)

#    SR=(CR2LepOS40 CR2LepSS40 CRZ VR1SS NTOP NWW1  ZXCR1 ZXCR3 ZXCR4 ZXCR5 ZXCR6 ZXCR7)
#    SR=(SRjveto SRSSjveto SR2jets SRmT2 SRmT2b)
#    SR=(SRjveto SRjveto2 VRjveto VRjveto2 NTOP NWW1)
#    SR=(CR2LepSS)
#    SR=(VR1SS)
#    SR=(SRSS1j SRSS1jcut)
#     SR=(CRZ ZXCR1 ZXCR4)
    SR=(CRZjveto)
    DIL=(EE MM )
    PLOTS=(metrel)
#    PLOTS=(mll pTll etmiss metrel mt2 nJets nCJets nFJets nBJets \
#	ptl1 ptl2 etal1 etal2 ePt eEta mPt mEta dPhill \
#	ptj1 ptj2  etaj1 etaj2 ptbj etabj mjj ST \
#	metRefEle metRefGam metRefMuo metRefJet metRefSJet metCellout      
#	dPhilMet dPhiJetMet mEff mTl1 mTl2 mWWT
#	nSoftJets ptSj1 ptSj2 etaSj1 etaSj2 jvfSj1 jvfSj2)

#    PLOTS=(pTll etmiss metrel mt2 nJets nCJets nFJets nBJets \
#	ptl1 ptl2 etal1 etal2 ePt eEta mPt mEta dPhill \
#	ptj1 ptj2  etaj1 etaj2 ptbj etabj mjj ST \
#	metRefEle metRefGam metRefMuo metRefJet metRefSJet metCellout      
#	dPhilMet dPhiJetMet mEff mTl1 mTl2 mWWT
#	nSoftJets ptSj1 ptSj2 etaSj1 etaSj2 jvfSj1 jvfSj2)


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
