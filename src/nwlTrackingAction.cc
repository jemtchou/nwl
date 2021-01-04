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
#include "G4VEmProcess.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTable.hh"
#include "nwlEventAction.hh"
#include "nwlRunAction.hh"

#include <fstream>
#include <map>

nwlTrackingAction::nwlTrackingAction(nwlEventAction* eventAction) : fEventAction(eventAction)
{ }

void nwlTrackingAction::PreUserTrackingAction(const G4Track* track)
{
  G4int pdg = track->GetDefinition()->GetPDGEncoding();
  G4int na = 0;
  G4int nz = 0;
  G4String pname;
  const G4VProcess* proc = track->GetCreatorProcess();

  if(proc)
  {
        pname = proc->GetProcessName();
	if(proc->GetProcessType()==fHadronic)
	{
		const G4Nucleus* nucleus = ((G4HadronicProcess*)proc)->GetTargetNucleus();
		na = nucleus->GetA_asInt();
  		nz = nucleus->GetZ_asInt();
	}
        else if (proc->GetProcessType()==fElectromagnetic)
        {
                G4int subtype = ((G4VEmProcess*)proc)->GetProcessSubType();
  //              G4cout << "-->" << proc->GetProcessName() << " " << subtype << G4endl;

                if(subtype != 2 && subtype !=3 && subtype !=10) // no eBrem, eIoni, msc
                {
                  const G4Element* elem = ((G4VEmProcess*)proc)->GetCurrentElement();
                  if(elem) {
                  nz = elem->GetZasInt();
                  na = elem->GetN(); }
  //		  G4cout << "EM " << nz << " " << na << G4endl; 
                }
        }

  }
  else
        pname = "Generator";

  G4int parentid = track->GetParentID();
 
  nwlParticleInfo* info = new nwlParticleInfo;
  info->SetOriginInfo(track->GetTrackID(), parentid, pdg, track->GetKineticEnergy(), track->GetGlobalTime(), track->GetPosition(), track->GetVolume(), pname, na, nz);

  G4Track*  theTrack( const_cast< G4Track * >( track ) );
  theTrack->SetUserInformation(info);
}

void nwlTrackingAction::PostUserTrackingAction(const G4Track* track) {
    G4String volname = track->GetVolume()->GetName();
    G4bool stopInDet = false;
    G4String detId = "";
    if ( fEventAction->GetRunAction()->IsSensitive(volname) )
    {
	detId = volname;
        stopInDet = true;
    }

    nwlParticleInfo* info = (nwlParticleInfo*)(track->GetUserInformation());
 
    G4VProcess* proc =  G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager()->GetfCurrentProcess();

    info->SetFinalInfo(stopInDet, detId, proc->GetProcessName());

    /*G4TrackVector* secondaries = G4EventManager::GetEventManager()->GetTrackingManager()->GimmeSecondaries();
    G4TrackVector::iterator it;
    for(it = secondaries->begin(); it!= secondaries->end(); it++)
    {
       G4int pdgcode = (*it)->GetDefinition()->GetPDGEncoding();
       if(pdgcode == 1000010030) // He3(n,p)H3, tritium detected
         {
           mHit = 1;
	 }

    }
    */
   
   //info->Print();

   if(info->GetDetectorID() != "" || stopInDet) // if particle crossed or interacted in the detector
       fEventAction->StoreParticleInfo(*info); 
}
