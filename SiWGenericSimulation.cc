#include "DetectorConstruction.hh"
#include "SiWEcalActionInitialization.hh"

#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "G4UIcommand.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "G4PhysListFactory.hh"

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " exampleSiWEcal [-m macro ] [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  G4String macro;
  G4String geom;
  G4String session;
  G4int seed=0;

  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
    else if ( G4String(argv[i]) == "-g" ) geom = argv[i+1];
    else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
    else if ( G4String(argv[i]) == "-s" ) seed = atoi(argv[i+1]);
    else {
      PrintUsage();
      return 1;
    }
  }  
  
  // Detect interactive mode (if no macro provided) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( ! macro.size() ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Choose the Random engine
  //
  //  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4Random::setTheSeed(seed);
  
  // Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Set mandatory initialization classes
  //
  DetectorConstruction* detConstruction = new DetectorConstruction();
  if ( geom.size() ) {
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+geom);
  }
  runManager->SetUserInitialization(detConstruction);

  // Default physics list FTFP_BERT
  // To change the physic list in shell env: export PHYSLIST = thePhysListYouWant (eg:QGSP_BERT(_HP), FTF_BIC, QGSP_BIC ...)
  G4PhysListFactory* physFactory = new G4PhysListFactory();
  runManager->SetUserInitialization(physFactory->ReferencePhysList());
    
  SiWGenericEcalActionInitialization* actionInitialization
    = new SiWGenericEcalActionInitialization(detConstruction);
  runManager->SetUserInitialization(actionInitialization);
  
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();


  // Process macro or start UI session
  //
  if ( macro.size() ) {
    // batch mode
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);
  }
  else  {  
    // interactive mode : define UI session
    UImanager->ApplyCommand("/control/execute mac/init_vis.mac");
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute mac/gui.mac");
    }
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
