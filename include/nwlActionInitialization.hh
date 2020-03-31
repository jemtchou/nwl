#ifndef nwlActionInitialization_h
#define nwlActionInitialization_h 1

#include "globals.hh"
#include "G4VUserActionInitialization.hh"

class nwlActionInitialization : public G4VUserActionInitialization
{
  public:
    nwlActionInitialization(G4int jobId);
    virtual ~nwlActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
  private:
    G4int jobID;
};

#endif

    
