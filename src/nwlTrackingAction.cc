#include "nwlTrackingAction.hh"

#include "nwlParticleInfo.hh"
#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "G4TrackingManager.hh"
#include "G4EventManager.hh"
#include "G4NeutronHPManager.hh"
#include "G4HadronicProcess.hh"
#include "G4Nucleus.hh"
#include "G4VProcess.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTable.hh"
#include "nwlEventAction.hh"
#include "nwlRunAction.hh"

#include <fstream>
#include <map>

G4int mPDG;
G4double mEnergy;
G4double mTime;
G4int mDetId;
G4int mHit;
G4double mXmax;
G4double mYmax;
G4double mZmax;
G4double mRmax;
G4int mTrackId;
G4int mProc;
G4int mWeight;

std::ofstream fout;

nwlTrackingAction::nwlTrackingAction()
{
   fout.open("fout.csv");
   fout << "pdgcode,energy,time,detId,hit,xmax,ymax,zmax,rmax,trackId,process,w" << std::endl;

}

void nwlTrackingAction::PreUserTrackingAction(const G4Track* track)
{
  G4int pdg = track->GetDefinition()->GetPDGEncoding();

  G4int parentid = track->GetParentID();
  G4VUserTrackInformation* info = new nwlParticleInfo;
  G4Track *  theTrack( const_cast< G4Track * >( track ) );
  theTrack->SetUserInformation(info);
}

void nwlTrackingAction::PostUserTrackingAction(const G4Track* track) {
    G4String volname = track->GetVolume()->GetName();

    if(volname == "Detector1")
    { 
      mDetId = 1;
    } 
    else if (volname == "Detector2")
    { 
      mDetId = 2;
    } 
    else
      return;
 
    mPDG = track->GetDefinition()->GetPDGEncoding();
    mEnergy = track->GetTotalEnergy();
//    G4ThreeVector pos = track->GetPosition();
    mTime = track->GetGlobalTime();
    mTrackId = track->GetTrackID();
    mWeight = track->GetWeight();

    G4String pname;
    if(track->GetCreatorProcess())
        pname = track->GetCreatorProcess()->GetProcessName();
    else
        pname = "Generator";
    mProc = nwlRunAction::Instance()->process[pname];

    nwlParticleInfo* info = (nwlParticleInfo*)(track->GetUserInformation());

    mHit = 0;
    G4TrackVector* secondaries = G4EventManager::GetEventManager()->GetTrackingManager()->GimmeSecondaries();
    G4TrackVector::iterator it;
    for(it = secondaries->begin(); it!= secondaries->end(); it++)
    {
       G4int pdgcode = (*it)->GetDefinition()->GetPDGEncoding();
       if(pdgcode == 1000010030) // He3(n,p)H3, tritium detected
         {
           mHit = 1;
	 }

    }
    
   fout << mPDG<<","<<mEnergy<<","<<mTime<<","<<mDetId<<","<<mHit<<","<<mTrackId<<","<<mProc<<","<<mWeight<<std::endl;
   fout.flush();
}
