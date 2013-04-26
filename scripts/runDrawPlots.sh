#
# Run all the Data/MC prediction plots
#
# ./DrawPlotsExec -pred -p DG2L_CR2LepOS_EE_DG2L_metrel -logy -ZJet histo_Zjets_Sherpa.root
#
# ./runDrawPlots.sh DG2L preSRmT2
# ./runDrawPlots.sh DG2L SRmT2a; ./runDrawPlots.sh DG2L SRmT2b
# ./runDrawPlots.sh DG2L CRTOP; ./runDrawPlots.sh DG2L CRWW5;
# ./runDrawPlots.sh DG2L ZXCRmT2a; ./runDrawPlots.sh DG2L ZXCRmT2b
#
#

#!/bin/bash

if [[ $# = 2 ]]; then
    mode=$1
    SR=$2
elif [[ $# = 1 ]]; then
    mode=$1
    SR=(preSRmT2 \
	ZXCRmT2a \
	ZXCRmT2b \
	ZXCRWW \
	CRTOP \
	CRWW CRWW2 CRWW3 CRWW4 \
	CRWW5 \ 
	CRWW6 \
	CRZ CRZjveto CR2LepOS \
	SRmT2a \
	SRmT2b \
       )
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
    
#SR=(CR2LepOS)
#SR=(preSRmT2 CRWW CRWW2 CRWW5 CRWW6 ZXCRpremT2 ZXCRWW ZXCRWW2 ZXCRmT2a ZXCRmT2b CRTOP )
#PLOTS=(metrel pTll mll mllcoarse mt2 ptl1 ptl2 ePt mPt )
#PLOTS=(mllcoarse)

DIL=(EE)
#DIL=(EE MM EM)




<<SKIP
SR=(SROSjveto SRmT2a SRmT2b \
    SR2jets SRZjets SRSSjets \
    SRWWa SRWWb SRWWc \
    ZXCRjveto ZXCR2jets \
    ZXCRmT2a ZXCRmT2b ZXCRWW \
    CRTOP CRTOPWWa CRTOPWWb CRTOPWWc \
    CRWW CRWW2 CRWW3 CRWW4 CRWW5 \
    CRWWa CRWWb CRWWc \
    VRSS VRSSbtag \  
    CRZ CRZjveto CR2LepOS CR2LepSS CR2LepSS40 \
    preSROSjveto preSRmT2 \
    preSR2jets preSRZjets preSRSS  
)
SKIP

PLOTS=(mllcoarser pTll metrel mt2 mt2b metrel1 metrel2 metrel3 \
  ePt mPt \
    #mll  ptl1 ptl2 etal1 etal2 eEta mEta dPhill \    
    #nJets nCJets nFJets nBJets \
    #ptj1 ptj2  etaj1 etaj2 ptbj etabj mjj ST \
    #etmiss metRefEle metRefGam metRefMuo metRefJet metRefSJet metCellout \
    #dPhilMet dPhiJetMet mEff mTl1 mTl2 mWWT \
    #nSoftJets ptSj1 ptSj2 etaSj1 etaSj2 jvfSj1 jvfSj2
)

    
    for sr in ${SR[@]}; do
	for dil in ${DIL[@]}; do
	    if [ "$sr" == "SR2jet" -a "$dil" == "EM" ]; then continue; fi
	    if [ "$sr" == "CRZ" -a "$dil" == "EM" ]; then continue; fi
	    if [ "$sr" == "CRWW" -a "$dil" == "EM" ]; then continue; fi
	    if [ "$sr" == "SRWWa" -o "$sr" == "SRWWb" -o "$sr" == "SRWWc" -o \
		 "$sr" == "CRWWa" -o "$sr" == "CRWWb" -o "$sr" == "CRWWc" -o \
		 "$sr" == "CRWWa" -o "$sr" == "CRWWb" -o "$sr" == "CRWWc" \
		]; then
		if [ "$dil" == "EE" -o "$dil" == "MM" ]; then continue; fi
	    fi
	    
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
