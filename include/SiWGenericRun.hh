#ifndef SiWGenericRun_h 
#define SiWRun_h 1

#include "globals.hh"
#include "G4Run.hh"

#include "LCIOWriter.hh"
#include "IMPL/MCParticleImpl.h"

class SiWGenericRun : public G4Run
{
public:
  
 
  SiWGenericRun();
  virtual ~SiWGenericRun();

  virtual void RecordEvent(const G4Event*);


private:

private:
  LCIOWriter* lcWriter;

};

#endif
