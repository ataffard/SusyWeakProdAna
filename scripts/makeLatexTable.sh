# ./makeLatexTable.sh  

date="091812_1fb"
#date="082312_1fb_SRnoMet_remerge"
#date="081312_5fb_SRnoMet_unbiasIP"

pathRun=${WORKAREA}/SusyWeakProdAna/run
path=${WORKAREA}/histoAna/SusyAna/histos_${date}
dir=${path}/Tables

cp ${WORKAREA}/SusyWeakProdAna/scripts/*.tex ${dir}

declare -a file=(BkgEst_DG2L BkgEst_ML)

${pathRun}/DrawPlotsExec -table

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
    rm -f ${ifile}_table.tex
    
done