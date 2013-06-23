

pathRun=${WORKAREA}/SusyWeakProdAna/run

#Bkg=BkgYield_Andreas
#Sig=SignalYield_Andreas_wA_noslep_hadW
#Filename=Optimisation_Andreas_wA_noslep_hadW

#Bkg=BkgYield_AnyesSRZjetsOpt4
#Sig=SignalYield_AnyesSRZjetsOpt4_wA_noslep_hadW
#Filename=Optimisation_AnyesSRZjetsOpt4_wA_noslep_hadW

Bkg=BkgYield_AnyesSRSS_Opt1
Sig=SignalYield_AnyesSRSS_Opt1_wA_slep
Filename=Optimisation_AnyesSRSS_Opt1_wA_slep


cp ${pathRun}/$Bkg.tex .
cp ${pathRun}/$Sig.tex .

rm -f include.tex
cat >> include.tex <<EOF
\include{$Bkg}
\include{$Sig}
EOF

cat header_p.tex include.tex footer.tex > ${Filename}_table.tex
pdflatex --interaction batchmode ${Filename}_table.tex
mv ${Filename}_table.pdf ${pathRun}
rm -f *.aux
rm -f *.log
rm -f *.out

rm -f $Bkg.tex
rm -f $Sig.tex
rm -f ${Filename}_table.tex
