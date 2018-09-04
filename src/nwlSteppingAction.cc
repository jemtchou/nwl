#include "nwlSteppingAction.hh"

#include "nwlParticleInfo.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "nwlEventAction.hh"

#include <cassert>

using namespace CLHEP;

nwlSteppingAction* nwlSteppingAction::fgInstance = 0;

std::ofstream nwlSteppingAction::fout;

nwlSteppingAction* nwlSteppingAction::Instance()
{
//	fout.open("pos.csv");
//	fout<<"id,x,y,z,time,kine"<<std::endl;
	return fgInstance;
}      

nwlSteppingAction::nwlSteppingAction()
: G4UserSteppingAction(),
  fVolume(0),
  fEnergy(0.)
{ 
  fgInstance = this;
}

nwlSteppingAction::~nwlSteppingAction()
{ 
//	fout.close();
}

void nwlSteppingAction::UserSteppingAction(const G4Step* step)
{
  // if(particleType->GetPDGEncoding()==2112)
     {
       G4ThreeVector pos = step->GetTrack()->GetPosition();
       G4double r = pos.perp();
       nwlParticleInfo* info = (nwlParticleInfo*)(step->GetTrack()->GetUserInformation());
     }
}

void nwlSteppingAction::Reset()
{
}

