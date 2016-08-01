#ifndef DetectorConstructionMessenger_h
#define DetectorConstructionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

//#include "DetectorConstruction.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3Vector;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstructionMessenger: public G4UImessenger
{
public:
  DetectorConstructionMessenger(DetectorConstruction* detConstruction);
  virtual ~DetectorConstructionMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);


private:
  DetectorConstruction* detectorConstruction;
  G4UIdirectory*        runDir;   
  G4UIcmdWithAnInteger* construction_NlayerCmd;
  G4UIcmdWithADouble*   construction_CFThicknessCmd;
  G4UIcmdWithADouble*   construction_WThicknessCmd;
  G4UIcmdWithADouble*   construction_ThinCuThicknessCmd;
  G4UIcmdWithADouble*   construction_GAPThicknessCmd;
  G4UIcmdWithADouble*   construction_PCBThicknessCmd;
  G4UIcmdWithADouble*   construction_SiThicknessCmd;
  G4UIcmdWithADouble*   construction_WCuThicknessCmd;
  G4UIcmdWithADouble*   construction_ThickCuThicknessCmd;
  G4UIcmdWithAnInteger* construction_NumberOfPixelsCmd;
  G4UIcmdWithADouble*   construction_PixelSizeCmd;
  G4UIcmdWithADouble*   construction_NoiseRateCmd;
  G4UIcmdWith3Vector*   construction_MagFieldCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
