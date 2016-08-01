#include "SiWRun.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "SiWTrackHit.hh"
#include <iostream>
#include <cmath>

#ifdef ROOT_OUTPUT
#include "DetectorConstruction.hh"
#include "SiWEcalPrimaryGeneratorAction.hh"
#endif

SiWRun::SiWRun()
{
  G4SDManager* SDMan = G4SDManager::GetSDMpointer();
 
  G4cout << "SiWRun Print G4SDManager Tree " << G4endl;
  SDMan->ListTree();
    
#ifdef ROOT_OUTPUT
  file=new TFile("testOutPut.root","RECREATE");
  tree=new TTree("tree","test");
  tree->Branch("numberOfEvent",&numberOfEvent);
  tree->Branch("sumEdepInActive",&activeEdep);
  tree->Branch("nSimHit",&nSimHit);
  tree->Branch("x","std::vector<double>",&x);
  tree->Branch("y","std::vector<double>",&y);
  tree->Branch("z","std::vector<double>",&z);
  tree->Branch("edep","std::vector<double>",&edep);
  tree->Branch("time","std::vector<double>",&time);
  tree->Branch("length","std::vector<double>",&length);
  tree->Branch("primaryGunPosistion",&primaryGunPosition,"primaryGunPosition[3]/D");
  tree->Branch("primaryMomentum",&primaryMomentum,"primaryMomentum[3]/D");
  tree->Branch("deltaLayer","std::vector<double>",&deltaLayer);
  tree->Branch("deltaX","std::vector<double>",&deltaX);
  tree->Branch("deltaY","std::vector<double>",&deltaY);
  tree->Branch("pdg","std::vector<int>",&pdg);
  tree->Branch("cosTheta",&cosTheta);
#endif


#ifdef LCIO_OUTPUT
  lcWriter=new LCIOWriter("toto");
#endif

}

SiWRun::~SiWRun() 
{
#ifdef ROOT_OUTPUT
  file->Write();
  file->Close();
#endif
}

void SiWRun::RecordEvent(const G4Event* evt)
{
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
#ifdef print_debug
  std::cout << "HCE->GetNumberOfCollections() = " << HCE->GetNumberOfCollections() << std::endl;
  std::cout << "HCE->GetHC(0)->GetName() = " << HCE->GetHC(0)->GetName() << std::endl;
  std::cout << "HCE->GetHC(1)->GetName() = " << HCE->GetHC(1)->GetName() << std::endl;
#endif
  if(!HCE) return;
  numberOfEvent++;

  std::map< std::string, std::vector<SiWTrackHit*> > SiWTrackHitMap;

  activeEdep = 0.0 ;
  nSimHit = 0;
  cosTheta = evt->GetPrimaryVertex()->GetPrimary()->GetMomentum().cosTheta();

  for(G4int icoll=0; icoll<HCE->GetNumberOfCollections(); icoll++){
    SiWTrackHitsCollection *Hitcoll=(SiWTrackHitsCollection*) HCE->GetHC(icoll);
    std::vector<SiWTrackHit*> *HitVector=Hitcoll->GetVector();
    if( SiWTrackHitMap.find( Hitcoll->GetName() ) != SiWTrackHitMap.end() ){
      G4cout << "Problem in map in SiWRun::RecordEvent(const G4Event* evt) --> abort" << G4endl;
      std::abort();
    }
    std::vector<SiWTrackHit*> vec;
    for (unsigned int i=0; i<Hitcoll->GetSize(); i++){
      if( (*HitVector)[i]->energyDeposited() > std::numeric_limits<double>::epsilon() ){
	vec.push_back( (*HitVector)[i] );
	nSimHit++;
	x.push_back( ((*HitVector)[i]->entrancePoint() + (*HitVector)[i]->deltaPosition()/2).x() );
	y.push_back( ((*HitVector)[i]->entrancePoint() + (*HitVector)[i]->deltaPosition()/2).y() );
	z.push_back( ((*HitVector)[i]->entrancePoint() + (*HitVector)[i]->deltaPosition()/2).z() );
	edep.push_back( (*HitVector)[i]->energyDeposited() );
	time.push_back( (*HitVector)[i]->time() );
	activeEdep+=(*HitVector)[i]->energyDeposited();
	length.push_back( ((*HitVector)[i]->entrancePoint() - (*HitVector)[i]->exitPoint()).mag() );
	pdg.push_back( (*HitVector)[i]->pdgID() );
	if(evt->GetPrimaryVertex()!=0){
	  primaryGunPosition[0]=evt->GetPrimaryVertex()->GetX0();
	  primaryGunPosition[1]=evt->GetPrimaryVertex()->GetY0();
	  primaryGunPosition[2]=evt->GetPrimaryVertex()->GetZ0();
	  primaryMomentum[0]=evt->GetPrimaryVertex()->GetPrimary()->GetMomentum().x();
	  primaryMomentum[1]=evt->GetPrimaryVertex()->GetPrimary()->GetMomentum().y();
	  primaryMomentum[2]=evt->GetPrimaryVertex()->GetPrimary()->GetMomentum().z();
	}
#ifdef ROOT_OUTPUT
	double delta=-z.back();
	if(icoll==0)
	  delta += -DetectorConstruction::Instance()->getEcalGeomParams().calorThickness/2 +
	    DetectorConstruction::Instance()->getEcalGeomParams().Silicon0RelativePositionZ +
	    DetectorConstruction::Instance()->getEcalGeomParams().moduleThickness * ( (*HitVector)[i]->replicaCopyNumber()+0.5 );
	else
	  delta += -DetectorConstruction::Instance()->getEcalGeomParams().calorThickness/2 +
	    DetectorConstruction::Instance()->getEcalGeomParams().Silicon1RelativePositionZ +
	    DetectorConstruction::Instance()->getEcalGeomParams().moduleThickness * ( (*HitVector)[i]->replicaCopyNumber()+0.5 );
	deltaLayer.push_back(delta) ;

	int I = (int)(DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY/2+x.back())/DetectorConstruction::Instance()->getEcalGeomParams().pixelSize ;
	int J = (int)(DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY/2+y.back())/DetectorConstruction::Instance()->getEcalGeomParams().pixelSize ;
	delta = -x.back() +
	  (I+0.5)*DetectorConstruction::Instance()->getEcalGeomParams().pixelSize -
	  DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY/2 ;
	deltaX.push_back(delta) ;
	
	delta = -y.back() + 
	  (J+0.5)*DetectorConstruction::Instance()->getEcalGeomParams().pixelSize -
	  DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY/2 ;
	deltaY.push_back(delta) ;

#endif
      }
    }
    std::pair< std::string, std::vector<SiWTrackHit*> > pair(Hitcoll->GetName(),vec);
    SiWTrackHitMap.insert(pair);
  }

#ifdef print_debug
  std::cout << "DetectorConstruction::Instance()->getEcalGeomParams().calorThickness/2 = " <<  DetectorConstruction::Instance()->getEcalGeomParams().calorThickness/2 << "\n"
	    << "DetectorConstruction::Instance()->getEcalGeomParams().Silicon0RelativePositionZ = " << DetectorConstruction::Instance()->getEcalGeomParams().Silicon0RelativePositionZ << "\n"
	    << "DetectorConstruction::Instance()->getEcalGeomParams().moduleThickness = " << DetectorConstruction::Instance()->getEcalGeomParams().moduleThickness
	    << std::endl;
#endif
  
#ifdef ROOT_OUTPUT
  tree->Fill();
#endif

#ifdef LCIO_OUTPUT

  std::cout << "evt->GetNumberOfPrimaryVertext() = " << evt->GetNumberOfPrimaryVertex() << std::endl;
  
  IMPL::MCParticleImpl *motherpart=new IMPL::MCParticleImpl();
  if (evt->GetPrimaryVertex() != 0){
    motherpart->setPDG(evt->GetPrimaryVertex()->GetPrimary()->GetPDGcode());
    double momentum[3],vertex[3];
    for (int i=0; i<3; i++){
      momentum[i]=evt->GetPrimaryVertex()->GetPrimary()->GetMomentum()[i];
      vertex[i]=evt->GetPrimaryVertex()->GetPosition()[i];
    }
    motherpart->setMomentum( momentum );
    motherpart->setVertex( vertex );
  }
  else{
    motherpart->setPDG(99);
    double momentum[3]={0,0,0};
    double vertex[3]={0,0,0};
    motherpart->setMomentum( momentum );
    motherpart->setVertex( vertex );
  }
  std::string detectorName("SiWEcal");
  lcWriter->createLCEvent(detectorName);
  lcWriter->createOutputCollection(std::string("simcalorimeterhit") , std::string("SiWEcalSimHit"));
  for(std::map< std::string,std::vector<SiWTrackHit*> >::iterator it=SiWTrackHitMap.begin(); it!=SiWTrackHitMap.end(); ++it)
    lcWriter->fillSimCaloHitCollection(std::string("SiWEcalSimHit"), it->first, it->second, motherpart);
  lcWriter->GenerateUniformNoise(motherpart,std::string("SiWEcalSimHit"));
  lcWriter->writeEvent(motherpart);

  delete motherpart;
  SiWTrackHitMap.clear();
#endif

  x.clear();
  y.clear();
  z.clear();
  edep.clear();
  time.clear();
  length.clear();
  deltaLayer.clear();
  deltaX.clear();
  deltaY.clear();
}
