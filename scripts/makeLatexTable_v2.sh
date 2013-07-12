# ./makeLatexTable.sh  

date="070913_21fb_n0144_DD_v1"

pathRun=${WORKAREA}/SusyWeakProdAna/run
pathScripts=${WORKAREA}/SusyWeakProdAna/scripts
path=${HISTOANA}/SusyAna/histos_${date}
dir=${path}/Tables

#Generate the latex table for each SR/CR
SR=( \
    SRmT2a SRmT2b SRmT2c \
    SRWWa SRWWb SRWWc \
    SRZjets SRSSjets \
    CRWWmet CRWWmt2 \
    CRTOPmet CRTOPmt2 CRTOPZjets \
    CRZVmet CRZVmt2a CRZVmt2b CRZVmt2c CRZVmt2d \
    CRZVZjets \
    VRSS \
    CRZ CRZjets CRZjveto \
    CR2LepOS CR2LepSS CR2LepSS40 \
    preSROSjveto preSRmT2 preSRZjets preSRWW preSRSS \
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
