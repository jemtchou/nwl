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

  virtual void BeginOfRunAction(const G4Run*);
  virtual void   EndOfRunAction(const G4Run*);

  bool IsSensitive(G4String dname);

  void SetJobID(long j){jobID = j;}
  long GetJobID(){return jobID;}
  
  const std::map<G4int, std::string>& GetH1map() {return H1map;}
  const std::map<G4int, std::pair<std::string, std::string> >& GetH2map(){return H2map;}

  G4int GetProcessID(G4String name);
  G4int GetDetectorID(G4String name);
  
  std::ostream& GetStream();

private:
  std::vector <G4String> detectorId;
  std::map <G4String, int> process;
  std::map <G4String, int> detectorList;
  
  long jobID;

  std::ofstream fout;

  std::map<G4int, std::string> H1map;
  std::map<G4int, std::pair<std::string, std::string> > H2map;
};

#endif

