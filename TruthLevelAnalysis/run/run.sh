#python run.py 2l_01 --regionFile regions_container.yaml --modConf module_config.yaml --sampleDir inputs  --maxEvents 1000000 2>&1  | tee l2.log
#python run.py 3l_01 --regionFile regions_container.yaml --modConf module_config.yaml --sampleDir inputs  --maxEvents 1000000 2>&1  | tee l3.log
#python run.py 4l_01 --regionFile regions_container.yaml --modConf module_config.yaml --sampleDir inputs  --maxEvents 1000000 2>&1  | tee l4.log
#python run.py 2l_10 --regionFile regions_container.yaml --modConf module_config.yaml --sampleDir inputs  --maxEvents 1000000 2>&1  | tee l2.log
#python run.py 3l_10 --regionFile regions_container.yaml --modConf module_config.yaml --sampleDir inputs  --maxEvents 1000000 2>&1  | tee l3.log
#python run.py 4l_10 --regionFile regions_container.yaml --modConf module_config.yaml --sampleDir inputs  --maxEvents 1000000 2>&1  | tee l4.log
python run.py bbZZ4l_01 --regionFile regions_bbZZ4l.yaml --modConf module_config.yaml --sampleDir inputs  --maxEvents 1000000 2>&1  | tee l4.log
python run.py bbZZ4l_10 --regionFile regions_bbZZ4l.yaml --modConf module_config.yaml --sampleDir inputs  --maxEvents 1000000 2>&1  | tee l4.log
