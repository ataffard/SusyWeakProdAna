#!/bin/bash

path=${WORKAREA}/histoAna/SusyAna/histos_110212_13fb_n0111_DD_MMtrial9_SYS_HFT

hadd -f ${path}/histo_Zee.147770_rlep.root ${path}/histo_Zee.147770_rlep_*
hadd -f ${path}/histo_Zmumu.147771_rlep.root ${path}/histo_Zmumu.147771_rlep_*
hadd -f ${path}/histo_TtbarLeptLept.117800_rlep.root ${path}/histo_TtbarLeptLept.117800_rlep_*
hadd -f ${path}/histo_llnunu_WW.126892_rlep.root ${path}/histo_llnunu_WW.126892_rlep_*



