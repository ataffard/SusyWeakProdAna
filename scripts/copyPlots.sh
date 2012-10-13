#!/bin/bash

#FOR CR's PLOTS
#histDir=histos_101212_13fb_n0105_DD_blindData_noMetRel

#For  SR's blind data plots
histDir=histos_101212_13fb_n0105_DD_unblindData_noMetRel

pathIn=${WORKAREA}/histoAna/SusyAna/${histDir}/Plots
pathOut=~/Dropbox/DG2L/4Note/Section8Plots/

DIL=(EE MM EM)

#SR=(CR2LepOS CR2LepSS)
#PLOTS=(mll metrel nJets nCJets nFJets nBJets mt2 pTll)

SR=(SRjveto SRSSjveto SR2jets SRmT2 )
PLOTS=(mll metrel)

for sr in ${SR[@]}; do
    for dil in ${DIL[@]}; do
	for plots in ${PLOTS[@]}; do
	    name=pred_DG2L_${sr}_${dil}_DG2L_${plots}_logy.png
	    cp ${pathIn}/${name} ${pathOut}
	done
    done
done





ls -1 ${pathOut}