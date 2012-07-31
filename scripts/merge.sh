#!/bin/bash

date="073112_1fb"
logPath=${WORKAREA}/SusyWeakProdAna/run/jobLogs
#histPath=${WORKAREA}/histoAna/SusyAna
histPath=${WORKAREA}/histoAna/SusyAna/histos_${date}
outPath=${WORKAREA}/histoAna/SusyAna/histos_${date}


#cp ${histPath}/histo_Egamma.periodA.root ${histPath}/histo_Egamma.periodA_merge.root
#cp ${histPath}/histo_Muons.periodA.root ${histPath}/histo_Muons.periodA_merge.root
#period=( B)
#for pp in ${period[@]}; do
#    hadd -f ${histPath}/histo_Egamma.period${pp}_merge.root ${histPath}/histo_Egamma.period${pp}?.root #${histPath}/histo_Egamma.period${pp}??.root
#    hadd -f ${histPath}/histo_Muons.period${pp}_merge.root ${histPath}/histo_Muons.period${pp}?.root #${histPath}/histo_Muons.period${pp}??.root
#done

#hadd -f ${histPath}/histo_data12.root ${histPath}/histo_Egamma.periodA_merge.root ${histPath}/histo_Muons.periodA_merge.root \
#    ${histPath}/histo_Egamma.periodB_merge.root ${histPath}/histo_Muons.periodB_merge.root

#
# mc12a
#


#rm -f ${histPath}/histo_top.root
#rm -f ${histPath}/histo_diBoson_Sherpa.root
#rm -f ${histPath}/histo_diBoson_Herwig.root
#rm -f ${histPath}/histo_diBoson_MCATNLO.root
#rm -f ${histPath}/histo_diBoson_Powheg.root
#rm -f ${histPath}/histo_Zjets_Alpgen.root
#rm -f ${histPath}/histo_Zjets_Sherpa.root
#rm -f ${histPath}/histo_Wjets.root
#rm -f ${histPath}/histo_SimplifiedModel_wA_slep.root

echo "Merge Top "
#hadd -f ${histPath}/histo_top.root   \
#    ${histPath}/histo_ttbar.105200.root \
#    ${histPath}/histo_SingleTopSChan*.*.root  \
#    ${histPath}/histo_singletop_tchan*.*.root  \
#    ${histPath}/histo_SingleTopWtChanIncl.108346.root  \
#    ${histPath}/histo_ttbarW.119353.root \
#    ${histPath}/histo_ttbarWj.119354.root \
#    ${histPath}/histo_ttbarZ.119355.root

#Sherpa Diboson
echo "Merge diBoson Sherpa "
#hadd -f ${histPath}/histo_diBoson_Sherpa.root \
#    ${histPath}/histo_llll_ZZ.126894.root \
#    ${histPath}/histo_llnunu_ZZ.126895.root \
#    ${histPath}/histo_lllnu_WZ.126893.root \
#    ${histPath}/histo_llnunu_WW.126892.root \
#    ${histPath}/histo_*Pt10.*.root  

#Herwig Diboson +Wg AlpgenJimmy + Zg Sherpa
echo "Merge diBoson Herwig "
hadd -f ${histPath}/histo_diBoson_Herwig.root \
    ${histPath}/histo_WW.105985.root  \
    ${histPath}/histo_ZZ.105986.root  \
    ${histPath}/histo_WpWm*.106*.root  \
    ${histPath}/histo_WZ.105987.root  \
    ${histPath}/histo_WgammaNp?.*.root  \
    ${histPath}/histo_eegammaPt10.145161.root \
    ${histPath}/histo_mumugammaPt10.145162.root \
    ${histPath}/histo_tautaugammaPt10.126854.root 

#MC@NLO WW + Herwig for ZZ, WZ and Wg AlpgenJimmy + Zg Sherpa
echo "Merge diBoson MC@NLO "
hadd -f ${histPath}/histo_diBoson_MCATNLO.root \
    ${histPath}/histo_WpWm*.105*.root \
    ${histPath}/histo_ZZ.105986.root  \
    ${histPath}/histo_WZ.105987.root  \
    ${histPath}/histo_WgammaNp?.*.root \
    ${histPath}/histo_eegammaPt10.145161.root \
    ${histPath}/histo_mumugammaPt10.145162.root \
    ${histPath}/histo_tautaugammaPt10.126854.root 


#Diboson PowHegPythia and Wg AlpgenJimmy + Zg Sherpa
echo "Merge diBoson Powheg "
hadd -f ${histPath}/histo_diBoson_Powheg.root \
    ${histPath}/histo_WpWm*.126*.root \
    ${histPath}/histo_WZ.1294*.root \
    ${histPath}/histo_ZZ*.1269*.root \
    ${histPath}/histo_WgammaNp?.*.root \
    ${histPath}/histo_eegammaPt10.145161.root \
    ${histPath}/histo_mumugammaPt10.145162.root \
    ${histPath}/histo_tautaugammaPt10.126854.root 



#Z+jets Alpgen
#echo "Merge Z+jets Alpgen "
#hadd -f ${histPath}/histo_Zjets_Alpgen.root  \
#    ${histPath}/histo_ZeeNp?.*.root  \
#    ${histPath}/histo_ZeeNp?Excl_Mll10to60.*.root  \
#    ${histPath}/histo_ZeebbNp?.*.root \
#    ${histPath}/histo_ZmumuNp?.*.root  \
#    ${histPath}/histo_ZmumuNp?Excl_Mll10to60.*.root  \
#    ${histPath}/histo_ZmumubbNp?.*.root  \
#    ${histPath}/histo_ZtautauNp?.*.root  \
#    ${histPath}/histo_ZtautauNp?Excl_Mll10to60.*.root  \
#    ${histPath}/histo_ZtautaubbNp?.*.root  
#    ${histPath}/histo_ZmumuccNp?.*.root  \
#    ${histPath}/histo_ZeeccNp?.*.root \
#    ${histPath}/histo_ZtautauccNp?.*.root  


#Z+jets Sherpa
#echo "Merge Z+jets Sherpa "
#hadd -f ${histPath}/histo_Zjets_Sherpa.root  \
#    ${histPath}/histo_Zee.147770.root \
#    ${histPath}/histo_Zmumu.147771.root \
#    ${histPath}/histo_Ztautau.147772.root 
    
#W+jets Alpgen
#echo "Merge W+jets Alpgen "
#hadd -f ${histPath}/histo_Wjets.root   \
#    ${histPath}/histo_WenuNp?.*.root  \
#    ${histPath}/histo_WmunuNp?.*.root  \
#    ${histPath}/histo_WtaunuNp?.*.root \
#    ${histPath}/histo_WbbNp?.*.root  \
#    ${histPath}/histo_WccNp?.*.root  \
#    ${histPath}/histo_WcNp?.*.root  

#Simplified Model ModeA w/Slepton
#echo "Merge ModeA w/Slepton "
#hadd -f ${histPath}/histo_SimplifiedModel_wA_slep.root \
#    ${histPath}/histo_Herwigpp_simplifiedModel_wA_slep_noWcascade_*.*.root


#move all the histos to new area:
#if [ ! -d "${outPath}" ]; then
#    mkdir ${outPath}
#    mkdir ${outPath}/logs
#fi
#mv ${histPath}/*.root ${outPath}
#mv ${logPath}/*.log ${outPath}/logs

