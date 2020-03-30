#include "nwlGeoModel.hh"
#include "nwlActionInitialization.hh"
#include "nwlPhysicsList.hh"

#include "G4ImportanceBiasing.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

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
  G4Random::setTheEngine(new CLHEP::MTwistEngine);
  
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  nwlConfigParser* cfg = nwlConfigParser::Instance();
  cfg->ReadConfig(std::string(argv[1]));

  // Geometry description
  nwlGeoModel* geo = new nwlGeoModel();
  runManager->SetUserInitialization(geo);

  // Physics list
  G4VModularPhysicsList* physicsList = new nwlPhysicsList;
  physicsList->SetVerboseLevel(0);
  
  if( geo->DoImportanceSampling() )
      physicsList->RegisterPhysics(new G4ImportanceBiasing(geo->GetSampler()));

  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new nwlActionInitialization(jobId));

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

  std::string gen = cfg->GetGeneratorMacro();
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/control/execute "+gen);

  if(argc==2) // Interactive
  {
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
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
