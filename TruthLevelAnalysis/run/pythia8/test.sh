for file in `ls output*/data-fetch/*root`
do                                     
array=(${file//_/ })
name=${array[1]}
echo /afs/ihep.ac.cn/users/x/xinst/work/Validation/TruthLevelAnalysis/TruthLevelAnalysis/plotting/5000Events/pythia8/${name}/
#mkdir -p /afs/ihep.ac.cn/users/x/xinst/work/Validation/TruthLevelAnalysis/TruthLevelAnalysis/plotting/5000Events/pythia8/${name}/
#cp $file /afs/ihep.ac.cn/users/x/xinst/work/Validation/TruthLevelAnalysis/TruthLevelAnalysis/plotting/5000Events/pythia8/${name}/
done
