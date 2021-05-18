#!/bin/bash
## CONFIG

##======= Define setup variables
BASEREL="21.2.60"

##======= Setup release
# test if setupATLAS is defined, if not define it
alias setupATLAS 2> /dev/null > /dev/null || (export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase && alias setupATLAS='source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh')
setupATLAS -q
lsetup git -q

asetup AnalysisBase,${BASEREL},here
lsetup "sft releases/LCG_93/pyyaml/3.11/"


##======= Checkout ELCore
if [ ! -d ELCore ]
then
    git clone ssh://git@gitlab.cern.ch:7999/ELCore/ELCore.git
    cd ELCore
    git checkout origin/MemoryOpt
    source fetch_externals.sh
    cd ${TestArea}
fi

##======= Compile it
BuildDir=${TestArea}/../build
if [ ! -d ${BuildDir} ]
then
    mkdir -p ${BuildDir}
    cd ${BuildDir}
    cmake ${TestArea}
    make
    cd ${TestArea}
fi

buildPlatformDirPattern="${BuildDir}/x86_64*"
buildPlatformDirs=($buildPlatformDirPattern)
source ${buildPlatformDirs[0]}/setup.sh
