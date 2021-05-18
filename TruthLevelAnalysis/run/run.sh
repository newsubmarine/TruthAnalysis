samples="2l0tau 2l1tau 3l0tau 3l1tau 4l0tau 4l1tau yy0l yy1l yy2l"
#samples="bbZZ4l bbZZ2l2q"
generators="pythia"
#generators="herwig"

for sample in $samples
do
    for generator in $generators
    do
    python run.py ${sample} --regionFile regions_container.yaml --modConf module_config.yaml --sampleDir dihiggs-inputs/${generator} --maxEvents -1 2>&1 | tee log/${sample}_${generator}.log
    done
done
#python run.py vbf_2l0tau --regionFile regions_container.yaml --modConf module_config.yaml --sampleDir inputs --maxEvents -1 2>&1 | tee vbf_2l0tau.log
#python run.py vbf_2l1tau --regionFile regions_container.yaml --modConf module_config.yaml --sampleDir inputs --maxEvents -1 2>&1 | tee vbf_2l1tau.log
#python run.py vbf_3l0tau --regionFile regions_container.yaml --modConf module_config.yaml --sampleDir inputs --maxEvents -1 2>&1 | tee vbf_3l0tau.log
#python run.py vbf_3l1tau --regionFile regions_container.yaml --modConf module_config.yaml --sampleDir inputs --maxEvents -1 2>&1 | tee vbf_3l1tau.log
#python run.py vbf_4l0tau --regionFile regions_container.yaml --modConf module_config.yaml --sampleDir inputs --maxEvents -1 2>&1 | tee vbf_4l0tau.log
#python run.py vbf_4l1tau --regionFile regions_container.yaml --modConf module_config.yaml --sampleDir inputs --maxEvents -1 2>&1 | tee vbf_4l1tau.log
