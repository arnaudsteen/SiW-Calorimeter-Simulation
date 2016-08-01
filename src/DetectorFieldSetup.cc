#include "G4UniformElectricField.hh"
#include "G4EqMagElectricField.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"
#include "G4MagIntegratorDriver.hh"

#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"
#include "DetectorFieldSetup.hh"

//////////////////////////////////////////////////////////////////////////
//

//  Constructors:

DetectorFieldSetup::DetectorFieldSetup()
  :  fChordFinder(0), fLocalChordFinder(0), fStepper(0)
{
  fEMfield = new G4UniformElectricField(
					G4ThreeVector(0.0,0,0.0));
  fLocalEMfield = new G4UniformElectricField(
					     G4ThreeVector(0.0,0.0,80*CLHEP::kilovolt/CLHEP::mm));
  //      G4ThreeVector(0.0,0.0,0.0)); 

  //  fFieldMessenger = new DetectorFieldMessenger(this) ;  
 
  fEquation = new G4EqMagElectricField(fEMfield); ; 
  fLocalEquation = new G4EqMagElectricField(fLocalEMfield); 
 
  fMinStep     = 1*CLHEP::mm ; // minimal step of 1 mm is default
  fMinLocalStep     = 0.30*CLHEP::mm ; // minimal step of 1 mm is default
  fStepperType = 4 ;      // ClassicalRK4 is default stepper

  fFieldManager = GetGlobalFieldManager();
  fLocalFieldManager = new G4FieldManager();


  UpdateField();
}

/////////////////////////////////////////////////////////////////////////////////


DetectorFieldSetup::~DetectorFieldSetup()
{
  if(fChordFinder) delete fChordFinder;
  if(fStepper)     delete fStepper;
  if(fEquation)    delete fEquation;   
  if(fEMfield)     delete fEMfield;
  if(fLocalEMfield)     delete fLocalEMfield;
}

/////////////////////////////////////////////////////////////////////////////

void DetectorFieldSetup::UpdateField()
{
  SetStepper();
  G4cout<<"The minimal Global step is equal to "<<fMinStep/CLHEP::mm<<" mm"<<G4endl ;
  G4cout<<"The minimal RPCGap step is equal to "<<fMinLocalStep/CLHEP::mm<<" mm"<<G4endl ;

  fFieldManager->SetDetectorField(fEMfield );
  fLocalFieldManager->SetDetectorField(fLocalEMfield );

  if(fChordFinder) delete fChordFinder;
  if(fLocalChordFinder) delete fLocalChordFinder;

  fIntgrDriver = new G4MagInt_Driver(fMinStep, 
				     fStepper, 
                                     fStepper->GetNumberOfVariables() );
  fLocalIntgrDriver = new G4MagInt_Driver(fMinLocalStep, 
					  fStepper, 
					  fStepper->GetNumberOfVariables() );

  fChordFinder = new G4ChordFinder(fIntgrDriver);
  fLocalChordFinder = new G4ChordFinder( fLocalIntgrDriver);

  fFieldManager->SetChordFinder( fChordFinder );
  fLocalFieldManager->SetChordFinder( fLocalChordFinder );
}

/////////////////////////////////////////////////////////////////////////////
//
// Set stepper according to the stepper type
//

void DetectorFieldSetup::SetStepper()
{
  if(fStepper) delete fStepper;
  G4int nvar = 8;

  switch ( fStepperType ) 
    {
    case 0:  
      fStepper = new G4ExplicitEuler( fEquation,nvar ); 
      fLocalStepper = new G4ExplicitEuler( fLocalEquation,nvar ); 
      G4cout<<"G4ExplicitEuler is called"<<G4endl;     
      break;
    case 1:  
      fStepper = new G4ImplicitEuler( fEquation,nvar );      
      fLocalStepper = new G4ImplicitEuler( fLocalEquation,nvar );      
      G4cout<<"G4ImplicitEuler is called"<<G4endl;     
      break;
    case 2:  
      fStepper = new G4SimpleRunge( fEquation,nvar );        
      fLocalStepper = new G4SimpleRunge( fLocalEquation,nvar );        
      G4cout<<"G4SimpleRunge is called"<<G4endl;     
      break;
    case 3:  
      fStepper = new G4SimpleHeum( fEquation,nvar );         
      fLocalStepper = new G4SimpleHeum( fLocalEquation,nvar );         
      G4cout<<"G4SimpleHeum is called"<<G4endl;     
      break;
    case 4:  
      fStepper = new G4ClassicalRK4( fEquation,nvar );       
      fLocalStepper = new G4ClassicalRK4( fLocalEquation,nvar );       
      G4cout<<"G4ClassicalRK4 (default) is called"<<G4endl;     
      break;
    case 5:  
      fStepper =0 ; 
      fLocalStepper = 0 ; 
      G4cout<<"G4HelixExplicitEuler is not valid for electric field"<<G4endl;     
      break;
    case 6:  
      fStepper = 0;
      fLocalStepper = 0;
      G4cout<<"G4HelixImplicitEuler is not valid for electric field"<<G4endl;     
      break;
    case 7:  
      fStepper = 0;
      fLocalStepper = 0;
      G4cout<<"G4HelixSimpleRunge is not valid for electric field"<<G4endl;     
      break;
    case 8:  
      fStepper = 0;
      fLocalStepper = 0;      
      G4cout<<"G4CashKarpRKF45 is not valid for electric field"<<G4endl;     
      break;
    case 9:  
      fStepper = 0 ;       
      fLocalStepper = 0 ;    
      G4cout<<"G4RKG3_Stepper is not valid for electric field"<<G4endl;     
      break;
    default: 
      fStepper = 0;
      fLocalStepper = 0 ;   
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Set the value of the Global Field to fieldValue along Z
//

void DetectorFieldSetup::SetFieldValue(G4double fieldStrength)
{
  G4ThreeVector fieldSetVec(0.0, 0.0, fieldStrength);
  this->SetFieldValue( fieldSetVec ); 
  //    *************

}

///////////////////////////////////////////////////////////////////////////////
//
// Set the value of the Global Field
//

void DetectorFieldSetup::SetFieldValue(G4ThreeVector fieldVector)
{
  if(fLocalEMfield) delete fLocalEMfield;

  if(fieldVector != G4ThreeVector(0.,0.,0.))
    { 
      fLocalEMfield = new  G4UniformElectricField(fieldVector);
    }
  else 
    {
      fLocalEMfield = 0; 
    }

  UpdateField();
  GetGlobalFieldManager()->SetDetectorField(fLocalEMfield);
  fLocalEquation->SetFieldObj( fLocalEMfield ); 

}

////////////////////////////////////////////////////////////////////////////////
//
//  Utility method

G4FieldManager*  DetectorFieldSetup::GetGlobalFieldManager()
{
  return G4TransportationManager::GetTransportationManager()
    ->GetFieldManager();
}


// In place of G4UniformField::GetConstantFieldValue ...
// 
G4ThreeVector DetectorFieldSetup::GetConstantFieldValue()
{
  static G4double fieldValue[6],  position[4]; 
  position[0] = position[1] = position[2] = position[3] = 0.0; 

  fEMfield->GetFieldValue( position, fieldValue);
  G4ThreeVector fieldVec(fieldValue[0], fieldValue[1], fieldValue[2]); 

  return fieldVec;
}
