#ifndef SiWRun_h 
#define SiWRun_h 1

#include "globals.hh"
#include "G4Run.hh"

#include "TROOT.h"
#include <TFile.h>
#include <TTree.h>
#include "LCIOWriter.hh"
#include "IMPL/MCParticleImpl.h"

#define ROOT_OUTPUT
#define LCIO_OUTPUT

class SiWRun : public G4Run
{
public:
  
 
  SiWRun();
  virtual ~SiWRun();

  virtual void RecordEvent(const G4Event*);


private:

#ifdef ROOT_OUTPUT
  TFile* file;
  TTree* tree;
#endif
private:
  G4double activeEdep;
  G4int nSimHit;
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> z;
  std::vector<double> edep;
  std::vector<double> time;
  std::vector<double> length;
  std::vector<double> deltaLayer;
  std::vector<double> deltaX;
  std::vector<double> deltaY;
  std::vector<int> pdg;
  G4double primaryGunPosition[3];
  G4double primaryMomentum[3];
  G4double cosTheta;
  
#ifdef LCIO_OUTPUT
  LCIOWriter* lcWriter;
#endif
};

#endif
