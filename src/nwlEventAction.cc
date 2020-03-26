#include "nwlEventAction.hh"

#include "nwlRunAction.hh"
#include "nwlAnalysis.hh"
#include "nwlSteppingAction.hh"
#include "nwlParticleSource.hh"
#include <time.h>

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Run.hh"

nwlEventAction* nwlEventAction::fgInstance = 0;

nwlEventAction* nwlEventAction::Instance()
{
  return fgInstance;
}      

nwlEventAction::nwlEventAction()
: G4UserEventAction(),
  fPrintModulo(100)
{ 
  fgInstance = this;
}

nwlEventAction::~nwlEventAction()
{ 
  fgInstance = 0;
}

void nwlEventAction::BeginOfEventAction(const G4Event* event)
{
  G4long total = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
  fPrintModulo = G4int(total/20);
  if(fPrintModulo<1) fPrintModulo=1;
  
  G4int eventNb = event->GetEventID();
  if (eventNb%fPrintModulo == 0) {  
       char outstr[200];
       time_t t;
       struct tm *tmp;

       t = time(NULL);
       tmp = localtime(&t);
       strftime(outstr, sizeof(outstr), "%H%M%S.%d%m%Y", tmp);
  
       G4cout << "\n---> Begin of event: " << eventNb << " " << outstr << G4endl;
     } 
}

void nwlEventAction::StoreParticleInfo(nwlParticleInfo& pinfo)
{
    particles.push_back(pinfo);
}

void nwlEventAction::EndOfEventAction(const G4Event* event)
{
 // nwlRunAction* ra = nwlRunAction::Instance();

  // Store particle info to file and histo
  auto analysisManager = G4AnalysisManager::Instance();

  nwlParticleInfoVector::iterator it;
  for (it=particles.begin(); it!=particles.end(); ++it)
  {
     // one may put some selection logic here
//     (*it).Write(ra->GetStream());
 
     G4int counter = 0;
     analysisManager->FillNtupleIColumn(counter++, event->GetEventID());
     analysisManager->FillNtupleIColumn(counter++, (*it).GetTrackID());
     analysisManager->FillNtupleIColumn(counter++, (*it).GetPDG());
     analysisManager->FillNtupleDColumn(counter++, (*it).GetOriginPoint().x());
     analysisManager->FillNtupleDColumn(counter++, (*it).GetOriginPoint().y());
     analysisManager->FillNtupleDColumn(counter++, (*it).GetOriginPoint().z());
     analysisManager->FillNtupleDColumn(counter++, (*it).GetOriginTime());
     analysisManager->FillNtupleDColumn(counter++, (*it).GetOriginKineticEnergy());
     analysisManager->FillNtupleSColumn(counter++, (*it).GetOriginVolumeName());
     analysisManager->FillNtupleSColumn(counter++, (*it).GetCreatorProcess());
     analysisManager->FillNtupleIColumn(counter++, (*it).GetOriginNucleusA());
     analysisManager->FillNtupleIColumn(counter++, (*it).GetOriginNucleusZ());
     analysisManager->FillNtupleSColumn(counter++, (*it).GetDetectorID());
     analysisManager->FillNtupleDColumn(counter++, (*it).GetEntrancePoint().x());
     analysisManager->FillNtupleDColumn(counter++, (*it).GetEntrancePoint().y());
     analysisManager->FillNtupleDColumn(counter++, (*it).GetEntrancePoint().z());
     analysisManager->FillNtupleDColumn(counter++, (*it).GetEntranceDirection().x());
     analysisManager->FillNtupleDColumn(counter++, (*it).GetEntranceDirection().y());
     analysisManager->FillNtupleDColumn(counter++, (*it).GetEntranceDirection().z());
     analysisManager->FillNtupleDColumn(counter++, (*it).GetDetectorTime());
     analysisManager->FillNtupleDColumn(counter++, (*it).GetDetectorKineticEnergy());
     analysisManager->FillNtupleSColumn(counter++, (*it).GetStopInDetectorID());
     analysisManager->FillNtupleSColumn(counter++, (*it).GetReactionInTheDetector());
     analysisManager->FillNtupleDColumn(counter++, (*it).GetWeight());
     analysisManager->AddNtupleRow();
  }
 
  Reset();

} 

void nwlEventAction::Reset()
{
  particles.clear();
}
