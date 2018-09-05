#ifndef nwlParticleInfo_hh
#define nwlParticleInfo_hh

#include "G4VUserTrackInformation.hh"
#include "G4ThreeVector.hh"

class G4VPhysicalVolume;

class  nwlParticleInfo : public G4VUserTrackInformation
{
    public:
        nwlParticleInfo();
	~nwlParticleInfo();

        void Print() const;

	void SetOriginInfo(G4int TrackID, G4int Pdg, G4double Kine, G4double Time, G4ThreeVector OriginPoint, G4VPhysicalVolume* vol, G4String CreatorProcess, G4int nA, G4int nZ);

	void SetDetectorInfo(G4int DetId, G4double Time, G4double Kine, G4ThreeVector EntrancePoint, G4ThreeVector EntranceDir, G4double W);

	void SetFinalInfo(G4bool StopInDet, G4int detId, G4String DetProcess);

    private:
	G4int trackID;
	G4int pdg;
	G4ThreeVector originPoint;
	G4double originTime;
	G4double originKineticEnergy;
	G4VPhysicalVolume* originVolume;
	G4String creatorProcess;
	G4int originNucleusA;
	G4int originNucleusZ;
	G4int detectorId;
	G4ThreeVector entrancePoint;
	G4ThreeVector entranceDirection;
	G4double detectorTime;
	G4double detectorKineticEnergy;
	G4bool stopInTheDetector;
	G4int stopInDetectorID;
	G4String reactionInTheDetector;
	G4double weight;
};

#endif
