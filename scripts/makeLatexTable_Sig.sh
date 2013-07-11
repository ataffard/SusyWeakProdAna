

pathRun=${WORKAREA}/SusyWeakProdAna/run

#Bkg=BkgYield_Andreas
#Sig=SignalYield_Andreas_wA_noslep_hadW
#Filename=Optimisation_Andreas_wA_noslep_hadW

#Bkg=BkgYield_AnyesSRZjetsOpt4
#Sig=SignalYield_AnyesSRZjetsOpt4_wA_noslep_hadW
#Filename=Optimisation_AnyesSRZjetsOpt4_wA_noslep_hadW

#Bkg=BkgYield_AnyesSRSS_Opt1
#Sig=SignalYield_AnyesSRSS_Opt1_wA_slep
#Filename=Optimisation_AnyesSRSS_Opt1_wA_slep

### WH ###
Bkg=BkgYield_AnyesSRSS_WH_MM_d
Sig=SignalYield_AnyesSRSS_WH_MM_d_wA_noslep_WH_2Lep
Filename=Optimisation_AnyesSRSS_WH_MM_d_wA_noslep_WH_2Lep


#BkgYield_AnyesSRSS_WH_EE_a.tex
#BkgYield_AnyesSRSS_WH_EE_b.tex
#BkgYield_AnyesSRSS_WH_EM_a.tex
#BkgYield_AnyesSRSS_WH_EM_b.tex
#BkgYield_AnyesSRSS_WH_MM_a.tex
#BkgYield_AnyesSRSS_WH_MM_b.tex
#BkgYield_AnyesSRSS_WH_MM_c.tex
#BkgYield_AnyesSRSS_WH_MM_d.tex

#SignalYield_AnyesSRSS_WH_base_wA_noslep_WH_2Lep.tex
#SignalYield_AnyesSRSS_WH_EE_a_wA_noslep_WH_2Lep.tex
#SignalYield_AnyesSRSS_WH_EE_b_wA_noslep_WH_2Lep.tex
#SignalYield_AnyesSRSS_WH_EM_a_wA_noslep_WH_2Lep.tex
#SignalYield_AnyesSRSS_WH_EM_b_wA_noslep_WH_2Lep.tex
#SignalYield_AnyesSRSS_WH_MM_a_wA_noslep_WH_2Lep.tex
#SignalYield_AnyesSRSS_WH_MM_b_wA_noslep_WH_2Lep.tex
#SignalYield_AnyesSRSS_WH_MM_c_wA_noslep_WH_2Lep.tex
#SignalYield_AnyesSRSS_WH_MM_d_wA_noslep_WH_2Lep.tex


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
