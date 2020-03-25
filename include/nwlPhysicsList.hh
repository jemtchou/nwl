#ifndef nwlPhysicsList_h
#define nwlPhysicsList_h 1

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

class nwlPhysicsList : public G4VModularPhysicsList
{
public:
  nwlPhysicsList(G4int ver = 1, G4String LEN_model="HP");
  virtual ~nwlPhysicsList()=default;

  // delete copy constructor and assignment operator
  nwlPhysicsList(const nwlPhysicsList &)=delete;
  nwlPhysicsList & operator=(const nwlPhysicsList &right)=delete;

  virtual void SetCuts();
};

#endif



