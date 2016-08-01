#include "DetectorConstructionMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3Vector.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction* detConstruction)
  :detectorConstruction(detConstruction)
{
  runDir = new G4UIdirectory("/DetectorConstruction/");

  construction_NlayerCmd = new G4UIcmdWithAnInteger("/DetectorConstruction/Nlayer",this);
  construction_CFThicknessCmd = new G4UIcmdWithADouble("/DetectorConstruction/CFThickness",this);
  construction_WThicknessCmd = new G4UIcmdWithADouble("/DetectorConstruction/WThickness",this);
  construction_ThinCuThicknessCmd = new G4UIcmdWithADouble("/DetectorConstruction/ThinCuThickness",this);
  construction_GAPThicknessCmd = new G4UIcmdWithADouble("/DetectorConstruction/GAPThickness",this);
  construction_PCBThicknessCmd = new G4UIcmdWithADouble("/DetectorConstruction/PCBThickness",this);
  construction_SiThicknessCmd = new G4UIcmdWithADouble("/DetectorConstruction/SiThickness",this);
  construction_WCuThicknessCmd = new G4UIcmdWithADouble("/DetectorConstruction/WCuThickness",this);
  construction_ThickCuThicknessCmd = new G4UIcmdWithADouble("/DetectorConstruction/ThickCuThickness",this);
  construction_NumberOfPixelsCmd = new G4UIcmdWithAnInteger("/DetectorConstruction/NumberOfPixels",this);
  construction_PixelSizeCmd = new G4UIcmdWithADouble("/DetectorConstruction/PixelSize",this);
  construction_NoiseRateCmd = new G4UIcmdWithADouble("/DetectorConstruction/NoiseRate",this);
  construction_MagFieldCmd = new G4UIcmdWith3Vector("/DetectorConstruction/MagField",this);

  construction_NlayerCmd->SetGuidance("Set detector construction number of layers");
  construction_CFThicknessCmd->SetGuidance("Set detector construction carbon fiber thickness");
  construction_WThicknessCmd->SetGuidance("Set detector construction tungsten thickness");
  construction_ThinCuThicknessCmd->SetGuidance("Set detector construction thin cupper thickness");
  construction_GAPThicknessCmd->SetGuidance("Set detector construction air gap thickness");
  construction_PCBThicknessCmd->SetGuidance("Set detector construction PCB thickness");
  construction_SiThicknessCmd->SetGuidance("Set detector construction silicon thickness");
  construction_WCuThicknessCmd->SetGuidance("Set detector construction tungsten/cupper thickness");
  construction_ThickCuThicknessCmd->SetGuidance("Set detector construction thick cupper thickness");
  construction_NumberOfPixelsCmd->SetGuidance("Set number of pixels per layer (in one direction; assume square geom)");
  construction_PixelSizeCmd->SetGuidance("Set detector pixel size (assume square pixel)");
  construction_NoiseRateCmd->SetGuidance("Set pixel noise rate (should move this command to another messenger class)");
  construction_MagFieldCmd->SetGuidance("Set detector magnetic field (tesla)");

  construction_NlayerCmd->SetParameterName("Nlayer",true);
  construction_CFThicknessCmd->SetParameterName("CFThickness",true);
  construction_WThicknessCmd->SetParameterName("WThickness",true);
  construction_ThinCuThicknessCmd->SetParameterName("ThinCuThickness",true);
  construction_GAPThicknessCmd->SetParameterName("GAPThickness",true);
  construction_PCBThicknessCmd->SetParameterName("PCBThickness",true);
  construction_SiThicknessCmd->SetParameterName("SiThickness",true);
  construction_WCuThicknessCmd->SetParameterName("WCuThickness",true);
  construction_ThickCuThicknessCmd->SetParameterName("ThickCuThickness",true);
  construction_NumberOfPixelsCmd->SetParameterName("NumberOfPixels",true);
  construction_PixelSizeCmd->SetParameterName("PixelSize",true);
  construction_NoiseRateCmd->SetParameterName("NoiseRate",true);
  construction_MagFieldCmd->SetParameterName("MagField_x","MagField_y","MagField_z",true);

  construction_NlayerCmd->SetDefaultValue(28);
  construction_CFThicknessCmd->SetDefaultValue(0.5);
  construction_WThicknessCmd->SetDefaultValue(2.8);
  construction_ThinCuThicknessCmd->SetDefaultValue(0.5);
  construction_GAPThicknessCmd->SetDefaultValue(2.0);
  construction_PCBThicknessCmd->SetDefaultValue(2.0);
  construction_SiThicknessCmd->SetDefaultValue(0.3);
  construction_WCuThicknessCmd->SetDefaultValue(1.2);
  construction_ThickCuThicknessCmd->SetDefaultValue(6.0);
  construction_NumberOfPixelsCmd->SetDefaultValue(32);
  construction_PixelSizeCmd->SetDefaultValue(10.0);
  construction_NoiseRateCmd->SetDefaultValue(0.005);
  construction_MagFieldCmd->SetDefaultValue(G4ThreeVector(0.0,0.0,0.0));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
  delete construction_NlayerCmd;
  delete construction_CFThicknessCmd;
  delete construction_WThicknessCmd;
  delete construction_ThinCuThicknessCmd;
  delete construction_GAPThicknessCmd;
  delete construction_PCBThicknessCmd;
  delete construction_SiThicknessCmd;
  delete construction_WCuThicknessCmd;
  delete construction_ThickCuThicknessCmd;
  delete construction_NumberOfPixelsCmd;
  delete construction_PixelSizeCmd;
  delete construction_NoiseRateCmd;
  delete construction_MagFieldCmd;
  delete runDir;   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void DetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if( command == construction_NlayerCmd )
    detectorConstruction->setNumberOfLayer( construction_NlayerCmd->GetNewIntValue(newValue) );

  if( command == construction_CFThicknessCmd )
    detectorConstruction->setCupperFiberThickness( construction_CFThicknessCmd->GetNewDoubleValue(newValue) );

  if( command == construction_WThicknessCmd )
    detectorConstruction->setTungstenThickness( construction_WThicknessCmd->GetNewDoubleValue(newValue) );
  
  if( command == construction_ThinCuThicknessCmd )
    detectorConstruction->setThinCupperThickness( construction_ThinCuThicknessCmd->GetNewDoubleValue(newValue) );
  
  if( command == construction_GAPThicknessCmd )
    detectorConstruction->setGAPThickness( construction_GAPThicknessCmd->GetNewDoubleValue(newValue) );
  
  if( command == construction_PCBThicknessCmd )
    detectorConstruction->setPCBThickness( construction_PCBThicknessCmd->GetNewDoubleValue(newValue) );

  if( command == construction_SiThicknessCmd )
    detectorConstruction->setSiliconThickness( construction_SiThicknessCmd->GetNewDoubleValue(newValue) );

  if( command == construction_WCuThicknessCmd )
    detectorConstruction->setTungstenCupperThickness( construction_WCuThicknessCmd->GetNewDoubleValue(newValue) );

  if( command == construction_ThickCuThicknessCmd )
    detectorConstruction->setThickCupperThickness( construction_ThickCuThicknessCmd->GetNewDoubleValue(newValue) );

  if( command == construction_NumberOfPixelsCmd )
    detectorConstruction->setNumberOfPixel( construction_NumberOfPixelsCmd->GetNewIntValue(newValue) );

  if( command == construction_PixelSizeCmd )
    detectorConstruction->setThickCupperThickness( construction_PixelSizeCmd->GetNewDoubleValue(newValue) );

  if( command == construction_NoiseRateCmd )
    detectorConstruction->setNoiseRate( construction_NoiseRateCmd->GetNewDoubleValue(newValue) );

  if( command == construction_MagFieldCmd )
    detectorConstruction->setMagneticField( construction_MagFieldCmd->GetNew3VectorValue(newValue) );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
