#!/usr/bin/env python
import os,sys,subprocess

## Common packages required by SusyWeakProdAna

lpkgs = [
'svn co svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/D3PDTools/RootCore/tags/RootCore-00-01-77 RootCore',
'svn co svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/CalibrationDataInterface/tags/CalibrationDataInterface-00-02-01/cmt svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/CalibrationDataInterface/tags/CalibrationDataInterface-00-02-01/CalibrationDataInterface svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/CalibrationDataInterface/tags/CalibrationDataInterface-00-02-01/Root svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/CalibrationDataInterface/tags/CalibrationDataInterface-00-02-01/src CalibrationDataInterface',
'svn co svn+ssh://svn.cern.ch/reps/atlasphys/Physics/SUSY/Analyses/WeakProduction/ChargeFlip/trunk ChargeFlip',
'svn co svn+ssh://svn.cern.ch/reps/atlasphys/Physics/SUSY/Analyses/WeakProduction/DGTriggerReweight/tags/DGTriggerReweight-00-00-29 DGTriggerReweight',
'svn co svn+ssh://svn.cern.ch/reps/atlasphys/Physics/SUSY/Analyses/WeakProduction/HistFitterTree/tags/HistFitterTree-00-00-17 HistFitterTree',
'svn co svn+ssh://svn.cern.ch/reps/atlasgrp/Institutes/UCIrvine/btoggers/Mt2/tags/Mt2-00-00-01 Mt2',
'svn co svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/AnalysisCommon/ReweightUtils/tags/ReweightUtils-00-02-13 ReweightUtils',
'svn co svn+ssh://svn.cern.ch/reps/atlasphys/Physics/SUSY/Analyses/WeakProduction/SignificanceCalculator/trunk SignificanceCalculator',
'svn co svn+ssh://svn.cern.ch/reps/atlasphys/Physics/SUSY/Analyses/WeakProduction/SusyMatrixMethod/tags/SusyMatrixMethod-00-02-04 SusyMatrixMethod',
'svn co svn+ssh://svn.cern.ch/reps/atlasinst/Institutes/UCIrvine/SUSYAnalysis/SusyNtuple/trunk SusyNtuple',
#'svn co svn+ssh://svn.cern.ch/reps/atlasinst/Institutes/UCIrvine/SUSYAnalysis/SusyNtuple/branches/SusyNtuple-00-00-60-branch SusyNtuple',
'svn co svn+ssh://svn.cern.ch/reps/atlasinst/Institutes/UCIrvine/mrelich/SusyXSReader/trunk SusyXSReader',
'svn co svn+ssh://svn.cern.ch/reps/atlasgrp/Institutes/UCIrvine/btoggers/ZllR/tags/ZllR-00-00-01 ZllR',


]

## Extra packages
# - Fake lepton background
#lpkgs += ['']

def parseCmdLine(args):
    """ Parse input command line to optdict.
    To get the whole list of options type : install.py -h"""
    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option("--vclean", dest="vclean", help="Remove pkg before checking-out new version",
                      action='store_true', default=False)
    (config, args) = parser.parse_args(args)
    return config

def doc():
    return """
Short instructions
==================

If you have not done so:
cd RootCore
./configure
source scripts/setup.sh
cd ..

Then every time you start a new session you should:
source RootCore/scripts/setup.sh

To compile:
$ROOTCOREDIR/scripts/find_packages.sh
$ROOTCOREDIR/scripts/compile.sh

Further informations on RootCore can be found on:
https://twiki.cern.ch/twiki/bin/view/AtlasProtected/RootCore
"""

def removeObsolete(mylpkgs):
    for pkg in mylpkgs:
        if len(pkg.split()) < 4: continue
        pkgname = pkg.split()[len(pkg.split())-1]
        if not os.path.isdir(pkgname): continue
        pkgtag  = pkg.split()[2]
        if not 'tags/' in pkgtag: continue
        pkgtag = pkgtag.split('tags/')[1].split('/')[0]
        if os.path.isfile('%s/.svn/entries' % pkgname):
            fname = '%s/.svn/entries' % pkgname
        elif os.path.isfile('%s/cmt/.svn/entries' % pkgname):
            fname = '%s/cmt/.svn/entries' % pkgname
        else:
            continue
        fentries = open(fname,'r')
        found = False
        for line in fentries:
            if not pkgtag in line: continue
            found = True
            break
        fentries.close()
        if found: continue
        print 'removing',pkgname,'to prepare new tag',pkgtag
        ret = subprocess.call('rm -Rf %s' % pkgname, shell=True)
    return

## Check-out
def checkout(mylpkgs):
    ret = 0
    for pkg in mylpkgs:
        print 'checking-out',pkg.split()[len(pkg.split())-1]
        ret = subprocess.call(pkg, shell=True)
        if ret == 0: continue
        print pkg,'returned with code',ret
        break
    return

def main():
    config = parseCmdLine(sys.argv[1:])
    if config.vclean: removeObsolete(lpkgs)
    checkout(lpkgs)
    ## Print short instructions
    print doc()
    return

if __name__ == "__main__":
    main()
