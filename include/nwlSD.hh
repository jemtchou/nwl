#ifndef nwlSD_h
#define nwlSD_h 1

#include "G4VSensitiveDetector.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class nwlSD : public G4VSensitiveDetector
{
  public:
    nwlSD(const G4String& name);
    virtual ~nwlSD();
  
    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  private:

};

#endif
