#include "SiWEcalPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "TRandom.h"
#include "TF2.h"
#include "TMath.h"
#include <time.h>  
#include <iostream>
#include <fstream>

#include "DetectorConstruction.hh"

SiWEcalPrimaryGeneratorAction::SiWEcalPrimaryGeneratorAction()
{
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  particleGun->SetParticleDefinition(particleTable->FindParticle(particleName="mu+"));
  particleGun->SetParticleEnergy(1.0*CLHEP::GeV);
  
  gunOptionPosition = std::string("default");// could be with primary_gun_action_messenger class
  gunOptionMomentum = std::string("normal"); // could be with primary_gun_action_messenger class

  randMaxPos = DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY/2 ;
  
  gaussianMean = 10e-5*CLHEP::mm;
  gaussianSigma = 10e-5*CLHEP::mm;
  uniformParameter = 0.5;  

  _xPosSigma = 50.0*CLHEP::mm;
  _yPosSigma = 50.0*CLHEP::mm;

  _gunPosition_x_function=new TF1("func","gaus",-500,500);
  _gunPosition_y_function=new TF1("func","gaus",-500,500);

  _thePrimaryGeneratorMessenger = new SiWEcalPrimaryGeneratorActionMessenger(this);

}

SiWEcalPrimaryGeneratorAction::~SiWEcalPrimaryGeneratorAction()
{
  PrintInfo();
  delete particleGun;
  delete _thePrimaryGeneratorMessenger;
  delete _gunPosition_x_function;
  delete _gunPosition_y_function;
}

void SiWEcalPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  std::cout << "gaussianMean = " << gaussianMean << "\t gaussianSigma = " << gaussianSigma << std::endl;
  G4ThreeVector v(0.0,0.0,1.0); // par default could be modified with primary_gun_action_messenger class
  _primaryPos=G4ThreeVector( 0.0*CLHEP::mm ,
			     0.0*CLHEP::mm ,
			     - DetectorConstruction::Instance()->getEcalGeomParams().worldSizeZ/6 );
  if(gunOptionPosition==std::string("random")){
    if( randMaxPos > DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY/2 ){
      G4cout << "WARNING /GunParameter/randomMaxPosition out of range -> should be lower than calorSizeXY/2 : " << DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY/2 << " mm -->> exit simu in SiWEcalPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)" <<G4endl;
      std::abort();
    }
    _primaryPos.setX( 2*randMaxPos*G4UniformRand() - randMaxPos ) ;
    _primaryPos.setY( 2*randMaxPos*G4UniformRand() - randMaxPos ) ;
  }
  if(gunOptionPosition==std::string("cosmic")){
    G4double x0=0.0;//DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY*G4UniformRand()-DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY/2;
    G4double z0=0.0;//DetectorConstruction::Instance()->getEcalGeomParams().calorThickness*G4UniformRand()-DetectorConstruction::Instance()->getEcalGeomParams().calorThickness/2;
    G4double y0=1.2*DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY/2;
    _primaryPos=G4ThreeVector(x0,y0,z0);
    G4cout << _primaryPos << G4endl;
    //    getchar();
  }
  if(gunOptionPosition==std::string("beamtest")){
    _gunPosition_x_function->SetParameters(1.0,0.0,_xPosSigma);
    _gunPosition_y_function->SetParameters(1.0,0.0,_yPosSigma);
    _primaryPos.setX(_gunPosition_x_function->GetRandom());
    _primaryPos.setY(_gunPosition_y_function->GetRandom());
  }
  particleGun->SetParticlePosition(_primaryPos);
  
  if( gunOptionPosition==std::string("cosmic") ){
    if( gunOptionMomentum!=std::string("cosmic_gaus") && gunOptionMomentum!=std::string("cosmic_uniform") ){
      G4cout << " ERROR : wrong option : GunOptionPosition=cosmic should be used together with cosmic_gaus or cosmic_uniform as GunOptionMomentum" << G4endl;
      G4cout << " I PREFER KILL THE RUN" << G4endl;
      std::abort();
    }
  }
  
  if( gunOptionMomentum==std::string("cosmic_gaus") || gunOptionMomentum==std::string("cosmic_uniform") ){
    if( gunOptionPosition!=std::string("cosmic") ){
      G4cout << " ERROR : wrong option : GunOptionMomentum=cosmic_gaus or cosmic_uniform should be used together with cosmic GunOptionPosition" << G4endl;
      G4cout << " I PREFER KILL THE RUN" << G4endl;
      std::abort();
    }
  }

  if(gunOptionMomentum==std::string("gaus")){
    TF1 *func=new TF1("func","gaus",-1,1);
    func->SetParameters(1,gaussianMean,gaussianSigma);
    G4double px=func->GetRandom();
    G4double py=func->GetRandom();
    G4double pz=1.0;
    v=G4ThreeVector(px,py,pz);
    v/=v.mag();
    delete func;
  }
  else if(gunOptionMomentum==std::string("uniform")){
    G4double px;
    G4double py;
    if(G4UniformRand()>uniformParameter)px=G4UniformRand();
    else px=-G4UniformRand();
    if(G4UniformRand()>uniformParameter) py=G4UniformRand();
    else py=-G4UniformRand();
    G4double pz=1.0;
    v=G4ThreeVector(px,py,pz);
    v/=v.mag();
  }
  else if(gunOptionMomentum==std::string("cosmic_gaus")){
    TF1 *func=new TF1("func","gaus",-1,1);
    func->SetParameters(1,gaussianMean,gaussianSigma);
    G4double px=func->GetRandom();
    G4double py=-1.0;
    G4double pz=func->GetRandom();
    v=G4ThreeVector(px,py,pz);
    v/=v.mag();
    delete func;
  }
  else if(gunOptionMomentum==std::string("cosmic_uniform")){
    G4double px;
    G4double py=-1.0;
    G4double pz;
    if(G4UniformRand()>uniformParameter)px=G4UniformRand();
    else px=-G4UniformRand();
    if(G4UniformRand()>uniformParameter) pz=G4UniformRand();
    else pz=-G4UniformRand();
    v=G4ThreeVector(px,py,pz);
    v/=v.mag();
  }

  particleGun->SetParticleMomentumDirection(v);
  particleGun->GeneratePrimaryVertex(anEvent);
  _primaryMom=v;
  //G4cout << "Gun position = " << pos << G4endl;
  //G4cout << "Gun momentum = " << v << G4endl;
}

void SiWEcalPrimaryGeneratorAction::PrintInfo()
{
  G4cout << "USED OPTION : " << G4endl;
  G4cout << "GUN POSITION OPTION = " << gunOptionPosition << G4endl;
  if(gunOptionPosition==std::string("random"))
    G4cout << "randMaxPos = " << randMaxPos << G4endl;
  G4cout << "GUN MOMENTUM OPTION = " << gunOptionMomentum << G4endl;
  if(gunOptionMomentum==std::string("gaus"))
    G4cout << "GAUSSIAN GUN MEAN = " << gaussianMean << "\t" 
	   << "GAUSSIAN GUN SIGMA = " << gaussianSigma << G4endl;
  else if(gunOptionMomentum==std::string("uniform"))
    G4cout << "UNIFORM GUN PARAMETER = " << uniformParameter<< G4endl;
}
