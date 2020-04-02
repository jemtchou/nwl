#include "nwlEventAction.hh"

#include "nwlRunAction.hh"
#include "nwlAnalysis.hh"
#include "nwlSteppingAction.hh"
#include "nwlParticleSource.hh"
#include "nwlConfigParser.hh"

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
  nwlRunAction* ra = nwlRunAction::Instance();

  // Store particle info to file and histo
  auto analysisManager = G4AnalysisManager::Instance();

  nwlConfigParser* cfg = nwlConfigParser::Instance();

  nwlParticleInfoVector::iterator it;
  
  // one may put some selection logic here
  //     (*it).Write(ra->GetStream());
  if(cfg->CreateH1())
    {
      const std::map<G4int, std::string>& h1map = ra->GetH1map();
	for (it=particles.begin(); it!=particles.end(); ++it)
	  {
	   std::map<G4int, std::string>::const_iterator h1;
	   for (h1 = h1map.begin(); h1 != h1map.end(); ++h1)
	     {
	       G4double val;
	       if((*h1).second == "Energy") { val = (*it).GetDetectorKineticEnergy(); }
	       else if ((*h1).second == "Time") { val = (*it).GetDetectorTime(); }
	       else if ((*h1).second == "X") { val = (*it).GetOriginPoint().x(); }
	       else if ((*h1).second == "Y") { val = (*it).GetOriginPoint().y(); }
	       else if ((*h1).second == "Z") { val = (*it).GetOriginPoint().z(); }
	       else if ((*h1).second == "ProcessID") { val = ra->GetProcessID((*it).GetCreatorProcess()); }
	       else if ((*h1).second == "NucleusA") { val = (*it).GetOriginNucleusA(); }
	       else if ((*h1).second == "NucleusZ") { val = (*it).GetOriginNucleusZ(); }
	       else if ((*h1).second == "DetectorID") { val = ra->GetDetectorID((*it).GetDetectorID()); }
	       else if ((*h1).second == "PDG") { val = (*it).GetPDG(); }
	       
	       G4double weight = (*it).GetWeight();
	       analysisManager->FillH1((*h1).first, val, weight);
	     }
	  }
    }

  if(cfg->CreateH2())
    {
      const std::map<G4int, std::pair<std::string, std::string> >& h2map = ra->GetH2map();
      for (it=particles.begin(); it!=particles.end(); ++it)
	{
	   std::map<G4int, std::pair<std::string, std::string> >::const_iterator h2;
	   for (h2 = h2map.begin(); h2 != h2map.end(); ++h2)
	     {
	       G4double valx, valy;
	       if((*h2).second.first == "Energy") { valx = (*it).GetDetectorKineticEnergy(); }
	       else if ((*h2).second.first == "Time") { valx = (*it).GetDetectorTime(); }
	       else if ((*h2).second.first == "X") { valx = (*it).GetOriginPoint().x(); }
	       else if ((*h2).second.first == "Y") { valx = (*it).GetOriginPoint().y(); }
	       else if ((*h2).second.first == "Z") { valx = (*it).GetOriginPoint().z(); }
	       else if ((*h2).second.first == "ProcessID")
		 { valx = ra->GetProcessID((*it).GetCreatorProcess()); }
	       else if ((*h2).second.first == "NucleusA") { valx = (*it).GetOriginNucleusA(); }
	       else if ((*h2).second.first == "NucleusZ") { valx = (*it).GetOriginNucleusZ(); }
	       else if ((*h2).second.first == "DetectorID")
		 { valx = ra->GetDetectorID((*it).GetDetectorID()); }
	       else if ((*h2).second.first == "PDG") { valx = (*it).GetPDG(); }

	       if((*h2).second.second == "Energy") { valy = (*it).GetDetectorKineticEnergy(); }
	       else if ((*h2).second.second == "Time") { valy = (*it).GetDetectorTime(); }
	       else if ((*h2).second.second == "X") { valy = (*it).GetOriginPoint().x(); }
	       else if ((*h2).second.second == "Y") { valy = (*it).GetOriginPoint().y(); }
	       else if ((*h2).second.second == "Z") { valy = (*it).GetOriginPoint().z(); }
	       else if ((*h2).second.second == "ProcessID")
		 { valy = ra->GetProcessID((*it).GetCreatorProcess()); }
	       else if ((*h2).second.second == "NucleusA") { valy = (*it).GetOriginNucleusA(); }
	       else if ((*h2).second.second == "NucleusZ") { valy = (*it).GetOriginNucleusZ(); }
	       else if ((*h2).second.second == "DetectorID")
		 { valy = ra->GetDetectorID((*it).GetDetectorID()); }
	       else if ((*h2).second.second == "PDG") { valy = (*it).GetPDG(); }
	       
	       G4double weight = (*it).GetWeight();
	       analysisManager->FillH2((*h2).first, valx, valy, weight);
	     }
	}
    }
   
  if(cfg->WriteNtuple())
    {
      for (it=particles.begin(); it!=particles.end(); ++it)
	{
 
	  if(! cfg->StoreAllParticles())
	    {
	      if ((*it).GetPDG() != 22 && (*it).GetPDG() != 2112) continue;
	    }
	  
	  G4int counter = 0;
	  analysisManager->FillNtupleIColumn(counter++, event->GetEventID());
	  analysisManager->FillNtupleIColumn(counter++, (*it).GetTrackID());
          analysisManager->FillNtupleIColumn(counter++, (*it).GetParentID());
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
    }
 
  Reset();

} 

void nwlEventAction::Reset()
{
  particles.clear();
}
