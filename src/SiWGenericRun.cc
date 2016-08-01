#include "SiWGenericRun.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "SiWTrackHit.hh"
#include <iostream>
#include <cmath>

SiWGenericRun::SiWGenericRun()
{
  G4SDManager* SDMan = G4SDManager::GetSDMpointer();
 
  G4cout << "SiWGenericRun Print G4SDManager Tree " << G4endl;
  SDMan->ListTree();
  lcWriter=new LCIOWriter("toto");
}

SiWGenericRun::~SiWGenericRun() 
{
}

void SiWGenericRun::RecordEvent(const G4Event* evt)
{
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  if(!HCE) return;
  numberOfEvent++;

  std::map< std::string, std::vector<SiWTrackHit*> > SiWTrackHitMap;

  for(G4int icoll=0; icoll<HCE->GetNumberOfCollections(); icoll++){
    SiWTrackHitsCollection *Hitcoll=(SiWTrackHitsCollection*) HCE->GetHC(icoll);
    std::vector<SiWTrackHit*> *HitVector=Hitcoll->GetVector();
    if( SiWTrackHitMap.find( Hitcoll->GetName() ) != SiWTrackHitMap.end() ){
      G4cout << "Problem in map in SiWGenericRun::RecordEvent(const G4Event* evt) --> abort" << G4endl;
      std::abort();
    }
    std::vector<SiWTrackHit*> vec;
    for (unsigned int i=0; i<Hitcoll->GetSize(); i++){
      if( (*HitVector)[i]->energyDeposited() > std::numeric_limits<double>::epsilon() ){
	vec.push_back( (*HitVector)[i] );
      }
    }
    std::pair< std::string, std::vector<SiWTrackHit*> > pair(Hitcoll->GetName(),vec);
    SiWTrackHitMap.insert(pair);
  }

  std::vector< IMPL::MCParticleImpl* >parts;
  IMPL::MCParticleImpl* part=NULL;
  for(int pid=0;pid<evt->GetNumberOfPrimaryVertex(); pid++){
    part=new IMPL::MCParticleImpl();
    part->setPDG( evt->GetPrimaryVertex(pid)->GetPrimary()->GetPDGcode() );
    double momentum[3];
    double vertex[3];
    for (int i=0; i<3; i++){
      momentum[i]=evt->GetPrimaryVertex(pid)->GetPrimary()->GetMomentum()[i];
      vertex[i]=evt->GetPrimaryVertex(pid)->GetPosition()[i];
    }
    part->setMomentum( momentum );
    part->setVertex( vertex );
    parts.push_back(part);
  }
  std::string detectorName("SiWEcal");
  lcWriter->createLCEvent(detectorName);
  lcWriter->createOutputCollection(std::string("simcalorimeterhit") , std::string("SiWEcalSimHit"));
  for(std::map< std::string,std::vector<SiWTrackHit*> >::iterator it=SiWTrackHitMap.begin(); it!=SiWTrackHitMap.end(); ++it)
    lcWriter->fillSimCaloHitCollection(std::string("SiWEcalSimHit"), it->first, it->second, parts.at(0));
  lcWriter->writeGenericEvent(parts);

  for(int pid=0;pid<evt->GetNumberOfPrimaryVertex(); pid++)
    delete parts.at(pid);
  SiWTrackHitMap.clear();
}
