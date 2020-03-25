#include <iomanip>   

#include <CLHEP/Units/SystemOfUnits.h>

#include "globals.hh"
#include "G4ios.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsLEND.hh"

#include "nwlPhysicsList.hh"
#include "G4HadronPhysicsShielding.hh"

nwlPhysicsList::nwlPhysicsList(G4int verbose, G4String LEN_model)
{

  G4cout << "<<< Geant4 Physics List simulation engine: nwlPhysicsList"<<G4endl;
  G4cout <<G4endl<<G4endl;

  defaultCutValue = 0.7*CLHEP::mm;  
  SetVerboseLevel(verbose);

  // EM Physics
  RegisterPhysics( new G4EmPenelopePhysics(verbose));

  // Synchroton Radiation & GN Physics
  RegisterPhysics( new G4EmExtraPhysics(verbose) );

  // Decays 
  RegisterPhysics( new G4DecayPhysics(verbose) );
  RegisterPhysics( new G4RadioactiveDecayPhysics(verbose) );

  size_t find = LEN_model.find("LEND__");
  G4cout << LEN_model << G4endl;
  G4String evaluation;
  if ( find != G4String::npos )
  {
      G4cout << LEN_model << G4endl;
      evaluation=LEN_model;
      evaluation.erase(0,find+6);
      LEN_model="LEND";
  }

  // Hadron Elastic scattering
  if ( LEN_model == "HP" ) 
  {
     RegisterPhysics( new G4HadronElasticPhysicsHP(verbose) );
  }
  else if ( LEN_model == "LEND" ) 
  {
     RegisterPhysics( new G4HadronElasticPhysicsLEND(verbose,evaluation) );
     //G4DataQuestionaire itt(lend);
  }
  else 
  {
     G4cout << "Earth Physics List: Warning!" <<G4endl;
     G4cout << "\"" << LEN_model << "\" is not valid for the low energy neutorn model." <<G4endl;
     G4cout << "Neutron HP package will be used." <<G4endl;
     RegisterPhysics( new G4HadronElasticPhysicsHP(verbose) );
  } 

   // Hadron Physics
  G4HadronPhysicsShielding* hps = new G4HadronPhysicsShielding(verbose);
  if ( LEN_model == "HP" ) 
  {
     ;
  }
  else if ( LEN_model == "LEND" ) 
  {
     hps->UseLEND(evaluation); 
  }
  else 
  {
     //G4cout << "Earth Physics List: Warning." <<G4endl;
     //G4cout << "Name of Low Energy Neutron model " << LEN_model << " is invalid." <<G4endl;
     //G4cout << "Will use neutron HP package." <<G4endl;
  }
  RegisterPhysics( hps );

  // Stopping Physics
  RegisterPhysics( new G4StoppingPhysics(verbose) );

  // Ion Physics
  RegisterPhysics( new G4IonPhysics(verbose));
}

void nwlPhysicsList::SetCuts()
{
  if (verboseLevel >1){
    G4cout << "nwlPhysicsList::SetCuts:";
  }  
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
  //   the default cut value for all particle types 

  SetCutsWithDefault();   

  //Set proton cut value to 0 for producing low energy recoil nucleus 
  SetCutValue(0, "proton");    
  
}
