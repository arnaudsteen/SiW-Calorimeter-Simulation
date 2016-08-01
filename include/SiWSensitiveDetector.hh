#ifndef SiWSensitiveDetector_h
#define SiWSensitiveDetector_h

#include "G4VSensitiveDetector.hh"
#include "SiWTrackHit.hh"
class G4Step;
class G4HCofThisEvent;

class SiWSensitiveDetector : public G4VSensitiveDetector
{
public:
  SiWSensitiveDetector(G4String name,int replicaCountLevel);
  virtual ~SiWSensitiveDetector() {;}
  virtual void Initialize(G4HCofThisEvent*);
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  virtual void EndOfEvent(G4HCofThisEvent*){;}
private:
  SiWTrackHitsCollection * hitsCollection;
  G4int collectionID;
  G4int _replicaCountLevel;
};

#endif
