#include "nwlSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "nwlEventAction.hh"
#include "G4VProcess.hh"
#include "G4EventManager.hh"

using namespace CLHEP;

nwlSD::nwlSD(const G4String& name) : G4VSensitiveDetector(name)
{}

nwlSD::~nwlSD() 
{}

void nwlSD::Initialize(G4HCofThisEvent*)
{}

G4bool nwlSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{  
  G4Track* track = step->GetTrack();

  G4String dname = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();

  nwlParticleInfo* info = (nwlParticleInfo*)track->GetUserInformation();

  if (info->GetDetectorID() == "")
    {
       info->SetDetectorInfo(dname, 
			     track->GetGlobalTime(), 
                             track->GetKineticEnergy(), 
                             step->GetPreStepPoint()->GetPosition(), 
                             track->GetMomentumDirection(), 
                             track->GetWeight());
       return true;
    }
  return false;
}

void nwlSD::EndOfEvent(G4HCofThisEvent*)
{

}

