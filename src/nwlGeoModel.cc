#include "nwlGeoModel.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Trd.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4RunManager.hh"
#include "G4GeometrySampler.hh"
#include "G4IStore.hh"
#include "G4ImportanceAlgorithm.hh"

#include "nwlConfigParser.hh"

using namespace std;


nwlGeoModel::nwlGeoModel()
{
  doImportanceSampling = false;
  vol.clear();
  weight.clear();
}

nwlGeoModel::~nwlGeoModel()
{
}

bool nwlGeoModel::DoImportanceSampling()
{
  if (doImportanceSampling)
    return doImportanceSampling;

  if(weight.size()==0)
  {
    nwlConfigParser* cfg = nwlConfigParser::Instance();
    vector<nwlGeometryRecord> vollist;
    if(cfg->GetGeometry(vollist))
    {
      for(unsigned int i = 0; i < vollist.size(); i++)
      {
        nwlGeometryRecord& vrecord = vollist[i];
        weight[vrecord.Name] = vrecord.Weight;
        if (fabs(vrecord.Weight - 1.0) > 1e-6)
	    doImportanceSampling = true;
      }
    }

    if(doImportanceSampling)
    {
      G4String particleName = "";
      G4String logType = cfg->GetLoggingType();
      if(logType == "NeutronNeutron")
	 particleName = "neutron";
      else if (logType == "NeutronGamma")
        particleName = "gamma";
      else if (logType == "GammaGamma")
	particleName = "gamma";
      else 
	G4Exception("nwlGeoModel", "ConfigFile", FatalException, ("LoggingType "+logType+" not supported yet.").c_str());

      sampler = new G4GeometrySampler(pWorld, particleName);
      ialg = new G4ImportanceAlgorithm();
    }
  }

  return doImportanceSampling;
}

//void nwlGeoModel::ConfigureSampler()
//{
//   sampler->Configure();
//}

G4GeometrySampler* nwlGeoModel::GetSampler()
{
  return sampler;
}

G4VPhysicalVolume* nwlGeoModel::Construct()
{
  // --- materials ---

  G4NistManager* nist = G4NistManager::Instance();
  nwlConfigParser* cfg = nwlConfigParser::Instance();
  //nist->SetVerbose(2);
  map<G4String, G4Material*> mat;

  G4Material* vacuum = nist->FindOrBuildMaterial("G4_Galactic");
  mat["Vacuum"] = vacuum;

  vector<nwlMaterialRecord> matlist;
  if (cfg->GetMaterial(matlist))
  {
    for(unsigned int i = 0; i < matlist.size(); i++){
      nwlMaterialRecord& mrecord = matlist[i];
      G4String name = mrecord.Name;
      // Create material from elements by fraction mass
      G4Material* tmpmat = new G4Material(name, mrecord.Density*g/cm3, mrecord.Element.size());
      for(unsigned int j = 0; j < mrecord.Element.size(); j++)
         { 
	   G4Element* tmpelm = nist->FindOrBuildElement(mrecord.Element[j]);
           if(tmpelm == 0)
           {
                G4Exception("nwlGeoModel", "ConfigFile", FatalException, ("Cannot find element \""+mrecord.Element[j]+"\"").c_str());
	   }        
 // fixme Special treatment for H in water?? H->TS_H_of_Water by default?
 // fixme He3 ? 
	   tmpmat->AddElement(tmpelm,mrecord.Fraction[j]*0.01); //percent
         }
       mat[name] = tmpmat;    
    }
  } else 
  {
    G4Exception("nwlGeoModel", "ConfigFile", FatalException, "Cannot find materials in the config file");
  }

  map<G4String, G4Material*>::iterator it;
  for(it=mat.begin(); it!=mat.end(); ++it)
  {
     std::cout << (*it).first << " " << (*(*it).second) << std::endl; 
  }

  // --- geometry --- 
  pWorld = 0;

  vector<nwlGeometryRecord> vollist;
  if(cfg->GetGeometry(vollist))
  {
    for(unsigned int i = 0; i < vollist.size(); i++)
    {
      nwlGeometryRecord& vrecord = vollist[i];

      G4VSolid* sv = 0;
      if(vrecord.Type == "Box")
      {
	    sv = new G4Box(vrecord.Name,vrecord.Parameters[0]*mm, vrecord.Parameters[1]*mm, vrecord.Parameters[2]*mm);
      }
      else if(vrecord.Type == "Tub")
      {
	    sv = new G4Tubs(vrecord.Name,vrecord.Parameters[0]*mm, vrecord.Parameters[1]*mm, vrecord.Parameters[2]*mm, vrecord.Parameters[3]*deg, vrecord.Parameters[4]*deg);
      }
      else if(vrecord.Type == "Con")
      {
	    sv = new G4Cons(vrecord.Name,vrecord.Parameters[0]*mm, vrecord.Parameters[1]*mm, vrecord.Parameters[2]*mm, vrecord.Parameters[3]*mm, vrecord.Parameters[4]*mm, vrecord.Parameters[5]*deg, vrecord.Parameters[6]*deg);
      }
      else if(vrecord.Type == "Trd")
      {
	    sv = new G4Trd(vrecord.Name,vrecord.Parameters[0]*mm, vrecord.Parameters[1]*mm, vrecord.Parameters[2]*mm, vrecord.Parameters[3]*mm, vrecord.Parameters[4]*mm);
      }
      else if(vrecord.Type == "Sphere")
      {
	    sv = new G4Sphere(vrecord.Name,vrecord.Parameters[0]*mm, vrecord.Parameters[1]*mm, vrecord.Parameters[2]*deg, vrecord.Parameters[3]*deg, vrecord.Parameters[4]*deg, vrecord.Parameters[5]*deg);
      }
      else
      {
         G4Exception("nwlGeoModel", "ConfigFile", FatalException, ("Volume type "+vrecord.Type+" not supported yet.").c_str());
      }


      G4Material* tmpmat = mat[vrecord.Material];
      if(tmpmat == 0) // Check NIST material library
         tmpmat = nist->FindOrBuildMaterial(vrecord.Material);
      if(tmpmat == 0)  
      {
         G4Exception("nwlGeoModel", "ConfigFile", FatalException, ("Material "+vrecord.Material+" not defined.").c_str());
      }

      G4LogicalVolume* lv = new G4LogicalVolume( sv, tmpmat, vrecord.Name, 0, 0, 0);

      G4VPhysicalVolume* motherv = 0;
      if (vrecord.exVolume != "") //not a world volume
      {       
           motherv = vol[vrecord.exVolume];
           if(motherv == 0)
           {
              G4Exception("nwlGeoModel", "ConfigFile", FatalException, ("Mother volume "+vrecord.exVolume+" does not exist.").c_str());
           }
      }

      G4ThreeVector pos(0, 0, 0);
      if(vrecord.Position.size() == 3) pos.set(vrecord.Position[0]*mm, vrecord.Position[1]*mm, vrecord.Position[2]*mm); 

      G4RotationMatrix* rotm = new G4RotationMatrix();

      if(vrecord.Rotation.size() == 9) 
	    rotm->set(G4Rep3x3(vrecord.Rotation[0], vrecord.Rotation[1], vrecord.Rotation[2],
                               vrecord.Rotation[3], vrecord.Rotation[4], vrecord.Rotation[5],
                               vrecord.Rotation[6], vrecord.Rotation[7], vrecord.Rotation[8]));
      G4VPhysicalVolume* pv = new G4PVPlacement(rotm, pos, vrecord.Name, lv, motherv, false, 0, true);

      std::cout << "Create volume " << vrecord.Name << " " << vrecord.exVolume << std::endl;

      if(motherv == 0) 
      {   
          if (pWorld !=0)
          {
              G4Exception("nwlGeoModel", "ConfigFile", FatalException, "Only one world volume is possible.");
          }  

          pWorld = pv;
      }
      vol[vrecord.Name] = pv;
    }
  }

// --- Sensitive detectors ---


  return pWorld;
}

void nwlGeoModel::ConfigureImportanceSampling()
{
      G4cout<<"Configure Importance sampling:"<<G4endl;

      istore = G4IStore::GetInstance();
      cout << pWorld->GetName() << endl;
      istore->AddImportanceGeometryCell(1, *pWorld);
      map<G4String, G4double>::iterator wt;
      for (wt = weight.begin(); wt != weight.end(); ++wt)
      {	    
	 if(vol[(*wt).first]==pWorld) continue;
         cout << "Volume " << (*wt).first << " ---> " << (*wt).second << endl;
         istore->AddImportanceGeometryCell((*wt).second,*(vol[(*wt).first]));
      }
      sampler->PrepareImportanceSampling(istore, ialg);
}

