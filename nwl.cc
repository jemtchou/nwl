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

  long jobId = 0;
  if ( argc == 3)
     jobId = atol(argv[2]); 

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  
  G4RunManager* runManager = new G4RunManager;

  nwlConfigParser* cfg = nwlConfigParser::Instance();
  cfg->SetFilename(std::string(argv[1]));

  std::cout << "Reading config file " << std::string(argv[1]) << std::endl;

  // Geometry description
  nwlGeoModel* geo = new nwlGeoModel();
  runManager->SetUserInitialization(geo);

  // Physics list
  G4VModularPhysicsList* physicsList = new Earth;
  physicsList->SetVerboseLevel(0);
  
  if( geo->DoImportanceSampling() )
      physicsList->RegisterPhysics(new G4ImportanceBiasing(geo->GetSampler()));

  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserAction(new nwlParticleSource());

  nwlRunAction* ra = new nwlRunAction();
  ra->SetJobID(jobId);
  runManager->SetUserAction(ra);  
  runManager->SetUserAction(new nwlEventAction());  
  runManager->SetUserAction(new nwlTrackingAction());
  runManager->SetUserAction(new nwlSteppingAction());

  // Initialize G4 kernel
  runManager->Initialize();

  if( geo->DoImportanceSampling() )
      geo->ConfigureImportanceSampling();

  physicsList->DumpCutValuesTable();

  long rand[2];
  rand[0] = long(jobId*100000000 + 123456789);
  rand[1] = 123456789;
  const long* rand1 = rand;
  G4Random::setTheSeeds(rand1);

  if(argc==2) // Interactive
  {
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/control/execute vis.mac");
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
