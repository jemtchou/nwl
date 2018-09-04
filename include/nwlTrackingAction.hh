#ifndef nwlTrackingAction_h
#define nwlTrackingAction_h 1

#include "G4UserTrackingAction.hh"


class nwlTrackingAction : public G4UserTrackingAction {

  public:
    nwlTrackingAction();
    virtual ~nwlTrackingAction(){};
   
    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

};

#endif
