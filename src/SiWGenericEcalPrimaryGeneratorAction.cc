#include "SiWGenericEcalPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "DetectorConstruction.hh"

SiWGenericEcalPrimaryGeneratorAction::SiWGenericEcalPrimaryGeneratorAction()
{
}

SiWGenericEcalPrimaryGeneratorAction::~SiWGenericEcalPrimaryGeneratorAction()
{
}

void SiWGenericEcalPrimaryGeneratorAction::readConfig(std::vector<std::string> &particleVec,
				      std::vector<float> &energyVec,
				      std::vector<float> &etaVec,
				      std::vector<float> &phiVec)
{
  std::string particle;
  float energy;
  float eta;
  float phi;
  std::fstream in;
  in.open("config.txt");

  while(1){
    if( !in.good() ) break;
    in >> particle >> energy >> eta >> phi;
    particleVec.push_back(particle);
    energyVec.push_back(energy);
    etaVec.push_back(eta);
    phiVec.push_back(phi);
  }
}

void SiWGenericEcalPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //generate config for this event
  std::string filename = "config.py";
  std::string command = "python ";
  command += filename;
  system(command.c_str());

  //read config
  std::vector<std::string> particle;
  std::vector<float> energy;
  std::vector<float> eta;
  std::vector<float> phi;
  SiWGenericEcalPrimaryGeneratorAction::readConfig(particle,energy,eta,phi);

  if(particle.empty() ||
     particle.size() != energy.size() ||
     particle.size() != eta.size() ||
     particle.size() != phi.size() ){
    G4cout << "FATAL ERROR in SiWGenericEcalPrimaryGeneratorAction::SiWGenericEcalPrimaryGeneratorAction() \n \t"
	   << "check muPjet.py and config.txt \n \t"
	   << "-->> std::abort()" << G4endl;
    std::abort();
  }

  G4ThreeVector vertexPosition=G4ThreeVector( 0.0*CLHEP::mm ,
					      0.0*CLHEP::mm ,
					      DetectorConstruction::Instance()->getEcalGeomParams().primaryVertexPosition );
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  for( unsigned int i=0; i<particle.size(); i++ ){
    G4ParticleGun* particleGun=NULL;
    particleGun = new G4ParticleGun();
    particleGun->SetParticleDefinition( particleTable->FindParticle(particle[i]) );
    particleGun->SetParticleEnergy( energy[i]*CLHEP::GeV );
    particleGun->SetParticlePosition( vertexPosition );
    G4double theta = 2 * std::atan( std::exp( -eta[i] ) ) ;
    G4ThreeVector momentum;

    momentum.setRThetaPhi( 1.0,
			   theta,
			   phi[i] );

    particleGun->SetParticleMomentumDirection( momentum/momentum.mag() );
    particleGun->GeneratePrimaryVertex(anEvent);
    PrintInfo(particleGun);
    delete particleGun;
  }
}

void SiWGenericEcalPrimaryGeneratorAction::PrintInfo(G4ParticleGun* gun)
{
  std::cout << "Generate one primary" << std::endl;
  std::cout << "Particle : \t" 
	    << "Particle pdg = " << gun->GetParticleDefinition()->GetParticleName() << "\t"
	    << "energy = " << gun->GetParticleEnergy() << "\n \t"
	    << "momentum direction= " << gun->GetParticleMomentumDirection() << std::endl;
}
