=====================================
The H/A/Z' to tauhad tauhad Framework
=====================================

.. contents:: Table of contents

Introduction
============

This is the analysis framework of the BSM H/A/Z' to tau tau search in fully hadronic final state.
With the framework provided, nTuples can be produced from appropriate derivations (HIGG4D4).
Currently two branches developed independently for different releases:

 * master for Rel 21 with CMake
 * r20_samples for Rel 20 with RootCore/CMT

How to get started (Rel 21)
===========================

Simply checkout master branch and Source the setup script::

  mkdir MyNewHtoTauTauAnalysis
  cd MyNewHtoTauTauAnalysis
  git clone ssh://git@gitlab.cern.ch:7999/atlasHBSM/atlas-phys-higgs-mssm-htautau-btag/HtautauAnalysis.git
  cd HtautauAnalysis
  source ./setup.sh

This sets up the environment, fetches all needed external packages and create a build folder where the framework is compiled.
In order to recompile, simply switch to the build folder and call make and set the new environment if needed::
  cd ../build/
  make
  source ./$AnalysisBase_PLATFORM/setup.sh

Running the code
================

Switch to the run folder and call the run.py, e.g.::

  cd $TestArea/HtautauAnalysis/run/
  python run.py <sample name reg ex> --sampleDir <path/to/samples> --regionFile <MyRegionDefinition.yaml> --modConf <MyModuleConfigurations.yaml>

where the regionFile argument describes the different regions (SR or CR, btag and bveto ect) by cuts and routines defined in the modules and modConf the configuration of the modules.
Other arguments like --grid allows to send jobs to grid. Also GNU parallel and SoGE batch (NAF Desy) driver are supported.

Running on the grid (lxplus)
============================

First, make sure that you have your grid certificate and panda set up::

    voms-proxy-init -voms atlas
    lsetup panda

When using the grid, instead of providing a folder where the samples reside, you provide a text file containing a list of all of the samples you require. Currently there are two lists inluded in the repository, gridFilesData_AODfix.txt and gridFilesMC_AODfix.txt. These will be updated once release 21 data is available. 

You can also add a sample name regex as before if required, or leave it blank to run all samples in the file. 

In addition, make sure to add an output folder, as some of the output is required for retrieving the data. You also need to specify a name for the production on the grid. 

The full command is::

  cd $TestArea/HtautauAnalysis/run/ 
  python run.py <sample name reg ex> --grid --gridInput <grid file list> --regionFile <MyRegionDefinition.yaml> --modConf <MyModuleConfigurations.yaml> --outputFolder <path/to/folder> --gridStreamName <name> &> log.txt 

An example command to run only the DYtautau MC would be::

  cd $TestArea/HtautauAnalysis/run/ 
  python run.py DYtautau --grid --gridInput gridFilesMC_AODfix.txt --regionFile regions_higgs.yaml --modConf module_config.yaml --outputFolder /afs/cern.ch/work/a/adbailey/finished/ --gridStreamName run1 &> log.txt

Once it is completed there is a useful script that will download the output with the correct filenames to use with the plotting part of the framework::

  HtautauAnalysis/run/grabdata.py
    
To use the script you need to have set up the framework as detailed above, and have rucio set up::

  lsetup rucio
    
Before using the script you need to download the details of the production from panda. On lxplus, do the following::

  cern-get-sso-cookie -u https://bigpanda.cern.ch/ -o bigpanda.cookie.txt
  curl -b bigpanda.cookie.txt -H 'Accept: application/json' -H 'Content-Type: application/json' "panda web address" > jobs.json

To get the panda web address, go to your user page, eg.:

  https://bigpanda.cern.ch/user/ADAM%20JOHN%20BAILEY@csic.es/

Once there, you can select the link for the tag for your production. Then you can use that web address in the command above to get only the samples from the current production, for example::

  curl -b bigpanda.cookie.txt -H 'Accept: application/json' -H 'Content-Type: application/json' "https://bigpanda.cern.ch/tasks/?username=ADAM%20JOHN%20BAILEY@csic.es&tag=run1/&datasets=true&limit=1000&days=14" > jobs.json
    
Here, I also added "&limit=1000" and "days=14" to ensure that all of the jobs from the production are included. 

This produces a json file that contains a list of the jobs and their status, required by the grabdata script. 

The command to run the script is the following:: 
    
  python grabdata.py -m </path/to/output__20XXXXXX_XX-XX-XX/output-hist/> -d <path/to/output__20XXXXXX_XX-XX-XX/output-hist/> -j <path/to/jobs.json> -o </path/to/production/> -t <tag>

The -m and -d options are the folders containing lists of datasets that were run: they are produced when submitting the jobs and will be in the folder specified by the "--outputFolder" argument. "-j" specifies the json file, and "-o" the location where the output should be saved to. Make sure that the location has enough free space to store the production. The "-t" option is for the grid tag, which should be what you specified with the "--gridStreamName" option before. 

Note, the combination of the dataset identifier and tag should be unique - if running a new production use a new tag. 

This take a while to run, but you can interupt the script if you want to using ctrl+c. If you do this, go to the output folder and delete and subfolders in the "data" and "mc" directories. You can restart the script with the same command, it will not re-download datasets that already exist. 

Once the script finishes, it produces an output text file which summarises the status of the samples called "Download_Summary.txt". If you have samples with a status other than "Done", you should resubmit those jobs. You can check the status of the jobs using the panda interface (eg. click the link under status, broken).

Most of the time, problems with running only occur on particular sites. When resubmitting you should exclude any problematic sites by editing the "run.py" script. Look for the line::

  driver.options().setString(ROOT.EL.Job.optGridExcludedSite, "") 

Uncomment it, and add the names of the excluded sites to the last argument, separated by commas::

  driver.options().setString(ROOT.EL.Job.optGridExcludedSite, "ANALY_SITE_1,ANALY_SITE_2,ANALY_SITE_3")
  
You can then copy the list of files you want to rerun from "Download_Summary.txt" into a new text file, and use that in the --gridInput option when re-submitting the jobs. 
