from ELCore.interface import *
from ROOT import *

def addModules(alg, conf_file):
    modules = [CommonTruthSelectionModule(),
               TruthOutputModule()]

    try:
        ZPModule
    except:
        print "Not initialising ZPModule."
    else:
        modules.append(ZPModule())
    
    #confModulesFromYAML(conf_file, *modules)
    
    [alg.addModule(mod) for mod in modules]
