#ifndef nwlGeoModel_h
#define nwlGeoModel_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include <map>

class G4GeometrySampler;
class G4Element;
class G4Material;
class G4IStore;
class G4VImportanceAlgorithm;

class nwlGeoModel : public G4VUserDetectorConstruction
{
public:
  nwlGeoModel();
  ~nwlGeoModel();

  G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

  void ConfigureImportanceSampling();
 
  bool DoImportanceSampling();
  
  G4GeometrySampler* GetSampler();

protected:
  G4IStore* istore;
  G4VImportanceAlgorithm* ialg;
  G4GeometrySampler* sampler;
 
  G4VPhysicalVolume* pWorld;
  bool doImportanceSampling;

  std::map<G4String, G4VPhysicalVolume*> vol;
  std::map<G4String, G4LogicalVolume*> logvol;
  std::map<G4String, G4double> weight;
};

#endif
