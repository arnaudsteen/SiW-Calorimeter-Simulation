#include "DetectorConstruction.hh"
#include "DetectorFieldSetup.hh"

#include "G4SDManager.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = 0; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction* DetectorConstruction::dcInstance = 0 ;

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
    f1CFPV(0),
    fWPV(0),
    f2CFPV(0),
    f1ThinCuPV(0),
    f1GapPV(0),
    f1PCBPV(0),
    f1SiliconPV(0), 
    f1WCuPV(0),
    fThickCuPV(0),
    f2WCuPV(0),
    f2SiliconPV(0),
    f2PCBPV(0),
    f2GapPV(0),
    f2ThinCuPV(0),
    fCheckOverlaps(true)
{
  fFieldSetup = new DetectorFieldSetup();
  _theDetectorConstructionMessenger = new DetectorConstructionMessenger(this);
  dcInstance=this;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete  f1CFPV;
  delete  fWPV;
  delete  f2CFPV;
  delete  f1ThinCuPV;
  delete  f1GapPV;
  delete  f1PCBPV;
  delete  f1SiliconPV; 
  delete  f1WCuPV;
  delete  fThickCuPV;
  delete  f2WCuPV;
  delete  f2SiliconPV;
  delete  f2PCBPV;
  delete  f2GapPV;
  delete  f2ThinCuPV;

  delete _theDetectorConstructionMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{ 
  // Lead material defined using NIST Manager
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_W",false); 
  nistManager->FindOrBuildMaterial("G4_Cu",false); 
  nistManager->FindOrBuildMaterial("G4_Si",false);
  nistManager->FindOrBuildMaterial("G4_AIR",false);
  nistManager->FindOrBuildMaterial("G4_C",false); 

  // Get materials
  defaultMat = G4Material::GetMaterial("G4_AIR");
  CFMat = G4Material::GetMaterial("G4_C");
  WMat = G4Material::GetMaterial("G4_W");
  CuMat = G4Material::GetMaterial("G4_Cu");
  SiMat = G4Material::GetMaterial("G4_Si");
  
  PCBMat = new G4Material("G10",1.700*g/cm3,4);
  PCBMat->AddElement( nistManager->FindOrBuildElement(14), 1);
  PCBMat->AddElement( nistManager->FindOrBuildElement(8) , 2);
  PCBMat->AddElement( nistManager->FindOrBuildElement(6) , 3);
  PCBMat->AddElement( nistManager->FindOrBuildElement(1) , 3);
    
  WCuMat = new G4Material("WCu",14.979*g/cm3,2);
  WCuMat->AddMaterial( G4Material::GetMaterial("G4_W") , 75*perCent);
  WCuMat->AddMaterial( G4Material::GetMaterial("G4_Cu") , 25*perCent);

  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  siWEcalGeom.moduleThickness=0.0*CLHEP::mm;
  siWEcalGeom.moduleThickness += siWEcalGeom.CF_thickness ;
  siWEcalGeom.moduleThickness += siWEcalGeom.wThickness ;
  siWEcalGeom.moduleThickness += siWEcalGeom.CF_thickness ;
  siWEcalGeom.moduleThickness += siWEcalGeom.thinCu_thickness ;
  siWEcalGeom.moduleThickness += siWEcalGeom.GAP_thickness ;
  siWEcalGeom.moduleThickness += siWEcalGeom.PCB_thickness ;
  siWEcalGeom.moduleThickness += siWEcalGeom.siThickness ;
  siWEcalGeom.moduleThickness += siWEcalGeom.wcuThickness ;
  siWEcalGeom.moduleThickness += siWEcalGeom.thickCu_thickness ;
  siWEcalGeom.moduleThickness += siWEcalGeom.wcuThickness ;
  siWEcalGeom.moduleThickness += siWEcalGeom.siThickness ;
  siWEcalGeom.moduleThickness += siWEcalGeom.PCB_thickness ;
  siWEcalGeom.moduleThickness += siWEcalGeom.GAP_thickness ;
  siWEcalGeom.moduleThickness += siWEcalGeom.thinCu_thickness ;
    
  // World
  siWEcalGeom.calorThickness = 0.0*CLHEP::mm;
  siWEcalGeom.calorThickness=siWEcalGeom.moduleThickness*siWEcalGeom.nModulePerSection;
  siWEcalGeom.calorSizeXY  = siWEcalGeom.nPixels*siWEcalGeom.pixelSize;
  siWEcalGeom.worldSizeXY = 3.0 * siWEcalGeom.calorSizeXY;
  siWEcalGeom.worldSizeZ  = 20.0 * siWEcalGeom.calorThickness;

  if( fabs(siWEcalGeom.primaryVertexPosition + siWEcalGeom.calorThickness/2 ) > siWEcalGeom.worldSizeZ/2 ){
    std::cout << "Warning primaryVertexPosition too far from detector -> move to world boundary" << std::endl;
    siWEcalGeom.primaryVertexPosition = -siWEcalGeom.worldSizeZ/2;
  }

  G4cout << "siWEcalGeom.worldSizeZ = " << siWEcalGeom.worldSizeZ << "\n"
	 << "siWEcalGeom.worldSizeXY = " << siWEcalGeom.worldSizeXY << "\n"
    	 << "siWEcalGeom.calorThickness = " << siWEcalGeom.calorThickness << "\n"
	 << "siWEcalGeom.calorSizeXY = " << siWEcalGeom.calorSizeXY << "\n"
	 << "siWEcalGeom.moduleThickness = " << siWEcalGeom.moduleThickness << "\n"
	 << G4endl;
  
  G4VSolid* worldS = new G4Box("World", siWEcalGeom.worldSizeXY/2, siWEcalGeom.worldSizeXY/2, siWEcalGeom.worldSizeZ/2);
  G4LogicalVolume* worldLV = new G4LogicalVolume(worldS, defaultMat, "World");
  G4VPhysicalVolume* worldPV = new G4PVPlacement(0, G4ThreeVector(), worldLV, "World", 0, false, 0, fCheckOverlaps);
  
  // Calorimeter
  G4VSolid* calorimeterS = new G4Box("Calorimeter", siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorThickness/2); 
  G4LogicalVolume* calorLV = new G4LogicalVolume(calorimeterS, defaultMat, "Calorimeter"); 
  new G4PVPlacement(0, G4ThreeVector(), calorLV, "Calorimeter", worldLV, false, 0, fCheckOverlaps); 
  
  //Module
  std::string name("module");
  G4VSolid* moduleS = new G4Box(name,siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.moduleThickness/2);
  G4LogicalVolume* moduleLV = new G4LogicalVolume(moduleS, defaultMat, name);
  new G4PVReplica(name, moduleLV, calorLV, kZAxis, siWEcalGeom.nModulePerSection, siWEcalGeom.moduleThickness);

  // Z position for module components
  // absorber
  G4double CF1_Z = -siWEcalGeom.moduleThickness/2 + siWEcalGeom.CF_thickness/2 ;
  G4double W_Z = CF1_Z + siWEcalGeom.CF_thickness/2 + siWEcalGeom.wThickness/2 ;
  G4double CF2_Z = W_Z + siWEcalGeom.wThickness/2 + siWEcalGeom.CF_thickness/2 ;
  // cassette
  G4double thinCu1_Z = CF2_Z + siWEcalGeom.CF_thickness/2 + siWEcalGeom.thinCu_thickness/2 ;
  G4double GAP1_Z = thinCu1_Z + siWEcalGeom.thinCu_thickness/2 + siWEcalGeom.GAP_thickness/2 ;
  G4double PCB1_Z = GAP1_Z + siWEcalGeom.GAP_thickness/2 + siWEcalGeom.PCB_thickness/2;
  G4double Si1_Z = PCB1_Z + siWEcalGeom.PCB_thickness/2 + siWEcalGeom.siThickness/2 ;
  G4double WCu1_Z = Si1_Z + siWEcalGeom.siThickness/2 + siWEcalGeom.wcuThickness/2 ;
  G4double thickCu_Z = WCu1_Z + siWEcalGeom.wcuThickness/2 + siWEcalGeom.thickCu_thickness/2 ;
  G4double WCu2_Z = thickCu_Z + siWEcalGeom.thickCu_thickness/2 + siWEcalGeom.wcuThickness/2 ;
  G4double Si2_Z = WCu2_Z + siWEcalGeom.wcuThickness/2 + siWEcalGeom.siThickness/2 ;
  G4double PCB2_Z = Si2_Z + siWEcalGeom.siThickness/2 + siWEcalGeom.PCB_thickness/2 ;
  G4double GAP2_Z = PCB2_Z + siWEcalGeom.PCB_thickness/2 + siWEcalGeom.GAP_thickness/2 ;
  G4double thinCu2_Z = GAP2_Z + siWEcalGeom.GAP_thickness/2 + siWEcalGeom.thinCu_thickness/2 ;
  //

  //
  siWEcalGeom.Silicon0RelativePositionZ=Si1_Z;
  siWEcalGeom.Silicon1RelativePositionZ=Si2_Z;
  //

  // visualisation attributes
  G4VisAttributes* CF_Attributes = new G4VisAttributes(G4Colour::Grey());
  CF_Attributes->SetForceSolid(true);
  G4VisAttributes* W_Attributes = new G4VisAttributes(G4Colour::Cyan());
  W_Attributes->SetForceSolid(true);
  G4VisAttributes* Cu_Attributes = new G4VisAttributes(G4Colour::Red());
  Cu_Attributes->SetForceSolid(true);
  G4VisAttributes* Gap_Attributes = new G4VisAttributes(G4Colour::White());
  Gap_Attributes->SetForceSolid(true);
  G4VisAttributes* PCB_Attributes = new G4VisAttributes(G4Colour::Green());
  PCB_Attributes->SetForceSolid(true);
  G4VisAttributes* Silicon_Attributes = new G4VisAttributes(G4Colour::Blue());
  Silicon_Attributes->SetForceSolid(true);
  G4VisAttributes* WCu_Attributes = new G4VisAttributes(G4Colour::Yellow());
  WCu_Attributes->SetForceSolid(true);
  //  

  // Carbon fiber 1
  G4VSolid* CF1_S = new G4Box("CF1", siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.CF_thickness/2);
  G4LogicalVolume* CF1_LV = new G4LogicalVolume(CF1_S, CFMat, "CF1"); 
  CF1_LV->SetVisAttributes(CF_Attributes);
  f1CFPV = new G4PVPlacement(0, G4ThreeVector(0., 0., CF1_Z), CF1_LV, "CF1", moduleLV, false, 0, fCheckOverlaps); 

  // Tungsten plate
  G4VSolid* W_S = new G4Box("W", siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.wThickness/2);
  G4LogicalVolume* W_LV = new G4LogicalVolume(W_S, WMat, "W"); 
  W_LV->SetVisAttributes(W_Attributes);
  fWPV = new G4PVPlacement(0, G4ThreeVector(0., 0., W_Z), W_LV, "W", moduleLV, false, 0, fCheckOverlaps); 
  
  // Carbon fiber 2
  G4VSolid* CF2_S = new G4Box("CF2", siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.CF_thickness/2);
  G4LogicalVolume* CF2_LV = new G4LogicalVolume(CF2_S, CFMat, "CF2"); 
  CF2_LV->SetVisAttributes(CF_Attributes);
  f2CFPV = new G4PVPlacement(0, G4ThreeVector(0., 0., CF2_Z), CF2_LV, "CF2", moduleLV, false, 0, fCheckOverlaps);
  
  // Thin cupper plate 1
  G4VSolid* thinCu1_S = new G4Box("thinCu1", siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.thinCu_thickness/2);
  G4LogicalVolume* thinCu1_LV = new G4LogicalVolume(thinCu1_S, CuMat, "thinCu1"); 
  thinCu1_LV->SetVisAttributes(Cu_Attributes);
  f1ThinCuPV = new G4PVPlacement(0, G4ThreeVector(0., 0., thinCu1_Z), thinCu1_LV, "thinCu1", moduleLV, false, 0, fCheckOverlaps);
  
  // Air GAP 1
  G4VSolid* GAP1_S = new G4Box("GAP1", siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.GAP_thickness/2);
  G4LogicalVolume* GAP1_LV = new G4LogicalVolume(GAP1_S, defaultMat, "GAP1"); 
  GAP1_LV->SetVisAttributes(Gap_Attributes);
  f1GapPV = new G4PVPlacement(0, G4ThreeVector(0., 0., GAP1_Z), GAP1_LV, "GAP1", moduleLV, false, 0, fCheckOverlaps);

  // PCB 1
  G4VSolid* PCB1_S = new G4Box("PCB1", siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.PCB_thickness/2);
  G4LogicalVolume* PCB1_LV = new G4LogicalVolume(PCB1_S, PCBMat, "PCB1"); 
  PCB1_LV->SetVisAttributes(PCB_Attributes);
  f1PCBPV = new G4PVPlacement(0, G4ThreeVector(0., 0., PCB1_Z), PCB1_LV, "PCB1", moduleLV, false, 0, fCheckOverlaps);

  // Silicon layer 1
  G4VSolid* Silicon1_S = new G4Box("Silicon1", siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.siThickness/2);
  G4LogicalVolume* Silicon1_LV = new G4LogicalVolume(Silicon1_S, SiMat, "Silicon1");
  Silicon1_LV->SetFieldManager( fFieldSetup->GetLocalFieldManager(), true);
  Silicon1_LV->SetVisAttributes(Silicon_Attributes);
  f1SiliconPV = new G4PVPlacement(0, G4ThreeVector(0., 0., Si1_Z), Silicon1_LV, "Silicon1", moduleLV, false, 0, fCheckOverlaps);

  // Tungsten/copper plate 1
  G4VSolid* WCu1_S = new G4Box("WCu1", siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.wcuThickness/2);
  G4LogicalVolume* WCu1_LV = new G4LogicalVolume(WCu1_S, WCuMat, "WCu1"); 
  WCu1_LV->SetVisAttributes(WCu_Attributes);
  f1WCuPV = new G4PVPlacement(0, G4ThreeVector(0., 0., WCu1_Z), WCu1_LV, "WCu1", moduleLV, false, 0, fCheckOverlaps);

  // Thick cupper plate 1
  G4VSolid* thickCu_S = new G4Box("thickCu", siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.thickCu_thickness/2);
  G4LogicalVolume* thickCu_LV = new G4LogicalVolume(thickCu_S, CuMat, "thickCu1"); 
  thickCu_LV->SetVisAttributes(Cu_Attributes);
  fThickCuPV = new G4PVPlacement(0, G4ThreeVector(0., 0., thickCu_Z), thickCu_LV, "thickCu", moduleLV, false, 0, fCheckOverlaps); 

  // Tungsten/copper plate 2
  G4VSolid* WCu2_S = new G4Box("WCu2", siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.wcuThickness/2);
  G4LogicalVolume* WCu2_LV = new G4LogicalVolume(WCu2_S, WCuMat, "WCu2"); 
  WCu2_LV->SetVisAttributes(WCu_Attributes);
  f2WCuPV = new G4PVPlacement(0, G4ThreeVector(0., 0., WCu2_Z), WCu2_LV, "WCu2", moduleLV, false, 0, fCheckOverlaps);

  // Silicon layer 2
  G4VSolid* Silicon2_S = new G4Box("Silicon2", siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.siThickness/2);
  G4LogicalVolume* Silicon2_LV = new G4LogicalVolume(Silicon2_S, SiMat, "Silicon2");
  Silicon2_LV->SetFieldManager( fFieldSetup->GetLocalFieldManager(), true);
  Silicon2_LV->SetVisAttributes(Silicon_Attributes);
  f2SiliconPV = new G4PVPlacement(0, G4ThreeVector(0., 0., Si2_Z), Silicon2_LV, "Silicon2", moduleLV, false, 0, fCheckOverlaps);

  // PCB 2
  G4VSolid* PCB2_S = new G4Box("PCB2", siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.PCB_thickness/2);
  G4LogicalVolume* PCB2_LV = new G4LogicalVolume(PCB2_S, PCBMat, "PCB2"); 
  PCB2_LV->SetVisAttributes(PCB_Attributes);
  f2PCBPV = new G4PVPlacement(0, G4ThreeVector(0., 0., PCB2_Z), PCB2_LV, "PCB2", moduleLV, false, 0, fCheckOverlaps);

  // Air GAP 2
  G4VSolid* GAP2_S = new G4Box("GAP2", siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.GAP_thickness/2);
  G4LogicalVolume* GAP2_LV = new G4LogicalVolume(GAP2_S, defaultMat, "GAP2"); 
  GAP2_LV->SetVisAttributes(Gap_Attributes);
  f2GapPV = new G4PVPlacement(0, G4ThreeVector(0., 0., GAP2_Z), GAP2_LV, "GAP2", moduleLV, false, 0, fCheckOverlaps);
  
  //Thin cupper plate 2
  G4VSolid* thinCu2_S = new G4Box("thinCu2", siWEcalGeom.calorSizeXY/2, siWEcalGeom.calorSizeXY/2, siWEcalGeom.thinCu_thickness/2);
  G4LogicalVolume* thinCu2_LV = new G4LogicalVolume(thinCu2_S, CuMat, "thinCu2"); 
  thinCu2_LV->SetVisAttributes(Cu_Attributes);
  f2ThinCuPV = new G4PVPlacement(0, G4ThreeVector(0., 0., thinCu2_Z), thinCu2_LV, "thinCu2", moduleLV, false, 0, fCheckOverlaps);
  

  //                                        
  // Visualization attributes
  //
  //  worldLV->SetVisAttributes (G4VisAttributes::Invisible);
  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(false);
  calorLV->SetVisAttributes(simpleBoxVisAtt);

  SiWSensitiveDetector* det0 = new SiWSensitiveDetector("Silicon0",1); //1 car le layer (volume parent) est replique.
  G4SDManager::GetSDMpointer()->AddNewDetector(det0);
  Silicon1_LV->SetSensitiveDetector(det0);
  
  SiWSensitiveDetector* det1 = new SiWSensitiveDetector("Silicon1",1); //1 car le layer (volume parent) est replique.
  G4SDManager::GetSDMpointer()->AddNewDetector(det1);
  Silicon2_LV->SetSensitiveDetector(det1);

  return worldPV;
}

void DetectorConstruction::ConstructSDandField()
{ 
  G4ThreeVector fieldValue = G4ThreeVector( siWEcalGeom.magField.x()*tesla,
					    siWEcalGeom.magField.y()*tesla,
					    siWEcalGeom.magField.z()*tesla);
  std::cout << "Magnetic Field = " << fieldValue << std::endl;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  G4AutoDelete::Register(fMagFieldMessenger);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
