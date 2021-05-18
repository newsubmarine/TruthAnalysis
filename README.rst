=====================================
The H/A/Z' to tauhad tauhad Framework
=====================================

.. contents:: Table of contents

Introduction
============

This is the analysis framework of the BSM H/A/Z' to tau tau search.

How to get started (Rel 21)
===========================

Simply checkout master branch and Source the setup script::

  source ./setup.sh
  source ./compile.sh

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
