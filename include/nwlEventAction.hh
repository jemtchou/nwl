#ifndef nwlEventAction_h
#define nwlEventAction_h 1

#include "G4UserEventAction.hh"
#include "nwlParticleInfo.hh"

class nwlRunAction;

class nwlEventAction : public G4UserEventAction
{
public:
  nwlEventAction(nwlRunAction*);
  virtual ~nwlEventAction();
  
  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);

  nwlRunAction* GetRunAction(){return fRunAction;};
  void StoreParticleInfo(nwlParticleInfo& pinfo);
  
  void Reset();
  
private:
  G4int     fPrintModulo;
  nwlRunAction* fRunAction;  
  nwlParticleInfoVector particles;
};

#endif

    
