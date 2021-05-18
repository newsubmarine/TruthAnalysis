#!/bin/bash

samples="l2 l3 l4 l5"

for sample in $samples
do

    if [ ! -d $sample ]; then
        mkdir $sample
    fi

    cd $sample
    if [ ! -d DAOD_Input ]; then
        mkdir DAOD_Input
    fi
    cd DAOD_Input
    
    DAODs=`ls -1d ~/publicfs/ATLAS/dihiggs/TruthAnalysis/Grid/DAOD/${sample}/user.xiaozhon.hh.${sample}.TRUTH1_EXT0/*`

    for DAOD in $DAODs
    do
        ln -nfs $DAOD .
    done
    cd ../..
done



