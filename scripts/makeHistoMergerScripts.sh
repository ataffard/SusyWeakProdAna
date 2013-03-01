#!/bin/bash
#Match dir name constructed in SusyAnaCommon.h

date="021713_21fb_n0127_Moriond_DD_v4"

#date="020813_13fb_n0124_HCP_DD_NEWJET"

#date="020813_21fb_n0123_Moriond_Fake_DD"

#date="020613_13fb_n0123_HCP_DD"
#date="020613_13fb_n0124_HCP_DD"

#date="020413_5fb_n0123_IJL_DD"

mth="rlep"

pathScript=${WORKAREA}/SusyWeakProdAna/scripts
logPath=${WORKAREA}/SusyWeakProdAna/run/jobLogs
histPathBase=${WORKAREA}/histoAna/SusyAna
outPath=${WORKAREA}/histoAna/SusyAna/histos_${date}

histPath=${outPath}

echo "Output dir set to ${histPath}"

#################

doMove=true

doData=true
doDataFake=true

doTopPowHeg=true
doTopAlpgen=true
doTopMCNLO=true
#doTopSherpa=true
#doTopDilSherpa=true

doWWSherpa=true;
#doWWPowHeg=true;

##doDiBHerwig=false
##doDiBMCNLO=false

dodiBosonSherpa=true

dodiBZSherpa=true
#dodiBZPowHeg=true

doZjetAlpgen=true
doZTauTaujetAlpgen=true

doZjetSherpaIncl=true
doZTauTaujetSherpaIncl=true

doZjetSherpaLFHF=true
doZTauTaujetSherpaLFHF=true

#doZjetPowheg=true
#doZTauTaujetPowheg=true

doZXSherpa=true
#doZXSherpaPowheg=true
doZXSherpaLFHF=true
doZXAlpgen=true
#doZXPowheg=true

#doWjetSherpa=true
doWjetAlpgen=true

doMergeWjetBB=true

doBkgDD=true

doSignal=true

#######################################################################

if [ $doMove ]; then
#move all the histos to new area:
    if [ ! -d "${outPath}" ]; then
	mkdir ${outPath}
	mkdir ${outPath}/logs
    fi
    mv ${histPathBase}/*.root ${outPath}
    mv ${logPath}/*.log ${outPath}/logs
    mv ${histPathBase}/HFTOutputs ${outPath}
fi

#######################################################################
#create dir for submission scripts
mergeJobDir="${pathScript}/MergeJobsScripts"
echo ${mergeJobDir}
if [ ! -d "${mergeJobDir}" ]; then
    mkdir ${mergeJobDir}
fi


#######################################################################

if [ $doData ]; then
    echo "Merge data STD"
    rm -f ${histPath}/histo_data12_std.root
    SCRIPT=${mergeJobDir}/data12_std_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
hadd -f ${histPath}/histo_data12_std.root \
	${histPath}/histo_Egamma.period?_std.root \
    	${histPath}/histo_Muons.period?_std.root 
EOF
    chmod 755 $SCRIPT
fi 
#	${histPath}/histo_Egamma.periodA_MISSING_std.root \


if [ $doDataFake ]; then
    echo "Merge data DD"
    rm -f ${histPath}/histo_data12_fake.root
    SCRIPT=${mergeJobDir}/data12_fake_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
hadd -f ${histPath}/histo_data12_fake.root \
	${histPath}/histo_Egamma.period?_flep.root \
	${histPath}/histo_Muons.period?_flep.root 
EOF
    chmod 755 $SCRIPT
fi 
#	${histPath}/histo_Egamma.periodA_MISSING_flep.root \


#
# mc12a
#

if [ $doTopMCNLO ]; then
    echo "Merge Top MC@NLO"
    rm -f ${histPath}/histo_top_MCNLO_${mth}.root
    SCRIPT=${mergeJobDir}/top_MCNLO_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_top_MCNLO_${mth}.root   \
	${histPath}/histo_ttbar.105200_${mth}.root \
	${histPath}/histo_SingleTopWtChanIncl.108346_${mth}.root  \
	${histPath}/histo_ttbarW.119353_${mth}.root \
	${histPath}/histo_ttbarWj.119354_${mth}.root \
	${histPath}/histo_ttbarZ.119355_${mth}.root \
	${histPath}/histo_ttbarZj.119356_${mth}.root \
	${histPath}/histo_ttbarWW.119583_${mth}.root 
EOF
    chmod 755 $SCRIPT
fi
#	${histPath}/histo_SingleTopSChan*.*_${mth}.root  \
#	${histPath}/histo_singletop_tchan*.*_${mth}.root  \


if [ $doTopPowHeg ]; then
    echo "Merge Top POWHEG"
    rm -f ${histPath}/histo_top_PowHeg_${mth}.root
    SCRIPT=${mergeJobDir}/top_PowHeg_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_top_PowHeg_${mth}.root   \
	${histPath}/histo_ttbar.105861_${mth}.root \
	${histPath}/histo_ttbarW.119353_${mth}.root \
	${histPath}/histo_ttbarWj.119354_${mth}.root \
	${histPath}/histo_ttbarZ.119355_${mth}.root \
	${histPath}/histo_ttbarZj.119356_${mth}.root \
	${histPath}/histo_ttbarWW.119583_${mth}.root \
	${histPath}/histo_SingleTopWtChanIncl.108346_${mth}.root  
EOF
#	${histPath}/histo_SingleTopSChan*.*_${mth}.root  \
#	${histPath}/histo_singletop_tchan*.*_${mth}.root  \


    chmod 755 $SCRIPT
fi

if [ $doTopAlpgen ]; then
    echo "Merge Top ALPGEN"
    rm -f ${histPath}/histo_top_Alpgen_${mth}.root
    SCRIPT=${mergeJobDir}/top_Alpgen_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_top_Alpgen_${mth}.root   \
        ${histPath}/histo_ttbarln*.1644*_rlep.root \
	${histPath}/histo_ttbarW.119353_${mth}.root \
	${histPath}/histo_ttbarWj.119354_${mth}.root \
	${histPath}/histo_ttbarZ.119355_${mth}.root \
	${histPath}/histo_ttbarZj.119356_${mth}.root \
	${histPath}/histo_ttbarWW.119583_${mth}.root \
	${histPath}/histo_SingleTopWtChanIncl.108346_${mth}.root  
EOF
#	${histPath}/histo_SingleTopSChan*.*_${mth}.root  \
#	${histPath}/histo_singletop_tchan*.*_${mth}.root  \


    chmod 755 $SCRIPT
fi


if [ $doTopSherpa ]; then
    echo "Merge Top Sherpa"
    rm -f ${histPath}/histo_top_Sherpa_${mth}.root
    SCRIPT=${mergeJobDir}/top_Sherpa_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_top_Sherpa_${mth}.root   \
	${histPath}/histo_Ttbar*.11780?_${mth}.root \
	${histPath}/histo_SingleTopWtChanIncl.108346_${mth}.root  \
	${histPath}/histo_ttbarW.119353_${mth}.root \
	${histPath}/histo_ttbarWj.119354_${mth}.root \
	${histPath}/histo_ttbarZ.119355_${mth}.root \
	${histPath}/histo_ttbarZj.119356_${mth}.root \
	${histPath}/histo_ttbarWW.119583_${mth}.root 
EOF
    chmod 755 $SCRIPT
#Included in fake lepton bkg
#	${histPath}/histo_SingleTopSChan*.*_${mth}.root  \
#	${histPath}/histo_singletop_tchan*.*_${mth}.root  \

fi

if [ $doTopDilSherpa ]; then
    echo "Merge Top Dil Sherpa"
    rm -f ${histPath}/histo_topDil_Sherpa_${mth}.root
    SCRIPT=${mergeJobDir}/topDil_Sherpa_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_topDil_Sherpa_${mth}.root   \
	${histPath}/histo_Ttbar*.117800_${mth}.root \
	${histPath}/histo_Ttbar*.117801_${mth}.root \
	${histPath}/histo_Ttbar*.117802_${mth}.root \
	${histPath}/histo_SingleTopWtChanIncl.108346_${mth}.root \
	${histPath}/histo_ttbarW.119353_${mth}.root \
	${histPath}/histo_ttbarWj.119354_${mth}.root \
	${histPath}/histo_ttbarZ.119355_${mth}.root \
	${histPath}/histo_ttbarZj.119356_${mth}.root \
	${histPath}/histo_ttbarWW.119583_${mth}.root 
EOF
    chmod 755 $SCRIPT

fi




if [ $doWWSherpa ]; then
#Sherpa Diboson
    echo "Merge WW Sherpa "
    rm -f ${histPath}/histo_WW_Sherpa_${mth}.root
    SCRIPT=${mergeJobDir}/WW_Sherpa_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_WW_Sherpa_${mth}.root \
	${histPath}/histo_llnunu_WW.126892_${mth}.root \
	${histPath}/histo_WpWm*.106*_${mth}.root  \
	${histPath}/histo_llnunu.126988_${mth}.root\
	${histPath}/histo_llnunujj.126989_${mth}.root
EOF
    chmod 755 $SCRIPT
fi

if [ $doWWPowHeg ]; then
#Diboson PowHegPythia and Wg AlpgenJimmy + Zg Sherpa
    echo "Merge WW Powheg "
    rm -f ${histPath}/histo_WW_Powheg_${mth}.root
    SCRIPT=${mergeJobDir}/WW_Powheg_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_WW_Powheg_${mth}.root \
	${histPath}/histo_WpWm*.126*_${mth}.root \
	${histPath}/histo_WpWm*.106*_${mth}.root  \
	${histPath}/histo_llnunu.126988_${mth}.root \
	${histPath}/histo_llnunujj.126989_${mth}.root
EOF
    chmod 755 $SCRIPT
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

if [ $dodiBosonSherpa ]; then
#Sherpa Diboson
    echo "Merge diBoson Sherpa "
    rm -f ${histPath}/histo_diBoson_Sherpa_${mth}.root
    SCRIPT=${mergeJobDir}/diBoson_Sherpa_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_diBoson_Sherpa_${mth}.root \
	${histPath}/histo_llnunu_WW.126892_${mth}.root \
	${histPath}/histo_WpWm*.106*_${mth}.root  \
	${histPath}/histo_llnunu.126988_${mth}.root\
	${histPath}/histo_llnunujj.126989_${mth}.root \
	${histPath}/histo_llll_ZZ.126894_${mth}.root \
	${histPath}/histo_llnunu_ZZ.126895_${mth}.root \
	${histPath}/histo_lllnu_WZ.126893_${mth}.root \
	${histPath}/histo_ZZ*.11660?_${mth}.root 
EOF
    chmod 755 $SCRIPT
fi





if [ $dodiBZSherpa ]; then
#Sherpa Diboson
    echo "Merge diBoson ZX Sherpa "
    rm -f ${histPath}/histo_diBZX_Sherpa_${mth}.root
   SCRIPT=${mergeJobDir}/diBZX_Sherpa_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_diBZX_Sherpa_${mth}.root \
	${histPath}/histo_llll_ZZ.126894_${mth}.root \
	${histPath}/histo_llnunu_ZZ.126895_${mth}.root \
	${histPath}/histo_lllnu_WZ.126893_${mth}.root \
	${histPath}/histo_ZZ*.11660?_${mth}.root 
EOF
    chmod 755 $SCRIPT
fi

if [ $dodiBZPowHeg ]; then
#Diboson PowHegPythia and Wg AlpgenJimmy + Zg Sherpa
    echo "Merge diBoson ZX Powheg "
    rm -f ${histPath}/histo_diBZX_Powheg_${mth}.root
    SCRIPT=${mergeJobDir}/diBZX_Powheg_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_diBZX_Powheg_${mth}.root \
	${histPath}/histo_WZ.1294*_${mth}.root \
	${histPath}/histo_ZZ*.1269*_${mth}.root \
	${histPath}/histo_ZZ*.11660?_${mth}.root 
EOF
    chmod 755 $SCRIPT
fi 

#Buggy
if [ $doZjetAlpgen ]; then
#Z+jets Alpgen
    echo "Merge Z+jets Alpgen+Pythia + low mass Alpgen+Herwig "
    rm -f ${histPath}/histo_Zjets_Alpgen_${mth}.root
    SCRIPT=${mergeJobDir}/Zjets_Alpgen_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
     hadd -f ${histPath}/histo_ZeeJets_Alpgen_${mth}.root  \
	${histPath}/histo_ZeeNp?.117*_${mth}.root  \
	${histPath}/histo_ZeeNp?Excl_Mll10to60.*_mll60_${mth}.root  \
	${histPath}/histo_ZeeNp5Incl_Mll10to60.*_mll60_${mth}.root  \
	${histPath}/histo_ZeebbNp?.110*_${mth}.root \
	${histPath}/histo_ZeeccNp?.110*_${mth}.root 


    hadd -f ${histPath}/histo_ZmumuJets_Alpgen_${mth}.root  \
	${histPath}/histo_ZmumuNp?.117*_${mth}.root \
	${histPath}/histo_ZmumuNp?Excl_Mll10to60.*_mll60_${mth}.root  \
	${histPath}/histo_ZmumuNp5Incl_Mll10to60.*_mll60_${mth}.root  \
	${histPath}/histo_ZmumubbNp?.110*_${mth}.root  \
	${histPath}/histo_ZmumuccNp?.110*_${mth}.root  


    hadd -f ${histPath}/histo_Zjets_Alpgen_${mth}.root  \
	${histPath}/histo_ZeeJets_Alpgen_${mth}.root \
	${histPath}/histo_ZmumuJets_Alpgen_${mth}.root  
EOF
    chmod 755 $SCRIPT
fi

if [ $doZTauTaujetAlpgen ]; then
#Z+jets Alpgen
    echo "Merge Ztautau+jets Alpgen "
    rm -f ${histPath}/histo_ZTauTaujets_Alpgen_${mth}.root
    SCRIPT=${mergeJobDir}/ZTauTaujets_Alpgen_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_ZTauTaujets_Alpgen_${mth}.root  \
	${histPath}/histo_ZtautauNp?.117*_${mth}.root  \
	${histPath}/histo_ZtautauNp?Excl_Mll10to60.*_mll60_${mth}.root  \
	${histPath}/histo_ZtautauNp5Incl_Mll10to60.*_mll60_${mth}.root  \
	${histPath}/histo_ZtautaubbNp?.110*_${mth}.root  \
	${histPath}/histo_ZtautauccNp?.110*_${mth}.root  
EOF
    chmod 755 $SCRIPT
fi

##
## Sherpa samples path with low mass mll<40 Alpgen (buggy)
##
if [ $doZjetSherpaIncl ]; then
#Z+jets Sherpa
    echo "Merge Z+jets Sherpa Inclusive + low mass Alpgen"
    rm -f ${histPath}/histo_Zjets_Sherpa_${mth}.root
    SCRIPT=${mergeJobDir}/Zjets_Sherpa_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}

##TO DO Add ee/mm PT10 145* - are these overlapping    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_Zjets_Sherpa_${mth}.root  \
	${histPath}/histo_Zee.147770_${mth}.root \
	${histPath}/histo_Zmumu.147771_${mth}.root \
        ${histPath}/histo_DYee*_${mth}.root \
        ${histPath}/histo_DYmumu*_${mth}.root 
EOF
    chmod 755 $SCRIPT
#	${histPath}/histo_ZeeNp?Excl_Mll10to60.*_mll40_${mth}.root  \
#	${histPath}/histo_ZeeNp5Incl_Mll10to60.*_mll40_${mth}.root  \
#	${histPath}/histo_ZmumuNp?Excl_Mll10to60.*_mll40_${mth}.root  \
#	${histPath}/histo_ZmumuNp5Incl_Mll10to60.*_mll40_${mth}.root  

fi    

if [ $doZTauTaujetSherpaIncl ]; then
#Z+jets Sherpa
    echo "Merge ZTauTau+jets Sherpa Inclusive + low mass Alpgen "
    rm -f ${histPath}/histo_ZTauTaujets_Sherpa_${mth}.root
    SCRIPT=${mergeJobDir}/ZTauTaujets_Sherpa_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_ZTauTaujets_Sherpa_${mth}.root  \
	${histPath}/histo_Ztautau.147772_${mth}.root \
        ${histPath}/histo_DYtautau*_${mth}.root 
EOF
    chmod 755 $SCRIPT
#	${histPath}/histo_ZtautauNp?Excl_Mll10to60.*_mll40_${mth}.root \
#	${histPath}/histo_ZtautauNp5Incl_Mll10to60.*_mll40_${mth}.root  
fi 



if [ $doZjetSherpaLFHF ]; then
#Z+jets Sherpa
    echo "Merge Z+jets Sherpa LF+HF + low mass Alpgen"
    rm -f ${histPath}/histo_Zjets_SherpaLFHF_${mth}.root
    SCRIPT=${mergeJobDir}/Zjets_SherpaLFHF_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_Zjets_SherpaLFHF_${mth}.root  \
	${histPath}/histo_ZeeLightJets.146820_${mth}.root \
	${histPath}/histo_ZeeHeavyJets.128975_${mth}.root \
	${histPath}/histo_ZmumuLightJets.146821_${mth}.root \
	${histPath}/histo_ZmumuHeavyJets.128976_${mth}.root \
        ${histPath}/histo_DYee*_${mth}.root \
        ${histPath}/histo_DYmumu*_${mth}.root 
EOF
    chmod 755 $SCRIPT
#	${histPath}/histo_ZeeNp?Excl_Mll10to60.*_mll40_${mth}.root  \
#	${histPath}/histo_ZeeNp5Incl_Mll10to60.*_mll40_${mth}.root  \
#	${histPath}/histo_ZmumuNp?Excl_Mll10to60.*_mll40_${mth}.root \
#	${histPath}/histo_ZmumuNp5Incl_Mll10to60.*_mll40_${mth}.root   
fi 

if [ $doZTauTaujetSherpaLFHF ]; then
#Z+jets Sherpa
    echo "Merge ZTauTau+jets Sherpa LF+HF + low mass Alpgen"
    rm -f ${histPath}/histo_ZTauTaujets_SherpaLFHF_${mth}.root
   SCRIPT=${mergeJobDir}/ZTauTaujets_SherpaLFHF_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_ZTauTaujets_SherpaLFHF_${mth}.root  \
	${histPath}/histo_ZtautauLightJets.146822_${mth}.root \
	${histPath}/histo_ZtautauHeavyJets.128977_${mth}.root \
        ${histPath}/histo_DYtautau*_${mth}.root 
EOF
    chmod 755 $SCRIPT
#	${histPath}/histo_ZtautauNp?Excl_Mll10to60.*_mll40_${mth}.root \
#	${histPath}/histo_ZtautauNp5Incl_Mll10to60.*_mll40_${mth}.root  

fi 



if [ $doZjetPowheg ]; then
#Z+jets Powheg
    echo "Merge Z+jets Powheg "
    rm -f ${histPath}/histo_Zjets_Powheg_${mth}.root
    SCRIPT=${mergeJobDir}/Zjets_Powheg_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
     hadd -f ${histPath}/histo_ZeeJets_Powheg_${mth}.root  \
	${histPath}/histo_Zee.147806_${mth}.root  \
	${histPath}/histo_DYee.129501_${mth}.root  

    hadd -f ${histPath}/histo_ZmumuJets_Powheg_${mth}.root  \
	${histPath}/histo_Zmumu.147807_${mth}.root \
	${histPath}/histo_DYmumu.129521_${mth}.root  

    hadd -f ${histPath}/histo_Zjets_Powheg_${mth}.root  \
	${histPath}/histo_ZeeJets_Powheg_${mth}.root \
	${histPath}/histo_ZmumuJets_Powheg_${mth}.root  
EOF
    chmod 755 $SCRIPT
fi

if [ $doZTauTaujetPowheg ]; then
#Z+jets Powheg - no low mass !!!
    echo "Merge Ztautau+jets Powheg"
    rm -f ${histPath}/histo_ZTauTaujets_Powheg_${mth}.root
    SCRIPT=${mergeJobDir}/ZTauTaujets_Powheg_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_ZTauTaujets_Powheg_${mth}.root  \
	${histPath}/histo_Ztautau.147808_${mth}.root  

EOF
    chmod 755 $SCRIPT
fi



#
# Merge diBZ & ZJets (ee/mm)
#

if [ $doZXSherpa ]; then
    echo "Merge ZX+jets Sherpa "
    rm -f ${histPath}/histo_ZX_Sherpa_${mth}.root
    SCRIPT=${mergeJobDir}/ZX_Sherpa_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_ZX_Sherpa_${mth}.root \
	${histPath}/histo_diBZX_Sherpa_${mth}.root   \
	${histPath}/histo_Zjets_Sherpa_${mth}.root 
EOF
    chmod 755 $SCRIPT
fi

if [ $doZXSherpaPowheg ]; then
    echo "Merge ZX+jets Sherpa "
    rm -f ${histPath}/histo_ZX_SherpaPowheg_${mth}.root
    SCRIPT=${mergeJobDir}/ZX_SherpaPowheg_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_ZX_SherpaPowheg_${mth}.root \
	${histPath}/histo_diBZX_Powheg_${mth}.root   \
	${histPath}/histo_Zjets_Sherpa_${mth}.root 
EOF
    chmod 755 $SCRIPT
fi



if [ $doZXSherpaLFHF ]; then
    echo "Merge ZX+jets SherpaLFHF "
    rm -f ${histPath}/histo_ZX_SherpaLFHF_${mth}.root
    SCRIPT=${mergeJobDir}/ZX_SherpaLFHF_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_ZX_SherpaLFHF_${mth}.root \
	${histPath}/histo_diBZX_Sherpa_${mth}.root   \
	${histPath}/histo_Zjets_SherpaLFHF_${mth}.root  
EOF
    chmod 755 $SCRIPT
fi

if [ $doZXAlpgen ]; then
    echo "Merge ZX+jets Alpgen "
    rm -f ${histPath}/histo_ZX_Alpgen_${mth}.root
    SCRIPT=${mergeJobDir}/ZX_Alpgen_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_ZX_Alpgen_${mth}.root \
	${histPath}/histo_diBZX_Sherpa_${mth}.root  \
	${histPath}/histo_Zjets_Alpgen_${mth}.root 
EOF
    chmod 755 $SCRIPT
fi


if [ $doZXPowheg ]; then
    echo "Merge ZX+jets Powheg "
    rm -f ${histPath}/histo_ZX_Powheg_${mth}.root
    SCRIPT=${mergeJobDir}/ZX_Powheg_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_ZX_Powheg_${mth}.root \
	${histPath}/histo_diBZX_Sherpa_${mth}.root  \
	${histPath}/histo_Zjets_Powheg_${mth}.root 
EOF
    chmod 755 $SCRIPT
fi

if [ $doWjetSherpa ]; then
#W+jets Sherpa
    echo "Merge W+jets Sherpa "
    rm -f ${histPath}/histo_Wjets_Sherpa_${mth}.root
    SCRIPT=${mergeJobDir}/Wjets_Sherpa_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_Wjets_Sherpa_${mth}.root   \
	${histPath}/histo_Wenu*.147774*_${mth}.root  \
	${histPath}/histo_Wmunu*.147775*_${mth}.root  \
	${histPath}/histo_Wtaunu*.147776*_${mth}.root  \
	${histPath}/histo_enugammaPt10.126739*_${mth}.root  \
	${histPath}/histo_munugammaPt10.126742*_${mth}.root  \
	${histPath}/histo_taunugammaPt10.126856*_${mth}.root  
EOF
    chmod 755 $SCRIPT
fi

if [ $doWjetAlpgen ]; then
#W+jets Alpgen
    echo "Merge W+jets Alpgen+Pythia "
    rm -f ${histPath}/histo_Wjets_Alpgen_${mth}.root
    SCRIPT=${mergeJobDir}/Wjets_Alpgen_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_Wjets_Alpgen_${mth}.root   \
	${histPath}/histo_WenuNp?.117*_${mth}.root  \
	${histPath}/histo_WmunuNp?.117*_${mth}.root  \
	${histPath}/histo_WtaunuNp?.117*_${mth}.root \
	${histPath}/histo_WbbNp?.110*_${mth}.root  \
	${histPath}/histo_WccNp?.126*_${mth}.root  \
	${histPath}/histo_WcNp?.126*_${mth}.root   \
	${histPath}/histo_WgammaNp?.*_${mth}.root
EOF
    chmod 755 $SCRIPT
fi

if [ $doMergeWjetBB ]; then
    rm -f ${histPath}/histo_mcFake_Alpgen_${mth}.root
    SCRIPT=${mergeJobDir}/mcFake_Alpgen_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_mcFake_Alpgen_${mth}.root   \
	${histPath}/histo_Wjets_Alpgen_${mth}.root \
        ${histPath}/histo_bbTo*15*_${mth}.root \
        ${histPath}/histo_ccTo*15*_${mth}.root 
EOF
    chmod 755 $SCRIPT

    rm -f ${histPath}/histo_mcFake_Sherpa_${mth}.root
    SCRIPT=${mergeJobDir}/mcFake_Sherpa_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_mcFake_Sherpa_${mth}.root   \
	${histPath}/histo_Wjets_Sherpa_${mth}.root \
	${histPath}/histo_Ttbar*.117803_${mth}.root \
	${histPath}/histo_Ttbar*.117804_${mth}.root \
	${histPath}/histo_Ttbar*.117805_${mth}.root \
	${histPath}/histo_Ttbar*.117806_${mth}.root \
	${histPath}/histo_Ttbar*.117808_${mth}.root \
	${histPath}/histo_Ttbar*.117809_${mth}.root \
	${histPath}/histo_SingleTopSChan*.*_${mth}.root  \
	${histPath}/histo_singletop_tchan*.*_${mth}.root  \
        ${histPath}/histo_bbTo*15*_${mth}.root \
        ${histPath}/histo_ccTo*15*_${mth}.root 
EOF
    chmod 755 $SCRIPT
fi


#
# Merge All Bkg DD
#

if [ $doBkgDD ]; then
    echo "Merge DD Bkg ZAlpgen TopPowHeg"
    rm -f ${histPath}/histo_BkgZAlpgenTopPowHeg_${mth}.root
    SCRIPT=${mergeJobDir}/Bkg_ZAlpgenTopPowHeg_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_BkgZAlpgenTopPowHeg_${mth}.root \
          ${histPath}/histo_top_PowHeg_${mth}.root \
          ${histPath}/histo_data12_fake.root \
          ${histPath}/histo_WW_Sherpa_${mth}.root \
  	  ${histPath}/histo_ZX_Alpgen_${mth}.root   \
	  ${histPath}/histo_ZTauTaujets_Alpgen_${mth}.root 
EOF
    chmod 755 $SCRIPT

   echo "Merge DD Bkg ZAlpgen TopMCNLO"
    rm -f ${histPath}/histo_BkgZAlpgenTopMCNLO_${mth}.root
    SCRIPT=${mergeJobDir}/Bkg_ZAlpgenTopMCNLO_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_BkgZAlpgenTopPowHeg_${mth}.root \
          ${histPath}/histo_top_MCNLO_${mth}.root \
          ${histPath}/histo_data12_fake.root \
          ${histPath}/histo_WW_Sherpa_${mth}.root \
  	  ${histPath}/histo_ZX_Alpgen_${mth}.root   \
	  ${histPath}/histo_ZTauTaujets_Alpgen_${mth}.root 
EOF
    chmod 755 $SCRIPT



    echo "Merge DD Bkg ZAlpgen Top Alpgen" 
    rm -f ${histPath}/histo_BkgZTopAlpgen_${mth}.root
    SCRIPT=${mergeJobDir}/Bkg_ZTopAlpgen_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_BkgZTopAlpgen_${mth}.root \
          ${histPath}/histo_top_Alpgen_${mth}.root \
          ${histPath}/histo_data12_fake.root \
          ${histPath}/histo_WW_Sherpa_${mth}.root \
  	  ${histPath}/histo_ZX_Alpgen_${mth}.root   \
	  ${histPath}/histo_ZTauTaujets_Alpgen_${mth}.root 
EOF
    chmod 755 $SCRIPT


    echo "Merge DD Bkg ZSherpa TopPowHeg "
    rm -f ${histPath}/histo_BkgZSherpaTopPowHeg_${mth}.root
    SCRIPT=${mergeJobDir}/Bkg_ZSherpaTopPowHeg_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_BkgZSherpaTopPowHeg_${mth}.root \
          ${histPath}/histo_top_PowHeg_${mth}.root \
          ${histPath}/histo_data12_fake.root \
          ${histPath}/histo_WW_Sherpa_${mth}.root \
  	  ${histPath}/histo_ZX_Sherpa_${mth}.root   \
	  ${histPath}/histo_ZTauTaujets_Sherpa_${mth}.root 
EOF
    chmod 755 $SCRIPT


fi






if [ $doSignal ]; then
#Simplified Model ModeA w/Slepton
    echo "Merge ModeA w/Slepton "
    rm -f ${histPath}/histo_SimplifiedModel_wA_slep_${mth}.root
    SCRIPT=${mergeJobDir}/SimplifiedModel_wA_slep_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_SimplifiedModel_wA_slep_${mth}.root \
	${histPath}/histo_Herwigpp_simplifiedModel_wA_slep_noWcascade_*.*_${mth}.root
EOF
    chmod 755 $SCRIPT
    
    echo "Merge ModeC w/Slepton "
    rm -f ${histPath}/histo_SimplifiedModel_wC_slep_${mth}.root
    SCRIPT=${mergeJobDir}/SimplifiedModel_wC_slep_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_SimplifiedModel_wC_slep_${mth}.root \
	${histPath}/histo_Herwigpp_simplifiedModel_wC_slep_noWcascade_*.*_${mth}.root
EOF
    chmod 755 $SCRIPT

#Simplified Model ModeA no Slepton
    echo "Merge ModeA no Slepton "
    rm -f ${histPath}/histo_SimplifiedModel_wA_noslep_lepW_${mth}.root
    SCRIPT=${mergeJobDir}/SimplifiedModel_wA_noslep_lepW_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_SimplifiedModel_wA_noslep_lepW_${mth}.root \
	${histPath}/histo_Herwigpp_simplifiedModel_wA_noslep_lepW_*.*_${mth}.root
EOF
    chmod 755 $SCRIPT
    
    echo "Merge ModeC no Slepton "
    rm -f ${histPath}/histo_SimplifiedModel_wC_noslep_lepW_${mth}.root
    SCRIPT=${mergeJobDir}/SimplifiedModel_wC_noslep_lepW_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_SimplifiedModel_wC_noslep_lepW_${mth}.root \
	${histPath}/histo_Herwigpp_simplifiedModel_wC_noslep_lepW_*.*_${mth}.root
EOF
    chmod 755 $SCRIPT

    echo "Merge DGemtR50_TB6 "
    rm -f ${histPath}/histo_pMSSM_DGemtR50_TB6_${mth}.root
    SCRIPT=${mergeJobDir}/pMSSM_DGemtR50_TB6_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_pMSSM_DGemtR50_TB6_${mth}.root \
	${histPath}/histo_Herwigpp_UEEE3_CTEQ6L1_DGemtR50_TB6_M1M2MU_*.*_${mth}.root
EOF
    chmod 755 $SCRIPT

    echo "Merge DGemtR50_TB6 "
    rm -f ${histPath}/histo_pMSSM_DGstauR_TB50_${mth}.root
    SCRIPT=${mergeJobDir}/pMSSM_DGstauR_TB50_${mth}_job.sh
    [ -f ${SCRIPT} ] && rm -f ${SCRIPT}
    
    cat > $SCRIPT << "EOF"
#!/bin/bash
    hadd -f ${histPath}/histo_pMSSM_DGstauR_TB50_${mth}.root \
	${histPath}/histo_Herwigpp_DGstauR_TB50_M1M2MU_*.*_${mth}.root
EOF
    chmod 755 $SCRIPT
    

fi

echo 
echo ">>> Now do: " 
echo "export histPath=$histPath"
echo "export mth=$mth"
echo 
