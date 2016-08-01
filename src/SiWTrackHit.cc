#include "SiWTrackHit.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4VProcess.hh"

SiWTrackHit::SiWTrackHit(const G4Step* aStep,int replicaCountLevel)
{
  _energyDeposited=aStep->GetTotalEnergyDeposit();
  _entrancePoint=aStep->GetPreStepPoint()->GetPosition();
  _exitPoint=aStep->GetPostStepPoint()->GetPosition();
  _deltaPosition=aStep->GetDeltaPosition();
  G4TouchableHistory * theTouchable = 
    (G4TouchableHistory *) aStep->GetPreStepPoint()->GetTouchable();
  _replicaCopyNumber=theTouchable->GetReplicaNumber(replicaCountLevel);
  _enteringStep=(aStep->GetPreStepPoint()->GetStepStatus()==fGeomBoundary);
  _leavingStep=(aStep->GetPostStepPoint()->GetStepStatus()==fGeomBoundary);
  _pdgID=aStep->GetTrack()->GetDynamicParticle()->GetPDGcode();
  _time=aStep->GetPostStepPoint()->GetGlobalTime();
  _momentum=aStep->GetTrack()->GetMomentum();
  _trackid=aStep->GetTrack()->GetTrackID();
  _parentid=aStep->GetTrack()->GetParentID();
  _trackStatus=aStep->GetTrack()->GetTrackStatus();
  _trueLength=(_exitPoint-_entrancePoint).mag();
  _charge=(int) aStep->GetTrack()->GetDynamicParticle()->GetCharge();
  _trackVertexVolumeName=aStep->GetTrack()->GetLogicalVolumeAtVertex()->GetName();
  _track=aStep->GetTrack();
}
