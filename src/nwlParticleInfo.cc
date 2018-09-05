#include "nwlParticleInfo.hh"
#include "G4VPhysicalVolume.hh"

nwlParticleInfo::nwlParticleInfo()
{
	trackID = 0;
        pdg = 0;
        originTime = 0;
        originKineticEnergy = 0;
        creatorProcess = "";
        originNucleusA = 0;
        originNucleusZ = 0;
        detectorId = -1;
        detectorTime = 0;
        detectorKineticEnergy = 0;
        stopInTheDetector = false;
        stopInDetectorID = -1;
        reactionInTheDetector = "";
        weight = 0;
}

nwlParticleInfo::~nwlParticleInfo()
{}

void nwlParticleInfo::Print() const
{
  G4cout << "******" << G4endl
         << "TrackID =    " << trackID << G4endl 
	 << "PDG          " << pdg  << G4endl
         << "OriginXYZ    " << originPoint << G4endl
	 << "OriginTime   " << originTime << G4endl
	 << "OriginKine   " << originKineticEnergy << G4endl
	 << "OriginVolu   " << originVolume->GetName() << G4endl
	 << "OriginReac   " << creatorProcess << G4endl
	 << "     A   Z   " << originNucleusA << " " << originNucleusZ << G4endl
	 << "DetectorID   " << detectorId << G4endl
	 << "DetectorTime " << detectorTime << G4endl
  	 << "DetectorKine " << detectorKineticEnergy  << G4endl
         << "EntranceXYZ  " << entrancePoint << G4endl
	 << "EntranceDir  " << entranceDirection << G4endl
	 << "StopInDet    " << stopInTheDetector << " " << stopInDetectorID   << G4endl
	 << "      reac   " << reactionInTheDetector  << G4endl
	 << "Weight       " << weight << G4endl;
}

void nwlParticleInfo::SetOriginInfo(G4int TrackID, G4int Pdg, G4double Kine, G4double Time, G4ThreeVector OriginPoint, G4VPhysicalVolume* vol, G4String CreatorProcess, G4int nA, G4int nZ)
{
	trackID = TrackID;
	pdg = Pdg;
	originKineticEnergy = Kine;
	originTime = Time;
	originPoint = OriginPoint;
	originVolume = vol;
	creatorProcess = CreatorProcess;
	originNucleusA = nA;
	originNucleusZ = nZ;
}

void nwlParticleInfo::SetDetectorInfo(G4int DetId, G4double Time, G4double Kine, G4ThreeVector EntrancePoint, G4ThreeVector EntranceDir, G4double W)
{
  detectorId = DetId;
  detectorTime = Time;
  detectorKineticEnergy = Kine;
  entrancePoint = EntrancePoint;
  entranceDirection = EntranceDir;
  weight = W;
}

void nwlParticleInfo::SetFinalInfo(G4bool StopInDet, G4int detId, G4String DetProcess)
{
   stopInTheDetector = StopInDet;
   stopInDetectorID = detId;
   reactionInTheDetector = DetProcess;
}


