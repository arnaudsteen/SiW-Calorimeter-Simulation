#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "globals.hh"

#include "SiWSensitiveDetector.hh"
#include "DetectorConstructionMessenger.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;
class DetectorFieldSetup;

struct SiWEcalGeometryParametrisation{
  G4int nSection;//currently only work with nSection=1
  G4int nModulePerSection;
  
  G4double CF_thickness;
  G4double wThickness;
  G4double thinCu_thickness;
  G4double GAP_thickness;
  G4double PCB_thickness;
  G4double siThickness;
  G4double wcuThickness;
  G4double thickCu_thickness;

  G4int    nPixels; //assume square detector
  G4double pixelSize; //assume square pads
  G4double calorSizeXY;
  G4double calorThickness;
  G4double worldSizeXY;
  G4double worldSizeZ;

  G4double moduleThickness;
  G4double Silicon0RelativePositionZ;
  G4double Silicon1RelativePositionZ;

  G4double noiseRate;
  G4ThreeVector magField;

  G4double primaryVertexPosition; //relatively from 1st detector layer
  
  //will need to use these vectors when nSection>=2
  /*
  std::vector<G4int> nModulePerSection;
  std::vector<G4double> wThickness;
  std::vector<G4double> wcuThickness;
  std::vector<G4double> moduleThickness;
  */
  SiWEcalGeometryParametrisation()
  {
    nSection=1;
    nModulePerSection=14;
    CF_thickness=0.5*CLHEP::mm;
    wThickness=2.8*CLHEP::mm;
    thinCu_thickness=0.5*CLHEP::mm;
    GAP_thickness=2.0*CLHEP::mm;
    PCB_thickness=2.0*CLHEP::mm;
    siThickness=0.3*CLHEP::mm;
    wcuThickness=1.2*CLHEP::mm;
    thickCu_thickness=6.0*CLHEP::mm;

    nPixels=64; // no more than 99 if LCIO_OUTPUT -> hit cell id would have problem (see IJKtoKey in LCIOWriter.hh)
    pixelSize=10.0*CLHEP::mm;

    noiseRate=0.0;
    magField=G4ThreeVector(0.0,0.0,0.0); //no magnetic field by default

    primaryVertexPosition=-2800*CLHEP::mm;
    //nModulePerSection.push_back(5); nModulePerSection.push_back(5); nModulePerSection.push_back(4);
    //wThickness.push_back(2.0); wThickness.push_back(2.8); wThickness.push_back(4.2);
    //wcuThickness.push_back(0.6); wcuThickness.push_back(1.2); wcuThickness.push_back(2.2);
  }
};

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();

public:
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

  inline void setEcalGeomParams(SiWEcalGeometryParametrisation params){siWEcalGeom=params;}
  inline const SiWEcalGeometryParametrisation getEcalGeomParams(){return siWEcalGeom;}

  inline void setNumberOfLayer(int n){siWEcalGeom.nModulePerSection=n;}
  inline void setCupperFiberThickness(double val){siWEcalGeom.CF_thickness=val;}
  inline void setTungstenThickness(double val){siWEcalGeom.wThickness=val;}
  inline void setThinCupperThickness(double val){siWEcalGeom.thinCu_thickness=val;}
  inline void setGAPThickness(double val){siWEcalGeom.GAP_thickness=val;}
  inline void setPCBThickness(double val){siWEcalGeom.PCB_thickness=val;}
  inline void setSiliconThickness(double val){siWEcalGeom.siThickness=val;}
  inline void setTungstenCupperThickness(double val){siWEcalGeom.wcuThickness=val;}
  inline void setThickCupperThickness(double val){siWEcalGeom.thickCu_thickness=val;}
  inline void setNumberOfPixel(double val){siWEcalGeom.nPixels=val;} //assume square detector
  inline void setPixelSize(double val){siWEcalGeom.pixelSize=val;} //assume square cells
  inline void setNoiseRate(double val){siWEcalGeom.noiseRate=val;} //assume square cells
  inline void setMagneticField(G4ThreeVector vec){siWEcalGeom.magField=vec;} //assume square cells

  static DetectorConstruction* Instance(){return dcInstance;}
private:
  // methods
  //
  void DefineMaterials();
  G4VPhysicalVolume* DefineVolumes();
  
  // data members
  //
  static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger; 
  DetectorConstructionMessenger* _theDetectorConstructionMessenger;
  
  //absorber
  G4VPhysicalVolume* f1CFPV;
  G4VPhysicalVolume* fWPV;
  G4VPhysicalVolume* f2CFPV;
  // cassettes 
  G4VPhysicalVolume* f1ThinCuPV;
  G4VPhysicalVolume* f1GapPV;
  G4VPhysicalVolume* f1PCBPV;
  G4VPhysicalVolume* f1SiliconPV; 
  G4VPhysicalVolume* f1WCuPV;
  G4VPhysicalVolume* fThickCuPV;
  G4VPhysicalVolume* f2WCuPV;
  G4VPhysicalVolume* f2SiliconPV;
  G4VPhysicalVolume* f2PCBPV;
  G4VPhysicalVolume* f2GapPV;
  G4VPhysicalVolume* f2ThinCuPV;

  G4Material* defaultMat;
  G4Material* CFMat;
  G4Material* WMat;
  G4Material* CuMat;
  G4Material* PCBMat;
  G4Material* SiMat;
  G4Material* WCuMat;

  SiWEcalGeometryParametrisation siWEcalGeom;

  G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps

  static DetectorConstruction* dcInstance;

  DetectorFieldSetup* fFieldSetup;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

