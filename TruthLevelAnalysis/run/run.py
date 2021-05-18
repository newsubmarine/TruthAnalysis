import ROOT
ROOT.gROOT.SetBatch(True)
from ROOT import PyConfig
PyConfig.IgnoreCommandLineOptions = True
import os, argparse

import time
from ELCore.PrintCutflow import *
from ELCore.interface import *
import truth_modules

ROOT.gROOT.Macro("$ROOTCOREDIR/scripts/load_packages.C")

ROOT.xAOD.Init().ignore()

__location__ = os.path.realpath(
    os.path.join(os.getcwd(), os.path.dirname(__file__)))

def main(args):
    sh = ROOT.SH.SampleHandler()    


    if not args.grid:
        dl = ROOT.SH.DiskListLocal(args.sampleDir)
        ROOT.SH.scanDir(sh, dl, "*.root*", "*{}*".format(args.process))
    else:
        f = open(args.gridInput)
        for line in f:
            if args.process in line:
                ROOT.SH.addGrid(sh, line.split()[0])
    
    sh.setMetaString ("nc_tree", "CollectionTree")
    if not args.grid:
        sh.setMetaDouble (ROOT.EL.Job.optFilesPerWorker, args.filesPerWorker);
    
    job = ROOT.EL.Job() 
    job.sampleHandler (sh)

    if args.maxEvents:
        job.options().setDouble(ROOT.EL.Job.optMaxEvents, args.maxEvents)

    alg = ROOT.CombinedSelection()
    alg.setLoggingLevel("info")
    
    read_yaml(args.regionFile, alg)
    truth_modules.addModules(alg, args.modConf)
    
    job.algsAdd (alg)

    if args.parallel:
        driver = ROOT.EL.ParallelDriver()
        #os.system("env|awk -F '=' '{print \"export \"$1\"=\"$2}' | grep -v '\\n'> %s/setup_workernode.sh"%__location__)
        #driver.setupScript = __location__+"/setup_workernode.sh"
        driver.nCores = 3
        driver.max_load = 25
        driver.nice = 19
        driver.clusterConfigFile = "/raid1/users/hauswald/scripts/parallel/cluster.txt"
    elif args.proof:
        return "proof mode is currently under development"
        driver = ROOT.EL.ProofDriver()
        driver.numWorkers = args.proof
    elif args.gebatch:
        driver = ROOT.EL.GEDriver()
        driver.shellInit = "source "+__location__+"/setup_workernode.sh"
        job.options().setString(ROOT.EL.Job.optSubmitFlags, "-S /bin/bash -l h_cpu=3:55:00 -l h_vmem=3500M -l distro=sld6")
    elif args.grid:
        driver = ROOT.EL.PrunDriver()
        driver.options().setString("nc_outputSampleName", "user.lhauswal.%in:name[2]%.%in:name[6]%.feb27b")
        driver.options().setString(ROOT.EL.Job.optGridNFilesPerJob, str(args.filesPerWorker))
        driver.options().setDouble(ROOT.EL.Job.optGridMergeOutput, 1)
        driver.options().setString(ROOT.EL.Job.optGridExcludedSite, "ANALY_SLAC_SHORT_1HR")
    else:
        driver = ROOT.EL.DirectDriver()
    
    if not args.outputFolder.endswith("/"):
        args.outputFolder += "/"
    folder = args.outputFolder+"output_{}_{}".format(args.process, time.strftime("%Y%m%d_%H-%M-%S"))
    os.system("rm -f latest")
    os.system("ln -s %s latest"%folder)

    if not args.grid:
        driver.submit (job, folder)
        sample=sh.at(0)
        print "~"*80
        print bcolors.BOLD+bcolors.OKGREEN+"SAMPLE:", sample.name(), bcolors.ENDC
        printCutflow(folder+"/data-fetch/"+sample.name()+".root", only="Nominal")
    else:
        driver.submitOnly(job, folder)
        
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='BSM Htautau analysis')
    parser.add_argument('process',nargs='?',default="",type=str,help='run only on subset of processes')
    parser.add_argument('--parallel',default=False, action='store_true',help='use parallel mode')
    parser.add_argument('--proof',type=int,default=None,help='use proof mode (under development)')
    parser.add_argument('--gebatch', default=False, action='store_true', help='use GE batch driver')  
    parser.add_argument('--regionFile', type=str, default='regions.yaml', help='Region config yaml file')
    parser.add_argument('--sampleDir', type=str, default=None, help='path to sample dir to scan')
    parser.add_argument('--maxEvents', type=int, default=None, help='max. number of events, default ~ all')
    parser.add_argument('--filesPerWorker', type=int, default=3, help='number of files per worker')
    parser.add_argument('--modConf', type=str, default='module_config.yaml', help='module config yaml file')
    parser.add_argument('--outputFolder', type=str, default='./', help='path to output folder')
    parser.add_argument('--grid', default=False, action='store_true')
    parser.add_argument('--gridInput', type=str, default='gridInput.txt')
    args = parser.parse_args()
    main(args)
