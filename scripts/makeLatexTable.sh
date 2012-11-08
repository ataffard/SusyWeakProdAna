# ./makeLatexTable.sh  

#date="102712_13fb_n0105_DD_SYS_HFT"
#date="103012_13fb_n0111_STD"
#date="103012_13fb_n0111_DD"

#date="103012_13fb_n0111_DD_MMtrial8_wQflipSFnew"

#date="103112_13fb_n0111_DD_MMtrial7"
#date="103112_13fb_n0111_DD_MMtrial8_SYS_HFT"
date="110812_13fb_n0111_DD_MMtrial9_SYS_HFT"

pathRun=${WORKAREA}/SusyWeakProdAna/run
path=${WORKAREA}/histoAna/SusyAna/histos_${date}
dir=${path}/Tables

#OPT="-ZX histo_ZX_SherpaLFHF -Top histo_top_PowHeg"
OPT="-mode DD"
#OPT="-mode STD "
#-ZX histo_ZX_SherpaLFHF -Fake histo_mcfake_Sherpa"
#OPT="-ZJet histo_Zjets_SherpaLFHF_wGam -Fake histo_mcFake_Sherpa_wGam"
#OPT="-ZJet histo_Zjets_Alpgen -Fake histo_mcFake_Alpgen -Top histo_top_MCNLO"

${pathRun}/DrawPlotsExec -table ${OPT}

cp ${WORKAREA}/SusyWeakProdAna/scripts/*.tex ${dir}
#declare -a file=(BkgEst_DG2L BkgEst_ML)
declare -a file=(BkgEst_DG2L)

cd ${dir}
for ifile in ${file[@]}; do
    rm -f include.tex
cat >> include.tex <<EOF
\include{$ifile}
EOF
    cat header.tex include.tex footer.tex > ${ifile}_table.tex
    
    pdflatex --interaction batchmode ${ifile}_table.tex
#    pdflatex  ${ifile}_table.tex
    
    rm -f *.aux
    rm -f *.log
    rm -f *.out
   # rm -f ${ifile}_table.tex
done
