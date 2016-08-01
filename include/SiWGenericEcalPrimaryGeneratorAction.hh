#ifndef SiWGenericEcalPrimaryGeneratorAction_h
#define SiWGenericEcalPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4String.hh"
#include "globals.hh"
#include "iostream"
#include "fstream"
#include "vector"
#include "stdlib.h"
#include "G4ThreeVector.hh" 

class G4ParticleGun;
class G4Event; 

class SiWGenericEcalPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  SiWGenericEcalPrimaryGeneratorAction();
  ~SiWGenericEcalPrimaryGeneratorAction();
  void PrintInfo(G4ParticleGun* gun);
  void GeneratePrimaries(G4Event* anEvent); //methode de G4VUserPrimaryGeneratorAction
private:
  void readConfig(std::vector<std::string> &particleVec,
		  std::vector<float> &energyVec,
		  std::vector<float> &etaVec,
		  std::vector<float> &phiVec);
};

#endif


