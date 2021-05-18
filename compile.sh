BuildDir=${TestArea}/../build

cd ${BuildDir}
cmake ${TestArea}
make
cd ${TestArea}

buildPlatformDirPattern="${BuildDir}/x86_64*"
buildPlatformDirs=($buildPlatformDirPattern)
source ${buildPlatformDirs[0]}/setup.sh
