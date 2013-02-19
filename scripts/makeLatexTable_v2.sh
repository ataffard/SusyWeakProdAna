# ./makeLatexTable.sh  

date="021713_21fb_n0127_Moriond_DD_v4"

pathRun=${WORKAREA}/SusyWeakProdAna/run
pathScripts=${WORKAREA}/SusyWeakProdAna/scripts
path=${WORKAREA}/histoAna/SusyAna/histos_${date}
dir=${path}/Tables

#Generate the latex table for each SR/CR
SR=(SROSjveto SRmT2a SRmT2b \
    SR2jets SRZjets SRSSjets \
    SRWWa SRWWb SRWWc \
    ZXCRjveto ZXCR2jets ZXCRmT2a ZXCRmT2b ZXCRWW \
    CRTOP CRTOPWWa CRTOPWWb CRTOPWWc \
    CRWW CRWWa CRWWb CRWWc\
    VRSS VRSSbtag\
    CRZ  CR2LepOS CR2LepSS CR2LepSS40\
    preSROSjveto preSRmT2\
    preSR2jets preSRZjets preSRSS  
)


cd ${pathRun}
for sr in ${SR[@]}; do
    root -q -l "../macros/bkgYield.C+(\"${sr}\")" 
done

cp ${WORKAREA}/SusyWeakProdAna/scripts/*.tex ${dir}
declare -a file=(DG2L_Bkg)

cd ${dir}
for ifile in ${file[@]}; do
    rm -f include.tex
    for sr in ${SR[@]}; do
cat >> include.tex <<EOF
\include{BkgYield_${sr}}
EOF
    done
    
    cat header.tex include.tex footer.tex > ${ifile}_table.tex
    pdflatex --interaction batchmode ${ifile}_table.tex
    rm -f *.aux
    rm -f *.log
    rm -f *.out

done
