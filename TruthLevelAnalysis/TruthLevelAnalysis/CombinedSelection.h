#pragma once

#ifndef COMBINEDSELECTION_H
#define COMBINEDSELECTION_H

#include <ELCore/CycleBase.h>

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"

#include "xAODTruth/TruthParticleContainer.h"

class CombinedSelection : public CycleBase
{

public:
  CombinedSelection ();

  virtual void core_setupJob (EL::Job& job);
  //virtual void core_fileExecute ();
  //virtual void core_histInitialize ();
  //virtual void core_changeInput (bool firstFile);
  virtual void core_initialize ();
  virtual void core_execute ();
  //virtual void core_postExecute ();
  //virtual void core_finalize ();
  //virtual void core_histFinalize ();

  ClassDef(CombinedSelection, 1);
};


#endif
