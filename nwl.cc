#include "nwlGeoModel.hh"
#include "nwlParticleSource.hh"
#include "nwlSteppingAction.hh"
#include "nwlEventAction.hh"
#include "nwlTrackingAction.hh"
#include "nwlRunAction.hh"

#include "G4ImportanceBiasing.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Earth.hh"
#include "QBBC.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"
#include "nwlConfigParser.hh"

int main(int argc,char** argv)
{
   if (argc != 2 && argc !=3)
    {
      G4cout << "Usage: ./nwl log.cfg RUN_NUMBER" << G4endl;
      return 0;
    }

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::MTwistEngine);
  
  G4RunManager* runManager = new G4RunManager;

  nwlConfigParser* cfg = nwlConfigParser::Instance();
  cfg->SetFilename(std::string(argv[1]));

  std::cout << std::string(argv[1]) << std::endl;

  // Geometry description
  nwlGeoModel* geo = new nwlGeoModel();
  runManager->SetUserInitialization(geo);

  // Physics list
  G4VModularPhysicsList* physicsList = new QBBC;//Earth;
  physicsList->SetVerboseLevel(0);
  
  if( geo->DoImportanceSampling() )
      physicsList->RegisterPhysics(new G4ImportanceBiasing(geo->GetSampler()));

  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserAction(new nwlParticleSource());
  runManager->SetUserAction(new nwlRunAction());  
  runManager->SetUserAction(new nwlEventAction());  
  runManager->SetUserAction(new nwlTrackingAction());
  runManager->SetUserAction(new nwlSteppingAction());

  // Initialize G4 kernel
  runManager->Initialize();

  if( geo->DoImportanceSampling() )
      geo->ConfigureImportanceSampling();

  physicsList->DumpCutValuesTable();

  if(argc==2) // Interactive
  {
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
    delete visManager;
  }
  else if(argc==3) // Batch mode
  {
      long evnb =  cfg->GetNumberOfEvents();
      G4cout << "Run " << evnb << " events" << G4endl;
      runManager->BeamOn(evnb);
      G4cout << "Done" << G4endl;
  }

  delete runManager;
  return 0;
}
