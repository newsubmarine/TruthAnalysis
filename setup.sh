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

###======= Checkout DDAnalysisModules
#if [ ! -d DDAnalysisModules ]
#then
#    git clone ssh://git@gitlab.cern.ch:7999/atlasHBSM/atlas-phys-higgs-mssm-htautau-btag/DDAnalysisModules.git
#fi
#
###======= Checkout LPXKfactorTool
#if [ ! -d LPXKfactorTool ]
#then
#    LPXKfactorToolRev=280743
#    svn checkout svn+ssh://svn.cern.ch/reps/atlasphys-exo/Physics/Exotic/LPX/CommonTools/LPXKfactorTool/trunk@${LPXKfactorToolRev} LPXKfactorTool
#fi

# Not for now, further validation needed
###======= Checkout TauSpinnerTool
#if [ ! -d TauSpinnerTool ]
#then
#    git clone ssh://git@gitlab.cern.ch:7999/atlas-perf-tau/TauSpinnerTool.git
#    cd TauSpinnerTool
#    git checkout origin/dev
#    cd ${TestArea}
#fi

###======= Checkout Asg_Tauola
#if [ ! -d Asg_Tauola ]
#then
#    git clone ssh://git@gitlab.cern.ch:7999/atlas-perf-tau/Asg_Tauola.git
#    cd Asg_Tauola
#    git checkout origin/dev
#    cd ${TestArea}
#fi

###======= Checkout PlottingTools
#if [ ! -d PlottingTools ]
#then
#    git clone ssh://git@gitlab.cern.ch:7999/atlasHBSM/atlas-phys-higgs-mssm-htautau-btag/PlottingTools.git
#    cd PlottingTools
#    echo "Compile PlottingTools"
#    make
#    cd ..
#    echo "Compilation done"
#fi

#export PLOTTINGTOOLS_PATH=${TestArea}/PlottingTools/
#export PYTHONPATH=${PYTHONPATH}:${PLOTTINGTOOLS_PATH}
#export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PLOTTINGTOOLS_PATH}/obj
#export ROOT_INCLUDE_PATH=${ROOT_INCLUDE_PATH}:${PLOTTINGTOOLS_PATH}/include
#export PYTHONPATH=${PYTHONPATH}:${TestArea}/TruthLevelAnalysis/plotting/

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

#source ${BuildDir}/${AnalysisBase_PLATFORM}/setup.sh
source ${BuildDir}/x*/setup.sh
