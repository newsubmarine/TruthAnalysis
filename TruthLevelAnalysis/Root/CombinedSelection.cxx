#include <EventLoop/Job.h>
#include <TruthLevelAnalysis/CombinedSelection.h>

ClassImp(CombinedSelection)

CombinedSelection :: CombinedSelection ()
{

}

//_____________________________________________________________________________

void CombinedSelection :: core_setupJob (EL::Job& job)
{
  job.useXAOD ();
  xAOD::Init( "CombinedSelection" ).ignore();
}

//_____________________________________________________________________________

void CombinedSelection :: core_initialize ()
{

  m_event = wk()->xaodEvent();

}

//_____________________________________________________________________________


void CombinedSelection :: core_execute ()
{
  auto weight = applyCuts("PreSelection");

  applyCuts("ZeroLepton", weight);
  applyCuts("OneLepton", weight);
  applyCuts("TwoLepton", weight);
  applyCuts("ThreeLepton", weight);
  applyCuts("FourLepton", weight);

}


