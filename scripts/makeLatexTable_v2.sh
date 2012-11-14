# ./makeLatexTable.sh  

date="110812_13fb_n0111_DD_MMtrial9_SYS_HFT"

pathRun=${WORKAREA}/SusyWeakProdAna/run
pathScripts=${WORKAREA}/SusyWeakProdAna/scripts
path=${WORKAREA}/histoAna/SusyAna/histos_${date}
dir=${path}/Tables

#Generate the latex table for each SR/CR

#SR=(CR2LepSS40)
SR=(CR2LepOS  CR2LepSS CR2LepOS40 CR2LepSS40 \
    CRZ VR1SS NTOP NWW1\
    ZXCR1 ZXCR3 ZXCR4 ZXCR5 ZXCR6 ZXCR7 \
    preSRjveto preSRSSjveto preSR2jets preSRmT2 \
    SRjveto SRSSjveto SR2jets SRmT2 SRmT2b )


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
