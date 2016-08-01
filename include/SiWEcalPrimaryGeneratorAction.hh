#ifndef SiWEcalPrimaryGeneratorAction_h
#define SiWEcalPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4String.hh"
#include "globals.hh"
#include "iostream"
#include "vector"
#include "G4ThreeVector.hh" 
#include "SiWEcalPrimaryGeneratorActionMessenger.hh"
#include "TF1.h"

class G4ParticleGun;//genere un faisceau de particules dont on determine le nombre
class G4Event; //

class SiWEcalPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  SiWEcalPrimaryGeneratorAction();
  ~SiWEcalPrimaryGeneratorAction();

  inline void setGunOptionPosition(std::string option){gunOptionPosition=option;}
  inline void setGunOptionMomentum(std::string option){gunOptionMomentum=option;}

  inline void setRandomPosMax(double max){randMaxPos=max;}

  inline void setGaussianMean(double val){gaussianMean=val;}
  inline void setGaussianSigma(double val){gaussianSigma=val;}
  inline void setXPositionSigma(double val){_xPosSigma=val;}
  inline void setYPositionSigma(double val){_yPosSigma=val;}

  inline void setUniformParameter(double val){uniformParameter=val;}
  inline G4ThreeVector GetPrimaryGeneratorMomentum()const{return _primaryMom;}
  void PrintInfo();
  public:
  void GeneratePrimaries(G4Event* anEvent); //methode de G4VUserPrimaryGeneratorAction
private:
  G4ThreeVector  _primaryMom;
  G4ThreeVector  _primaryPos;
  G4ParticleGun* particleGun;
  SiWEcalPrimaryGeneratorActionMessenger* _thePrimaryGeneratorMessenger;

  std::string gunOptionPosition;
  std::string gunOptionMomentum;
  
  double randMaxPos;//max position parameter for random gun position option

  double gaussianMean;//gaussian mean value for gaussian gun momentum option
  double gaussianSigma;//gaussian sigma value for gaussian gun momentum option
  
  double uniformParameter;//uniform parameter value for uniform gun momentum option
  TF1* _gunPosition_x_function;
  TF1* _gunPosition_y_function;
  double _xPosSigma;
  double _yPosSigma;
};

#endif


