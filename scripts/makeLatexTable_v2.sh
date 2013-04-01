# ./makeLatexTable.sh  

#date="030213_21fb_n0127_Moriond_DD_v6"
#date="031513_21fb_n0135_Moriond_DD_v3"

#date="032913_21fb_n0135_Moriond_DD_v5"
date="033113_21fb_n0135_Moriond_DD_v6"

pathRun=${WORKAREA}/SusyWeakProdAna/run
pathScripts=${WORKAREA}/SusyWeakProdAna/scripts
path=${HISTOANA}/SusyAna/histos_${date}
dir=${path}/Tables

#Generate the latex table for each SR/CR
SR=(CRTOP CRWW CRWW2 CRWW3 CRWW4 CRWW5 CRWW6 ZXCRWW ZXCRWW2 ZXCRpremT2 ZXCRmT2a ZXCRmT2b preSRmT2 SRmT2a SRmT2b)

<<SKIP
SR=(SROSjveto SRmT2a SRmT2b \
    SR2jets SRZjets SRSSjets \
    SRWWa SRWWb SRWWc \
    ZXCRjveto ZXCR2jets ZXCRmT2a ZXCRmT2b ZXCRWW \
    CRTOP CRTOPWWa CRTOPWWb CRTOPWWc \
    CRWW CRWW2 CRWW4 CRWW5 CRWWa CRWWb CRWWc\
    VRSS VRSSbtag\
    CRZ  CR2LepOS CR2LepSS CR2LepSS40\
    preSROSjveto preSRmT2\
    preSR2jets preSRZjets preSRSS  
)
SKIP

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
