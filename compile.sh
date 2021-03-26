BuildDir=${TestArea}/../build

cd ${BuildDir}
cmake ${TestArea}
make
cd ${TestArea}

source ${BuildDir}/${AnalysisBase_PLATFORM}/setup.sh

