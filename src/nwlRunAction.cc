#include "nwlRunAction.hh"
#include "nwlParticleSource.hh"
#include "nwlEventAction.hh"
#include "nwlSteppingAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

nwlRunAction* nwlRunAction::fgInstance = 0;

nwlRunAction* nwlRunAction::Instance()
{
  return fgInstance;
}

nwlRunAction::nwlRunAction()
: G4UserRunAction()
{
  fgInstance = this;
}

nwlRunAction::~nwlRunAction()
{}

void nwlRunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  process["Generator"] = 0;
  int j = 1;
  G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
  int k =0;
  for(k=0; k< ptable->entries(); ++k)
  {
    G4ParticleDefinition* particle = ptable->GetParticle(k);
    if(! particle) continue;
    G4ProcessManager* pmanager = particle->GetProcessManager();
    int i;
    for (i=0; i<pmanager->GetProcessListLength();i++){
      j++;
     // G4cout << "Process " << j << " " << (*(pmanager->GetProcessList()))[i]->GetProcessName() << " of " << particle->GetParticleName() << G4endl;
      process[(*(pmanager->GetProcessList()))[i]->GetProcessName()] = j;
    }
  }

  G4cout << "Process list" << G4endl;
  std::map<G4String,int>::iterator it;
  j=0;
  for(it=process.begin(); it!=process.end(); ++it)
  {
     (*it).second=j++;
  }
  for(it=process.begin(); it!=process.end(); ++it)
  {
     G4cout << (*it).first << " " << (*it).second << G4endl;
  }

  

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
    
  //initialize event cumulative quantities
  nwlEventAction::Instance()->Reset();
}

void nwlRunAction::EndOfRunAction(const G4Run* aRun)
{
/*  G4int nofEvents = aRun->GetNumberOfEvent();
  if (nofEvents == 0) return;
  
  // Compute dose
  //
  G4double energySum  = nwlEventAction::Instance()->GetEnergySum();
  G4double energy2Sum = nwlEventAction::Instance()->GetEnergy2Sum();
  G4double rms = energy2Sum - energySum*energySum/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

  G4double mass = nwlSteppingAction::Instance()->GetVolume()->GetMass();
  G4double dose = energySum/mass;
  G4double rmsDose = rms/mass;

  // Run conditions
  //
  const G4ParticleGun* particleGun 
    = PrimaryGeneratorAction::Instance()->GetParticleGun();
  G4String particleName 
    = particleGun->GetParticleDefinition()->GetParticleName();                       
  G4double particleEnergy = particleGun->GetParticleEnergy();
        
  // Print
  //  
  G4cout
     << "\n--------------------End of Run------------------------------\n"
     << " The run consists of " << nofEvents << " "<< particleName << " of "
     <<   G4BestUnit(particleEnergy,"Energy")      
     << "\n Dose in scoring volume " 
     << nwlSteppingAction::Instance()->GetVolume()->GetName() << " : " 
     << G4BestUnit(dose,"Dose")
     << " +- "                   << G4BestUnit(rmsDose,"Dose")
     << "\n------------------------------------------------------------\n"
     << G4endl;
*/
}
