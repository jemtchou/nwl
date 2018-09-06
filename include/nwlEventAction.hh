#ifndef nwlEventAction_h
#define nwlEventAction_h 1

#include "G4UserEventAction.hh"
#include "nwlParticleInfo.hh"

class nwlEventAction : public G4UserEventAction
{
public:
  nwlEventAction();
  virtual ~nwlEventAction();
  
  static nwlEventAction* Instance();
  
  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);

  void StoreParticleInfo(nwlParticleInfo& pinfo);
  
  void Reset();
  
private:
  static nwlEventAction* fgInstance;  
  
  G4int     fPrintModulo;
  
  nwlParticleInfoVector particles;
};

#endif

    
