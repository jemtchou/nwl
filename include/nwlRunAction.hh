#ifndef nwlRunAction_h
#define nwlRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <map>

class G4Run;

class nwlRunAction : public G4UserRunAction
{
  public:
    nwlRunAction();
    virtual ~nwlRunAction();

    static nwlRunAction* Instance();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    std::map <G4String, int> process;

    static nwlRunAction* fgInstance;
};

#endif

