#ifndef SiWEcalPrimaryGeneratorActionMessenger_h
#define SiWEcalPrimaryGeneratorActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class SiWEcalPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SiWEcalPrimaryGeneratorActionMessenger: public G4UImessenger
{
public:
  SiWEcalPrimaryGeneratorActionMessenger(SiWEcalPrimaryGeneratorAction*);
  virtual ~SiWEcalPrimaryGeneratorActionMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);


private:
  SiWEcalPrimaryGeneratorAction*     primaryGenerator;
  G4UIdirectory*        runDir;   
  G4UIcmdWithAString* gunOptionPositionCmd;
  G4UIcmdWithAString* gunOptionMomentumCmd;
  G4UIcmdWithADouble* randPosMaxCmd; 
  G4UIcmdWithADouble* gaussianMeanCmd;
  G4UIcmdWithADouble* gaussianSigmaCmd;
  G4UIcmdWithADouble* uniformParameterCmd;
  G4UIcmdWithADouble* xPositionSigmaCmd;
  G4UIcmdWithADouble* yPositionSigmaCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
