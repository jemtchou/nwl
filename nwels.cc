#include "NWLSGeometry.hh"
#include "NWLSPrimaryGeneratorAction.hh"
#include "NWLSSteppingAction.hh"
#include "NWLSEventAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QGSP_BERT.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"

int main(int argc,char** argv)
{
  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::MTwistEngine);
  
  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // Detector construction
  runManager->SetUserInitialization(new NWLSGeometry());

  // Physics list
  G4VModularPhysicsList* physicsList = new QGSP_BERT;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);
    
  // User action initialization
  runManager->SetUserAction(new NWLSPrimaryGeneratorAction);
  runManager->SetUserAction(new NWLSEventAction());  
  runManager->SetUserAction(new NWLSSteppingAction());

  // Initialize G4 kernel
  //
  runManager->Initialize();
  
#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc!=1) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode : define UI session
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
    UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
    ui->SessionStart();
    delete ui;
#endif
  }

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}
