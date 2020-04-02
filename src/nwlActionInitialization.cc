#include "nwlActionInitialization.hh"
#include "nwlParticleSource.hh"
#include "nwlRunAction.hh"
#include "nwlEventAction.hh"
#include "nwlTrackingAction.hh"
#include "nwlSteppingAction.hh"

nwlActionInitialization::nwlActionInitialization(G4int jobId)
 : G4VUserActionInitialization()
{
   jobID = jobId;
}

nwlActionInitialization::~nwlActionInitialization()
{}

void nwlActionInitialization::BuildForMaster() const
{
  nwlRunAction* runAction = new nwlRunAction;
  runAction->SetJobID(jobID);
  SetUserAction(runAction);
}

void nwlActionInitialization::Build() const
{
  SetUserAction(new nwlParticleSource);

  nwlRunAction* runAction = new nwlRunAction;
  runAction->SetJobID(jobID);
  SetUserAction(runAction);
  nwlEventAction* eventAction = new nwlEventAction(runAction);
  SetUserAction(eventAction);
  SetUserAction(new nwlTrackingAction(eventAction));
  SetUserAction(new nwlSteppingAction());
}  
