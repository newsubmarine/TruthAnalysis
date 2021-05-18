#########################################################################
# File Name: copy.sh
# Description: 
# Author: xzh
# mail: huangxz@ihep.ac.cn
# Created Time: Wed Nov 11 08:26:32 2020
#########################################################################
#!/bin/bash

#SAMPLES="bbZZ4l bbZZ2l2q"
SAMPLES="2l0tau 2l1tau 3l0tau 3l1tau 4l0tau 4l1tau yy0l yy1l yy2l bbZZ4l bbZZ2l2q"

LAMDAS="cHHHp01d0 cHHHp10d0"

INPUT_DIR="../../../../herwig ../../../../pythia"

for input in ${INPUT_DIR}
do
    for sample in ${SAMPLES}
    do
        for lambda in ${LAMDAS}
        do
            mkdir -p ${input##*/}/${sample}_${lambda}
            echo  ./${input##*/}/${sample}_${lambda}/DAOD_TRUTH1.${sample}_cHHHp01d0.pool.root
            cp ${input}/${sample}_${lambda}/DAOD_TRUTH1.DAOD.TRUTH1.pool.root ./${input##*/}/${sample}_${lambda}/DAOD_TRUTH1.${sample}_cHHHp01d0.pool.root
        done
    done
done
