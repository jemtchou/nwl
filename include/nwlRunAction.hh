#ifndef nwlRunAction_h
#define nwlRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <map>
#include <vector>
#include <fstream>

class G4Run;

class nwlRunAction : public G4UserRunAction
{
  public:
    nwlRunAction();
    virtual ~nwlRunAction();

    static nwlRunAction* Instance();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    bool IsSensitive(G4String dname);

    void SetJobID(long j){jobID = j;}
    long GetJobID(){return jobID;}

    std::ostream& GetStream();

  private:
    std::vector <G4String> detectorId;
    std::map <G4String, int> process;

    long jobID;

    std::ofstream fout;

    static nwlRunAction* fgInstance;
};

#endif

