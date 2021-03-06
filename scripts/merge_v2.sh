#!/bin/bash
#Match dir name constructed in SusyAnaCommon.h

date="100512_13fb_n0102_std"
mth="std"

#date="100412_1fb_n0102_std"
#mth="std"

#date="100212_1fb_n0102_DD"
#mth="rlep"
#mth="std"

logPath=${WORKAREA}/SusyWeakProdAna/run/jobLogs
histPathBase=${WORKAREA}/histoAna/SusyAna
outPath=${WORKAREA}/histoAna/SusyAna/histos_${date}

histPath=${outPath}

#################

#doMove=true

doData=true
#doDataFake=true

#doTopPowHeg=true
#doTopMCNLO=true
#doTopSherpa=true
#doTopDilSherpa=true

#doWWSherpa=true;
#doWWPowHeg=true;

##doDiBHerwig=false
##doDiBMCNLO=false

#dodiBZSherpa=true
#dodiBXPowHeg=true

#doZjetAlpgen=true
#doZTauTaujetAlpgen=true

#doZjetSherpaIncl=true
#doZTauTaujetSherpaIncl=true

#doZjetSherpaLFHF=true
#doZTauTaujetSherpaLFHF=true

#doZXSherpa=true
#doZXSherpaLFHF=true

#doWjetSherpa=true
#doWjetAlpgen=true

#doMergeWjetBB=true

#doSignal=true

#######################################################################

if [ $doMove ]; then
#move all the histos to new area:
    if [ ! -d "${outPath}" ]; then
	mkdir ${outPath}
	mkdir ${outPath}/logs
    fi
    mv ${histPathBase}/*.root ${outPath}
    mv ${logPath}/*.log ${outPath}/logs
fi


#######################################################################

if [ $doData ]; then
    echo "Merge data STD"
    rm -f ${histPath}/histo_data12_std.root
    hadd -f ${histPath}/histo_data12_std.root \
	${histPath}/histo_Egamma.periodA_std.root \
	${histPath}/histo_Muons.periodA_std.root \
	${histPath}/histo_Egamma.periodB_std.root \
	${histPath}/histo_Muons.periodB_std.root  \
	${histPath}/histo_Egamma.periodC_std.root \
	${histPath}/histo_Muons.periodC_std.root \
	${histPath}/histo_Egamma.periodD_std.root \
	${histPath}/histo_Muons.periodD_std.root \
	${histPath}/histo_Egamma.periodE?_std.root \
	${histPath}/histo_Muons.periodE?_std.root 
fi 

if [ $doDataFake ]; then
    echo "Merge data DD"
    rm -f ${histPath}/histo_data12_fake.root
    hadd -f ${histPath}/histo_data12_fake.root \
	${histPath}/histo_Egamma.periodA_flep.root \
	${histPath}/histo_Muons.periodA_flep.root \
	${histPath}/histo_Egamma.periodB_flep.root \
	${histPath}/histo_Muons.periodB_flep.root  \
	${histPath}/histo_Egamma.periodC_flep.root \
	${histPath}/histo_Muons.periodC_flep.root \
	${histPath}/histo_Egamma.periodD_flep.root \
	${histPath}/histo_Muons.periodD_flep.root \
	${histPath}/histo_Egamma.periodE?_flep.root \
	${histPath}/histo_Muons.periodE?_flep.root 
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

if [ $doTopSherpa ]; then
    echo "Merge Top Sherpa"
    rm -f ${histPath}/histo_top_Sherpa_${mth}.root
    hadd -f ${histPath}/histo_top_Sherpa_${mth}.root   \
	${histPath}/histo_Ttbar*.11780?_${mth}.root \
	${histPath}/histo_SingleTopSChan*.*_${mth}.root  \
	${histPath}/histo_singletop_tchan*.*_${mth}.root  \
	${histPath}/histo_SingleTopWtChanIncl.108346_${mth}.root  \
	${histPath}/histo_ttbarW.119353_${mth}.root \
	${histPath}/histo_ttbarWj.119354_${mth}.root \
	${histPath}/histo_ttbarZ.119355_${mth}.root \
	${histPath}/histo_ttbarZj.119356_${mth}.root 
fi

if [ $doTopDilSherpa ]; then
    echo "Merge Top Dil Sherpa"
    rm -f ${histPath}/histo_topDil_Sherpa_${mth}.root
    hadd -f ${histPath}/histo_topDil_Sherpa_${mth}.root   \
	${histPath}/histo_Ttbar*.117800_${mth}.root \
	${histPath}/histo_Ttbar*.117801_${mth}.root \
	${histPath}/histo_Ttbar*.117802_${mth}.root \
	${histPath}/histo_Ttbar*.117804_${mth}.root \
	${histPath}/histo_Ttbar*.117806_${mth}.root \
	${histPath}/histo_Ttbar*.117809_${mth}.root \
	${histPath}/histo_SingleTopWtChanIncl.108346_${mth}.root \
	${histPath}/histo_ttbarW.119353_${mth}.root \
	${histPath}/histo_ttbarWj.119354_${mth}.root \
	${histPath}/histo_ttbarZ.119355_${mth}.root \
	${histPath}/histo_ttbarZj.119356_${mth}.root 

#	${histPath}/histo_Ttbar*.117805_${mth}.root \
#	${histPath}/histo_Ttbar*.117808_${mth}.root \

fi




if [ $doWWSherpa ]; then
#Sherpa Diboson
    echo "Merge WW Sherpa "
    rm -f ${histPath}/histo_WW_Sherpa_${mth}.root
    hadd -f ${histPath}/histo_WW_Sherpa_${mth}.root \
	${histPath}/histo_llnunu_WW.126892_${mth}.root \
	${histPath}/histo_WpWm*.106*_${mth}.root  \
	${histPath}/histo_llnunujj.126989_${mth}.root
    #TODO add 126988
fi

if [ $doWWPowHeg ]; then
#Diboson PowHegPythia and Wg AlpgenJimmy + Zg Sherpa
    echo "Merge WW Powheg "
    rm -f ${histPath}/histo_WW_Powheg_${mth}.root
    hadd -f ${histPath}/histo_WW_Powheg_${mth}.root \
	${histPath}/histo_WpWm*.126*_${mth}.root \
	${histPath}/histo_WpWm*.106*_${mth}.root  \
	${histPath}/histo_llnunujj.126989_${mth}.root
    #TODO add 126988
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

if [ $dodiBZSherpa ]; then
#Sherpa Diboson
    echo "Merge diBoson ZX Sherpa "
    rm -f ${histPath}/histo_diBZX_Sherpa_${mth}.root
    hadd -f ${histPath}/histo_diBZX_Sherpa_${mth}.root \
	${histPath}/histo_llll_ZZ.126894_${mth}.root \
	${histPath}/histo_llnunu_ZZ.126895_${mth}.root \
	${histPath}/histo_lllnu_WZ.126893_${mth}.root \
	${histPath}/histo_ZZ*.11660?_${mth}.root 
fi

if [ $dodiBZPowHeg ]; then
#Diboson PowHegPythia and Wg AlpgenJimmy + Zg Sherpa
    echo "Merge diBoson ZX Powheg "
    rm -f ${histPath}/histo_diBZX_Powheg_${mth}.root
    hadd -f ${histPath}/histo_diBZX_Powheg_${mth}.root \
	${histPath}/histo_WZ.1294*_${mth}.root \
	${histPath}/histo_ZZ*.1269*_${mth}.root \
	${histPath}/histo_ZZ*.11660?_${mth}.root 
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
	${histPath}/histo_ZeeccNp?.*_${mth}.root \
	${histPath}/histo_ZmumuNp?.*_${mth}.root \ 
	${histPath}/histo_ZmumuNp?Excl_Mll10to60.*_mll60_${mth}.root  \
	${histPath}/histo_ZmumubbNp?.*_${mth}.root  \
	${histPath}/histo_ZmumuccNp?.*_${mth}.root  
fi

if [ $doZTauTaujetAlpgen ]; then
#Z+jets Alpgen
    echo "Merge Ztautau+jets Alpgen "
    rm -f ${histPath}/histo_ZTauTaujets_Alpgen_${mth}.root
    hadd -f ${histPath}/histo_ZTauTaujets_Alpgen_${mth}.root  \
	${histPath}/histo_ZtautauNp?.*_${mth}.root  \
	${histPath}/histo_ZtautauNp?Excl_Mll10to60.*_mll60_${mth}.root  \
	${histPath}/histo_ZtautaubbNp?.*_${mth}.root  \
	${histPath}/histo_ZtautauccNp?.*_${mth}.root  
fi

##
## Sherpa samples path with low mass mll<40 Alpgen (buggy)
##
if [ $doZjetSherpaIncl ]; then
#Z+jets Sherpa
    echo "Merge Z+jets Sherpa Inclusive + low mass Alpgen"
    rm -f ${histPath}/histo_Zjets_Sherpa_${mth}.root
    hadd -f ${histPath}/histo_Zjets_Sherpa_${mth}.root  \
	${histPath}/histo_Zee.147770_${mth}.root \
	${histPath}/histo_Zmumu.147771_${mth}.root \
	${histPath}/histo_ZeeNp?Excl_Mll10to60.*_mll40_${mth}.root  \
	${histPath}/histo_ZmumuNp?Excl_Mll10to60.*_mll40_${mth}.root  
fi    

if [ $doZTauTaujetSherpaIncl ]; then
#Z+jets Sherpa
    echo "Merge ZTauTau+jets Sherpa Inclusive + low mass Alpgen "
    rm -f ${histPath}/histo_ZTauTaujets_Sherpa_${mth}.root
    hadd -f ${histPath}/histo_ZTauTaujets_Sherpa_${mth}.root  \
	${histPath}/histo_Ztautau.147772_${mth}.root \
	${histPath}/histo_ZtautauNp?Excl_Mll10to60.*_mll40_${mth}.root 
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
	${histPath}/histo_ZeeNp?Excl_Mll10to60.*_mll40_${mth}.root  \
	${histPath}/histo_ZmumuNp?Excl_Mll10to60.*_mll40_${mth}.root  
fi 

if [ $doZTauTaujetSherpaLFHF ]; then
#Z+jets Sherpa
    echo "Merge ZTauTau+jets Sherpa LF+HF + low mass Alpgen"
    rm -f ${histPath}/histo_ZTauTaujets_SherpaLFHF_${mth}.root
    hadd -f ${histPath}/histo_ZTauTaujets_SherpaLFHF_${mth}.root  \
	${histPath}/histo_ZtautauLightJets.146822_${mth}.root \
	${histPath}/histo_ZtautauHeavyJets.128977_${mth}.root \
	${histPath}/histo_ZtautauNp?Excl_Mll10to60.*_mll40_${mth}.root 
fi 

#
# Merge diBZ & ZJets (ee/mm)
#

if [ $doZXSherpa ]; then
    echo "Merge ZX+jets Sherpa "
    rm -f ${histPath}/histo_ZX_Sherpa_${mth}.root
    hadd -f ${histPath}/histo_ZX_Sherpa_${mth}.root \
	${histPath}/histo_diBZX_Sherpa_${mth}.root   \
	${histPath}/histo_Zjets_Sherpa_${mth}.root 
fi

if [ $doZXSherpaLFHF ]; then
    echo "Merge ZX+jets SherpaLFHF "
    rm -f ${histPath}/histo_ZX_SherpaLFHF_${mth}.root
    hadd -f ${histPath}/histo_ZX_SherpaLFHF_${mth}.root \
	${histPath}/histo_diBZX_Sherpa_${mth}.root   \
	${histPath}/histo_Zjets_SherpaLFHF_${mth}.root  
fi


### TO Do add alpgen buggy full mll



if [ $doWjetSherpa ]; then
#W+jets Sherpa
    echo "Merge W+jets Sherpa "
    rm -f ${histPath}/histo_Wjets_Sherpa_${mth}.root
    hadd -f ${histPath}/histo_Wjets_Sherpa_${mth}.root   \
	${histPath}/histo_Wenu*.147774*_${mth}.root  \
	${histPath}/histo_Wmunu*.147775*_${mth}.root  \
	${histPath}/histo_Wtaunu*.147776*_${mth}.root  \
	${histPath}/histo_enugammaPt10.126739*_${mth}.root  \
	${histPath}/histo_munugammaPt10.126742*_${mth}.root  \
	${histPath}/histo_taunugammaPt10.126856*_${mth}.root  
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

