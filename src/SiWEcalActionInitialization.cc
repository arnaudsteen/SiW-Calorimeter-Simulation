//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: SiWEcalActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file SiWEcalActionInitialization.cc
/// \brief Implementation of the SiWEcalActionInitialization class

#include "SiWEcalActionInitialization.hh"
#include "SiWEcalPrimaryGeneratorAction.hh"
#include "SiWGenericEcalPrimaryGeneratorAction.hh"
#include "SiWRunAction.hh"
#include "DetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiWEcalActionInitialization::SiWEcalActionInitialization
                            (DetectorConstruction* detConstruction)
			      : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiWEcalActionInitialization::~SiWEcalActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiWEcalActionInitialization::BuildForMaster() const
{
  SetUserAction(new SiWRunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiWEcalActionInitialization::Build() const
{
  SetUserAction(new SiWEcalPrimaryGeneratorAction);
  SetUserAction(new SiWRunAction);
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiWGenericEcalActionInitialization::SiWGenericEcalActionInitialization
                            (DetectorConstruction* detConstruction)
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiWGenericEcalActionInitialization::~SiWGenericEcalActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiWGenericEcalActionInitialization::BuildForMaster() const
{
  SetUserAction(new SiWGenericRunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiWGenericEcalActionInitialization::Build() const
{
  SetUserAction(new SiWGenericEcalPrimaryGeneratorAction);
  SetUserAction(new SiWGenericRunAction);
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
