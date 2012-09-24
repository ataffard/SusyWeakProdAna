#!/bin/bash

#Match dir name constructed in SusyAnaCommon.h

date="092312_1fb_n0101"
#date="092312_1fb_n0101_noDGTrig"

mth="std"

logPath=${WORKAREA}/SusyWeakProdAna/run/jobLogs
#histPath=${WORKAREA}/histoAna/SusyAna
histPath=${WORKAREA}/histoAna/SusyAna/histos_${date}
outPath=${WORKAREA}/histoAna/SusyAna/histos_${date}


#doData=true
#doTopPowHeg=true
#doTopMCNLO=true

#doWWSherpa=true;
#doWWPowHeg=true;

##doDiBHerwig=false
##doDiBMCNLO=false

#doZXSherpa=true
#doZXPowHeg=true

#doZjetAlpgen=true
#doZjetSherpaIncl=true
#doZjetSherpaLFHF=true

doWjetSherpa=true
#doWjetAlpgen=true

#doMergeWjetBB=true

#doSignal=true

#doMove=true

if [ $doData ]; then
    hadd -f ${histPath}/histo_data12_${mth}.root \
	${histPath}/histo_Egamma.periodA_${mth}.root \
	${histPath}/histo_Muons.periodA_${mth}.root \
	${histPath}/histo_Egamma.periodB_${mth}.root \
	${histPath}/histo_Muons.periodB_${mth}.root  \
	${histPath}/histo_Egamma.periodC_${mth}.root \
	${histPath}/histo_Muons.periodC_${mth}.root \
	${histPath}/histo_Egamma.periodD_${mth}.root \
	${histPath}/histo_Muons.periodD_${mth}.root 
fi 

#
# mc12a
#

if [ $doTopMCNLO ]; then
    echo "Merge Top MC@NLO"
    rm -f ${histPath}/histo_top_MCNLO_${mth}.root
    hadd -f ${histPath}/histo_top_MCNLO_${mth}.root   \
	${histPath}/histo_ttbar.105200_${mth}.root \
	${histPath}/histo_SingleTopSChan*.*_${mth}.root  \
	${histPath}/histo_singletop_tchan*.*_${mth}.root  \
	${histPath}/histo_SingleTopWtChanIncl.108346_${mth}.root  \
	${histPath}/histo_ttbarW.119353_${mth}.root \
	${histPath}/histo_ttbarWj.119354_${mth}.root \
	${histPath}/histo_ttbarZ.119355_${mth}.root \
	${histPath}/histo_ttbarZj.119356_${mth}.root 
fi

if [ $doTopPowHeg ]; then
    echo "Merge Top POWHEG"
    rm -f ${histPath}/histo_top_PowHeg_${mth}.root
    hadd -f ${histPath}/histo_top_PowHeg_${mth}.root   \
	${histPath}/histo_ttbar.105861_${mth}.root \
	${histPath}/histo_SingleTopSChan*.*_${mth}.root  \
	${histPath}/histo_singletop_tchan*.*_${mth}.root  \
	${histPath}/histo_SingleTopWtChanIncl.108346_${mth}.root  \
	${histPath}/histo_ttbarW.119353_${mth}.root \
	${histPath}/histo_ttbarWj.119354_${mth}.root \
	${histPath}/histo_ttbarZ.119355_${mth}.root \
	${histPath}/histo_ttbarZj.119356_${mth}.root 
fi

if [ $doWWSherpa ]; then
#Sherpa Diboson
    echo "Merge WW Sherpa "
    rm -f ${histPath}/histo_WW_Sherpa_${mth}.root
    hadd -f ${histPath}/histo_WW_Sherpa_${mth}.root \
	${histPath}/histo_llnunu_WW.126892_${mth}.root \
	${histPath}/histo_WpWm*.106*_${mth}.root  
    #TODO add 126988+126989
fi

if [ $doWWPowHeg ]; then
#Diboson PowHegPythia and Wg AlpgenJimmy + Zg Sherpa
    echo "Merge WW Powheg "
    rm -f ${histPath}/histo_WW_Powheg_${mth}.root
    hadd -f ${histPath}/histo_WW_Powheg_${mth}.root \
	${histPath}/histo_WpWm*.126*_${mth}.root \
	${histPath}/histo_WpWm*.106*_${mth}.root  
    #TODO add 126988+126989
fi 

##BUGGY
#if [ $doDiBHerwig ]; then
#Herwig Diboson +Wg AlpgenJimmy + Zg Sherpa
#    echo "Merge diBoson Herwig "
#    rm -f ${histPath}/histo_diBoson_Herwig_${mth}.root
#    hadd -f ${histPath}/histo_diBoson_Herwig_${mth}.root \
#	${histPath}/histo_WW.105985_${mth}.root  \
#	${histPath}/histo_ZZ.105986_${mth}.root  \
#	${histPath}/histo_WZ.105987_${mth}.root  \
#	${histPath}/histo_WpWm*.106*_${mth}.root  \
#fi

##BUGGY
#if [ $doDiBMCNLO ]; then
#MC@NLO WW + Herwig for ZZ, WZ and Wg AlpgenJimmy + Zg Sherpa
#    echo "Merge diBoson MC@NLO "
#    rm -f ${histPath}/histo_diBoson_MCATNLO_${mth}.root
#    hadd -f ${histPath}/histo_diBoson_MCATNLO_${mth}.root \
#	${histPath}/histo_WpWm*.105*_${mth}.root \
#	${histPath}/histo_ZZ.105986_${mth}.root  \
#	${histPath}/histo_WZ.105987_${mth}.root  \
#	${histPath}/histo_WpWm*.106*_${mth}.root  
#fi

if [ $doZXSherpa ]; then
#Sherpa Diboson
    echo "Merge Z+X Sherpa "
    rm -f ${histPath}/histo_ZX_Sherpa_${mth}.root
    hadd -f ${histPath}/histo_ZX_Sherpa_${mth}.root \
	${histPath}/histo_llll_ZZ.126894_${mth}.root \
	${histPath}/histo_llnunu_ZZ.126895_${mth}.root \
	${histPath}/histo_lllnu_WZ.126893_${mth}.root 
    #TODO add gg2ZZ 11660*
fi

if [ $doZXPowHeg ]; then
#Diboson PowHegPythia and Wg AlpgenJimmy + Zg Sherpa
    echo "Merge Z+X Powheg "
    rm -f ${histPath}/histo_ZX_Powheg_${mth}.root
    hadd -f ${histPath}/histo_ZX_Powheg_${mth}.root \
	${histPath}/histo_WZ.1294*_${mth}.root \
	${histPath}/histo_ZZ*.1269*_${mth}.root 
    #TODO add gg2ZZ 11660*
fi 

#Buggy
if [ $doZjetAlpgen ]; then
#Z+jets Alpgen
    echo "Merge Z+jets Alpgen "
    rm -f ${histPath}/histo_Zjets_Alpgen_${mth}.root
    hadd -f ${histPath}/histo_Zjets_Alpgen_${mth}.root  \
	${histPath}/histo_ZeeNp?.*_${mth}.root  \
	${histPath}/histo_ZeeNp?Excl_Mll10to60.*_mll60_${mth}.root  \
	${histPath}/histo_ZeebbNp?.*_${mth}.root \
	${histPath}/histo_ZmumuNp?.*_${mth}.root  \
	${histPath}/histo_ZmumuNp?Excl_Mll10to60.*_mll60_${mth}.root  \
	${histPath}/histo_ZmumubbNp?.*_${mth}.root  \
	${histPath}/histo_ZtautauNp?.*_${mth}.root  \
	${histPath}/histo_ZtautauNp?Excl_Mll10to60.*_mll60_${mth}.root  \
	${histPath}/histo_ZtautaubbNp?.*_${mth}.root  \
	${histPath}/histo_ZmumuccNp?.*_${mth}.root  \
	${histPath}/histo_ZeeccNp?.*_${mth}.root \
	${histPath}/histo_ZtautauccNp?.*_${mth}.root  
fi

if [ $doZjetSherpaIncl ]; then
#Z+jets Sherpa
    echo "Merge Z+jets Sherpa Inclusive"
    rm -f ${histPath}/histo_Zjets_Sherpa_${mth}.root
    hadd -f ${histPath}/histo_Zjets_Sherpa_${mth}.root  \
	${histPath}/histo_Zee.147770_${mth}.root \
	${histPath}/histo_Zmumu.147771_${mth}.root \
	${histPath}/histo_Ztautau.147772_${mth}.root \
	${histPath}/histo_ZeeNp?Excl_Mll10to60.*_mll40_${mth}.root  \
	${histPath}/histo_ZmumuNp?Excl_Mll10to60.*_mll40_${mth}.root  \
	${histPath}/histo_ZtautauNp?Excl_Mll10to60.*_mll40_${mth}.root 
#\
#	${histPath}/histo_eegammaPt10.145161_${mth}.root \
#	${histPath}/histo_mumugammaPt10.145162_${mth}.root \
#	${histPath}/histo_tautaugammaPt10.126854_${mth}.root 
fi    

if [ $doZjetSherpaLFHF ]; then
#Z+jets Sherpa
    echo "Merge Z+jets Sherpa LF+HF + low mass Alpgen"
    rm -f ${histPath}/histo_Zjets_SherpaLFHF_${mth}.root
    hadd -f ${histPath}/histo_Zjets_SherpaLFHF_${mth}.root  \
	${histPath}/histo_ZeeLightJets.146820_${mth}.root \
	${histPath}/histo_ZeeHeavyJets.128975_${mth}.root \
	${histPath}/histo_ZmumuLightJets.146821_${mth}.root \
	${histPath}/histo_ZmumuHeavyJets.128976_${mth}.root \
	${histPath}/histo_ZtautauLightJets.146822_${mth}.root \
	${histPath}/histo_ZtautauHeavyJets.128977_${mth}.root \
	${histPath}/histo_ZeeNp?Excl_Mll10to60.*_mll40_${mth}.root  \
	${histPath}/histo_ZmumuNp?Excl_Mll10to60.*_mll40_${mth}.root  \
	${histPath}/histo_ZtautauNp?Excl_Mll10to60.*_mll40_${mth}.root 
#\
#	${histPath}/histo_eegammaPt10.145161_${mth}.root \
#	${histPath}/histo_mumugammaPt10.145162_${mth}.root \
#	${histPath}/histo_tautaugammaPt10.126854_${mth}.root 
fi 

if [ $doWjetSherpa ]; then
#W+jets Sherpa
    echo "Merge W+jets Sherpa "
    rm -f ${histPath}/histo_Wjets_Sherpa_${mth}.root
    hadd -f ${histPath}/histo_Wjets_Sherpa_${mth}.root   \
	${histPath}/histo_Wenu*.147774*_${mth}.root  \
	${histPath}/histo_Wmunu*.147775*_${mth}.root  \
	${histPath}/histo_Wtaunu*.147776*_${mth}.root  
#\
#	${histPath}/histo_enugammaPt10.126739*_${mth}.root  \
#	${histPath}/histo_munugammaPt10.126742*_${mth}.root  \
#	${histPath}/histo_taunugammaPt10.126856*_${mth}.root  

fi

if [ $doWjetAlpgen ]; then
#W+jets Alpgen
    echo "Merge W+jets Alpgen "
    rm -f ${histPath}/histo_Wjets_Alpgen_${mth}.root
    hadd -f ${histPath}/histo_Wjets_Alpgen_${mth}.root   \
	${histPath}/histo_WenuNp?.*_${mth}.root  \
	${histPath}/histo_WmunuNp?.*_${mth}.root  \
	${histPath}/histo_WtaunuNp?.*_${mth}.root \
	${histPath}/histo_WbbNp?.*_${mth}.root  \
	${histPath}/histo_WccNp?.*_${mth}.root  \
	${histPath}/histo_WcNp?.*_${mth}.root   \
	${histPath}/histo_WgammaNp?.*_${mth}.root
fi

if [ $doMergeWjetBB ]; then
    rm -f ${histPath}/histo_mcFake_Alpgen_${mth}.root
    hadd -f ${histPath}/histo_mcFake_Alpgen_${mth}.root   \
	${histPath}/histo_Wjets_Alpgen_${mth}.root 
    #TODO add BB/CC

    rm -f ${histPath}/histo_mcFake_Sherpa_${mth}.root
    hadd -f ${histPath}/histo_mcFake_Sherpa_${mth}.root   \
	${histPath}/histo_Wjets_Sherpa_${mth}.root 
    #TODO add BB/CC
fi



if [ $doSignal ]; then
#Simplified Model ModeA w/Slepton
    echo "Merge ModeA w/Slepton "
    rm -f ${histPath}/histo_SimplifiedModel_wA_slep_${mth}.root
    hadd -f ${histPath}/histo_SimplifiedModel_wA_slep_${mth}.root \
	${histPath}/histo_Herwigpp_simplifiedModel_wA_slep_noWcascade_*.*_${mth}.root
    
#echo "Merge ModeC w/Slepton "
    rm -f ${histPath}/histo_SimplifiedModel_wC_slep_${mth}.root
    hadd -f ${histPath}/histo_SimplifiedModel_wC_slep_${mth}.root \
	${histPath}/histo_Herwigpp_simplifiedModel_wC_slep_noWcascade_*.*_${mth}.root

#Simplified Model ModeA no Slepton
    echo "Merge ModeA no Slepton "
    rm -f ${histPath}/histo_SimplifiedModel_wA_noslep_lepW_${mth}.root
    hadd -f ${histPath}/histo_SimplifiedModel_wA_noslep_lepW_${mth}.root \
	${histPath}/histo_Herwigpp_simplifiedModel_wA_noslep_lepW_*.*_${mth}.root
    
#echo "Merge ModeC no Slepton "
    rm -f ${histPath}/histo_SimplifiedModel_wC_noslep_lepW_${mth}.root
    hadd -f ${histPath}/histo_SimplifiedModel_wC_noslep_lepW_${mth}.root \
	${histPath}/histo_Herwigpp_simplifiedModel_wC_noslep_lepW_*.*_${mth}.root

#echo "Merge DGemtR50_TB6 "
    rm -f ${histPath}/histo_pMSSM_DGemtR50_TB6_${mth}.root
    hadd -f ${histPath}/histo_pMSSM_DGemtR50_TB6_${mth}.root \
	${histPath}/histo_Herwigpp_UEEE3_CTEQ6L1_DGemtR50_TB6_M1M2MU_*.*_${mth}.root

#echo "Merge DGemtR50_TB6 "
    rm -f ${histPath}/histo_pMSSM_DGstauR_TB50_${mth}.root
    hadd -f ${histPath}/histo_pMSSM_DGstauR_TB50_${mth}.root \
	${histPath}/histo_Herwigpp_DGstauR_TB50_M1M2MU_*.*_${mth}.root


fi


if [ $doMove ]; then
#move all the histos to new area:
    if [ ! -d "${outPath}" ]; then
	mkdir ${outPath}
	mkdir ${outPath}/logs
    fi
    mv ${histPath}/*.root ${outPath}
    mv ${logPath}/*.log ${outPath}/logs
fi
