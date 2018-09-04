#ifndef nwlEventAction_h
#define nwlEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>

//typedef std::vector<int> VTrackId;
//typedef std::pair<int, VTrackId> PTrackId;

class nwlSteppingAction;

class nwlEventAction : public G4UserEventAction
{
public:
  nwlEventAction();
  virtual ~nwlEventAction();
  
  // static access method
  static nwlEventAction* Instance();
  
  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);
  
  void Reset();
  
  // get methods
//  nwlParticleRecordVector& GetParticleRecordVector(){return particles;};
  
//  void AddTrackId(int trackid);
//  bool CheckTrackId(int trackid);

//  void AddParticle(int trkid);
//  bool CheckParticle(int trkid);
//  int GetParticle(int trkid);
  
//  G4double GetRmax(){return Rmax;}
//  void SetRmax(G4double rmax) {Rmax=rmax;}

private:
  static nwlEventAction* fgInstance;  
  
  G4double Rmax; // maximum radius of neutron travel

  G4int     fPrintModulo;
  
//  nwlParticleRecordVector particles;
//  std::vector<int> vtrackid;
//  std::vector<PTrackId> vp;
};

#endif

    
