#include "SiWRunAction.hh"
#include "SiWRun.hh"
#include "SiWGenericRun.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

SiWRunAction::SiWRunAction()
{
 
  //  runMessenger = new SiWRunActionMessenger(this);

  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories 
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);

  // Book histograms, ntuple
    
  // Creating histograms
  analysisManager->CreateH1("1","Edep in absorber", 10000, 0., 100000*MeV);
  analysisManager->CreateH1("2","Edep in gap", 100, 0., 10*GeV);
  analysisManager->CreateH1("3","trackL in absorber", 100, 0., 10*m);
  analysisManager->CreateH1("4","trackL in gap", 100, 0., 500*cm);

  // Creating ntuple
  analysisManager->CreateNtuple("B4", "Edep and TrackL");
  analysisManager->CreateNtupleDColumn("Eabs");
  analysisManager->CreateNtupleDColumn("Egap");
  analysisManager->CreateNtupleDColumn("Labs");
  analysisManager->CreateNtupleDColumn("Lgap");
  analysisManager->FinishNtuple();
  
}

SiWRunAction::~SiWRunAction()
{
  delete G4AnalysisManager::Instance();
  //delete runMessenger;
}

G4Run* SiWRunAction::GenerateRun()
{ 
  return new SiWRun();

}

void SiWRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4String fileName = "SiW";
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiWRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->GetH1(1) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl; 
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl; 
    }
    
    G4cout << " EAbs : mean = " 
       << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") << G4endl;
    
    G4cout << " EGap : mean = " 
       << G4BestUnit(analysisManager->GetH1(2)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(2)->rms(),  "Energy") << G4endl;
    
    G4cout << " LAbs : mean = " 
      << G4BestUnit(analysisManager->GetH1(3)->mean(), "Length") 
      << " rms = " 
      << G4BestUnit(analysisManager->GetH1(3)->rms(),  "Length") << G4endl;

    G4cout << " LGap : mean = " 
      << G4BestUnit(analysisManager->GetH1(4)->mean(), "Length") 
      << " rms = " 
      << G4BestUnit(analysisManager->GetH1(4)->rms(),  "Length") << G4endl;
  }

  analysisManager->Write();
  analysisManager->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


SiWGenericRunAction::SiWGenericRunAction()
{
  G4RunManager::GetRunManager()->SetPrintProgress(1);     
}

SiWGenericRunAction::~SiWGenericRunAction()
{
  delete G4AnalysisManager::Instance();
}

G4Run* SiWGenericRunAction::GenerateRun()
{ 
  return new SiWGenericRun();

}

void SiWGenericRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiWGenericRunAction::EndOfRunAction(const G4Run* /*run*/)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
