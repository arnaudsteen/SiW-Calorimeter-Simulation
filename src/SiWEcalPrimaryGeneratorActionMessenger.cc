#include "SiWEcalPrimaryGeneratorActionMessenger.hh"
#include "SiWEcalPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
SiWEcalPrimaryGeneratorActionMessenger::SiWEcalPrimaryGeneratorActionMessenger(SiWEcalPrimaryGeneratorAction* PrimaryGenerator)
  :primaryGenerator(PrimaryGenerator)
{
  runDir = new G4UIdirectory("/GunParameter/");

  gunOptionPositionCmd =  new G4UIcmdWithAString("/GunParameter/gunOptionPosition",this);
  gunOptionPositionCmd->SetGuidance("Set primary gun option position");
  gunOptionPositionCmd->SetParameterName("gunOptionPosition",true);
  gunOptionPositionCmd->SetCandidates("default random cosmic beamtest");					

  gunOptionMomentumCmd =  new G4UIcmdWithAString("/GunParameter/gunOptionMomentum",this);
  gunOptionMomentumCmd->SetGuidance("Set primary gun option momentum");
  gunOptionMomentumCmd->SetParameterName("gunOptionMomentum",true);
  gunOptionMomentumCmd->SetCandidates("normal gaus uniform cosmic_gaus cosmic_uniform");					

  randPosMaxCmd = new G4UIcmdWithADouble("/GunParameter/randomMaxPosition",this);
  randPosMaxCmd->SetGuidance("Set max position parameter for random gun");
  randPosMaxCmd->SetParameterName("randMaxPos",true);
  randPosMaxCmd->SetDefaultValue(0.4);					

  gaussianMeanCmd = new G4UIcmdWithADouble("/GunParameter/gaussianMean",this);
  gaussianMeanCmd->SetGuidance("Set gaussian mean value for gaussian gun momentum");
  gaussianMeanCmd->SetParameterName("gaussianMean",true);
  gaussianMeanCmd->SetDefaultValue(0.0);
  
  gaussianSigmaCmd = new G4UIcmdWithADouble("/GunParameter/gaussianSigma",this);
  gaussianSigmaCmd->SetGuidance("Set gaussian sigma value for gaussian gun momentum");
  gaussianSigmaCmd->SetParameterName("gaussianSigma",true);
  gaussianSigmaCmd->SetDefaultValue(0.1);

  uniformParameterCmd = new G4UIcmdWithADouble("/GunParameter/uniformParameter",this);
  uniformParameterCmd->SetGuidance("Set uniform paramter value for uniform gun momentum");
  uniformParameterCmd->SetParameterName("uniformParameter",true);
  uniformParameterCmd->SetDefaultValue(0.1);

  xPositionSigmaCmd = new G4UIcmdWithADouble("/GunParameter/xPositionSigma",this);
  xPositionSigmaCmd->SetGuidance("Set gaussian sigma value for gaussian gun position (x) in mm");
  xPositionSigmaCmd->SetParameterName("xPositionSigma",true);
  xPositionSigmaCmd->SetDefaultValue(50);

  yPositionSigmaCmd = new G4UIcmdWithADouble("/GunParameter/yPositionSigma",this);
  yPositionSigmaCmd->SetGuidance("Set gaussian sigma value for gaussian gun position (y) in mm");
  yPositionSigmaCmd->SetParameterName("yPositionSigma",true);
  yPositionSigmaCmd->SetDefaultValue(50);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiWEcalPrimaryGeneratorActionMessenger::~SiWEcalPrimaryGeneratorActionMessenger()
{
  delete gunOptionPositionCmd;
  delete gunOptionMomentumCmd;
  
  delete randPosMaxCmd;
  delete gaussianMeanCmd;
  delete gaussianSigmaCmd;
  
  delete xPositionSigmaCmd;
  delete yPositionSigmaCmd;

  delete runDir;   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void SiWEcalPrimaryGeneratorActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  if( command == gunOptionPositionCmd )
    primaryGenerator->setGunOptionPosition(newValue);
  if( command == gunOptionMomentumCmd )
    primaryGenerator->setGunOptionMomentum(newValue);
  
  if(command == randPosMaxCmd)
    primaryGenerator->setRandomPosMax(randPosMaxCmd->GetNewDoubleValue(newValue));
  
  if(command == gaussianMeanCmd)
    primaryGenerator->setGaussianMean(gaussianMeanCmd->GetNewDoubleValue(newValue));
  if(command == gaussianSigmaCmd)
    primaryGenerator->setGaussianSigma(gaussianSigmaCmd->GetNewDoubleValue(newValue));
  
  if(command == xPositionSigmaCmd)
    primaryGenerator->setXPositionSigma(xPositionSigmaCmd->GetNewDoubleValue(newValue));
  if(command == yPositionSigmaCmd)
    primaryGenerator->setYPositionSigma(yPositionSigmaCmd->GetNewDoubleValue(newValue));

  if(command == uniformParameterCmd)
    primaryGenerator->setUniformParameter(randPosMaxCmd->GetNewDoubleValue(newValue));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
