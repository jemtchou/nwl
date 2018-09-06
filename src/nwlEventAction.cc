#include "nwlEventAction.hh"

#include "nwlRunAction.hh"
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

void nwlEventAction::EndOfEventAction(const G4Event* /*event*/)
{
  nwlRunAction* ra = nwlRunAction::Instance();

  // Store particle info to file and histo
  nwlParticleInfoVector::iterator it;
  for (it=particles.begin(); it!=particles.end(); ++it)
  {
     // one may put some selection logic here
     (*it).Write(ra->GetStream());
  }
  //

  Reset();

} 

void nwlEventAction::Reset()
{
  particles.clear();
}
