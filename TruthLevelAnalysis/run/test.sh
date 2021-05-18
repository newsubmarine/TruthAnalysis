for file in `ls output*/data-fetch/*root`
do                                     
array=(${file//_/ })
name=${array[1]}
mkdir -p /afs/ihep.ac.cn/users/x/xinst/work/Validation/TruthLevelAnalysis/TruthLevelAnalysis/plotting/5000Events/herwig7/${name}/
cp $file /afs/ihep.ac.cn/users/x/xinst/work/Validation/TruthLevelAnalysis/TruthLevelAnalysis/plotting/5000Events/herwig7/${name}/
done
