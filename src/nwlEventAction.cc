#include "nwlEventAction.hh"

#include "nwlRunAction.hh"
#include "nwlSteppingAction.hh"
#include "nwlParticleSource.hh"
#include <time.h>

//#include "RootWriter.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Run.hh"

//#include "TH1F.h"
//#include "TTree.h"

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

void nwlEventAction::EndOfEventAction(const G4Event* /*event*/)
{
  Reset();

} 

void nwlEventAction::Reset()
{
}
