#ifndef nwlParticleInfo_hh
#define nwlParticleInfo_hh

#include "G4VUserTrackInformation.hh"
#include "G4ThreeVector.hh"

class  nwlParticleInfo : public G4VUserTrackInformation
{
    public:
        nwlParticleInfo();

    public:
        void Print() const;

};

#endif
