# Declare the name of the package:
atlas_subdir( TruthLevelAnalysis )

find_package( ROOT COMPONENTS Core Hist Tree RIO )

# Declare the package's dependencies:
atlas_depends_on_subdirs(
   PUBLIC
   ELCore
   LPXKfactorTool
   #TauSpinnerTool
   Control/xAODRootAccess
   Control/CxxUtils
   DataQuality/GoodRunsLists
   Event/xAOD/xAODJet
   Event/xAOD/xAODMuon
   Event/xAOD/xAODEgamma
   Event/xAOD/xAODTau
   Event/xAOD/xAODTracking
   Event/xAOD/xAODMissingET
   Event/xAOD/xAODCore
   Event/xAOD/xAODEventInfo
   
   PhysicsAnalysis/MuonID/MuonSelectorTools
   PhysicsAnalysis/MuonID/MuonIDAnalysis/MuonEfficiencyCorrections
   PhysicsAnalysis/MuonID/MuonIDAnalysis/MuonMomentumCorrections
   PhysicsAnalysis/ElectronPhotonID/ElectronPhotonSelectorTools
   PhysicsAnalysis/ElectronPhotonID/ElectronPhotonFourMomentumCorrection
   PhysicsAnalysis/TauID/TauAnalysisTools
   PhysicsAnalysis/AnalysisCommon/PileupReweighting
   PhysicsAnalysis/AnalysisCommon/PATInterfaces/PATInterfaces
   PhysicsAnalysis/AnalysisCommon/IsolationSelection
   PhysicsAnalysis/AnalysisCommon/PMGTools
   InnerDetector/InDetRecTools/TrackVertexAssociationTool

   Trigger/TrigAnalysis/TrigDecisionTool
   Trigger/TrigConfiguration/TrigConfxAOD
   
   Reconstruction/MET/METUtilities
   
   Event/xAOD/xAODCutFlow
   Tools/PathResolver
  )

# Build a dictionary for the library:
atlas_add_root_dictionary( TruthLevelAnalysisLib TruthLevelAnalysisLibCintDict
   ROOT_HEADERS
   TruthLevelAnalysis/CombinedSelection.h
   TruthLevelAnalysis/CommonTruthSelectionModule.h
   TruthLevelAnalysis/TruthOutputModule.h
   Root/LinkDef.h
   EXTERNAL_PACKAGES ROOT
)

# Build a shared library:
atlas_add_library( TruthLevelAnalysisLib TruthLevelAnalysis/*.h Root/*.cxx Root/*.h ${TruthLevelAnalysisLibCintDict}
   PUBLIC_HEADERS TruthLevelAnalysis
   PRIVATE_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} Root/*.h
   LINK_LIBRARIES ELCoreLib xAODRootAccess xAODTau xAODCore GoodRunsListsLib xAODEventInfo TauAnalysisToolsLib xAODMuon xAODEgamma MuonSelectorToolsLib xAODJet xAODTracking PileupReweightingLib ElectronPhotonSelectorToolsLib xAODMissingET MuonMomentumCorrectionsLib METUtilitiesLib ElectronPhotonFourMomentumCorrectionLib MuonEfficiencyCorrectionsLib xAODCutFlow CxxUtils PATInterfaces IsolationSelectionLib  PMGToolsLib TrackVertexAssociationToolLib PathResolver TrigDecisionToolLib TrigConfxAODLib)

atlas_install_data( data/* )
