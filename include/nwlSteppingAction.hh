#ifndef nwlSteppingAction_h
#define nwlSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

#include <fstream>

class G4LogicalVolume;

class nwlSteppingAction : public G4UserSteppingAction
{
  public:
    nwlSteppingAction();
    virtual ~nwlSteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

    // reset accumulated energy
    void Reset();

    // set methods
    void SetVolume(G4LogicalVolume* volume) { fVolume = volume; }
  
    // get methods
    G4LogicalVolume* GetVolume() const { return fVolume; }
    G4double GetEnergy() const { return fEnergy; }
   
  private:
    G4LogicalVolume* fVolume;
    G4double  fEnergy;
};

#endif
