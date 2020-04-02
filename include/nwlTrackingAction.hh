#ifndef nwlTrackingAction_h
#define nwlTrackingAction_h 1

#include "G4UserTrackingAction.hh"

class nwlEventAction;

class nwlTrackingAction : public G4UserTrackingAction {

  public:
    nwlTrackingAction(nwlEventAction*);
    virtual ~nwlTrackingAction(){};
   
    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

  private:
    nwlEventAction* fEventAction;
};

#endif
