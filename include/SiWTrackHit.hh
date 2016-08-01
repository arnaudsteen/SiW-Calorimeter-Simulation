#ifndef SiWTrackHit_h
#define SiWTrackHit_h

#include "G4VHit.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"

class G4Step;

class SiWTrackHit : public G4VHit
{
public:
  SiWTrackHit(const G4Step* aStep,int replicaCountLevel);
  virtual ~SiWTrackHit() {;}
  // virtual void Draw();
  // virtual void Print();

private:
  G4double _energyDeposited;
  G4ThreeVector _entrancePoint;
  G4ThreeVector _exitPoint;
  G4int _replicaCopyNumber;
  G4ThreeVector _deltaPosition;
  G4bool _enteringStep;
  G4bool _leavingStep;
  G4int _pdgID;
  G4int _charge;
  G4double _time;
  G4double _trueLength;
  G4int _trackid;
  G4String processName;
  G4int _parentid;
  G4ThreeVector _momentum;
  G4String _trackVertexVolumeName;
  G4int _trackStatus;
  G4Track* _track;

public:
  G4double energyDeposited() const {return _energyDeposited;} 
  const G4ThreeVector& entrancePoint() const {return _entrancePoint;}
  const G4ThreeVector& exitPoint() const {return _exitPoint;}
  const G4ThreeVector& deltaPosition() const {return _deltaPosition;}
  G4int replicaCopyNumber() const {return _replicaCopyNumber;}
  G4bool isEntering() const {return _enteringStep;}
  G4bool isLeaving() const {return _leavingStep;}
  G4int pdgID() const {return _pdgID; }
  G4int charge() const {return _charge; }
  G4double time() const {return _time; }
  G4int trackid() const{ return _trackid;}
  G4int parentid() const{ return _parentid;}
  G4double trueLength() const{ return _trueLength;}
  G4int trackStatus() const{return _trackStatus;}
  const G4ThreeVector& momentum() const {return _momentum;}
  G4String trackVertexVolumeName() const{return _trackVertexVolumeName;}
  const G4Track* track() const{ return _track;}
};


#include "G4THitsCollection.hh"
typedef G4THitsCollection<SiWTrackHit> SiWTrackHitsCollection;

#endif
