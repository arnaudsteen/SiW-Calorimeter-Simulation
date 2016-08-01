#ifndef DetectorFieldSetup_H
#define DetectorFieldSetup_H

#include "G4ElectricField.hh"
#include "G4UniformElectricField.hh"

class G4FieldManager;
class G4ChordFinder;
class G4EquationOfMotion;
class G4Mag_EqRhs;
class G4EqMagElectricField;
class G4MagIntegratorStepper;
class G4MagInt_Driver; 
///class DetectorFieldMessenger;
class DetectorFieldSetup 
{
public:

  DetectorFieldSetup(G4ThreeVector) ;  //  The value of the field
  DetectorFieldSetup() ;               //  A zero field

 ~DetectorFieldSetup() ;  
      
  void SetStepperType( G4int i) { fStepperType = i ; }

  void SetStepper();

  void SetMinStep(G4double s) { fMinStep = s ; }

  void UpdateField();

  void SetFieldValue(G4ThreeVector fieldVector);
  void SetFieldValue(G4double      fieldValue);
  G4ThreeVector GetConstantFieldValue();
  G4FieldManager*         GetLocalFieldManager() {return fLocalFieldManager;};

protected:

      // Find the global Field Manager
  G4FieldManager*         GetGlobalFieldManager() ;

private:
  G4FieldManager*         fFieldManager ;
  G4FieldManager*         fLocalFieldManager ;

  G4ChordFinder*          fChordFinder ;
  G4ChordFinder*          fLocalChordFinder ;

  G4EqMagElectricField*   fEquation ;
  G4EqMagElectricField*   fLocalEquation ;

  G4ElectricField*        fEMfield;
  G4ElectricField*        fLocalEMfield;
 
  G4ThreeVector           fElFieldValue ; 

  G4MagIntegratorStepper* fStepper ;
  G4MagIntegratorStepper* fLocalStepper ;

  G4MagInt_Driver*        fIntgrDriver;
  G4MagInt_Driver*        fLocalIntgrDriver;


  G4int                   fStepperType ;

  G4double                fMinStep ;
  G4double                fMinLocalStep ;
 
  //  DetectorFieldMessenger*      fFieldMessenger;

};

#endif
