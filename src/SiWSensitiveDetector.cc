#include "SiWSensitiveDetector.hh"
#include "G4VProcess.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include <string>
SiWSensitiveDetector::SiWSensitiveDetector(G4String name,int replicaCountLevel)
  : G4VSensitiveDetector(name),
    collectionID(-1),
    _replicaCountLevel(replicaCountLevel)
{
  std::string colName=name;
  colName+=std::string("TrackHits");
  collectionName.insert(colName);
}

void SiWSensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
  if (collectionID<0) collectionID=GetCollectionID(0);
  hitsCollection = new SiWTrackHitsCollection(SensitiveDetectorName,collectionName[0]);
  HCE->AddHitsCollection(collectionID,hitsCollection);
}


G4bool SiWSensitiveDetector::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  SiWTrackHit* aHit=new SiWTrackHit(aStep,_replicaCountLevel);
  hitsCollection->insert(aHit);
  return true;
}
