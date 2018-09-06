#include "nwlParticleSource.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4PhysicalVolumeStore.hh"

#include "nwlConfigParser.hh"

using namespace CLHEP;

const int CountK= 1 ;
const int CountTh = 9;
const int CountU = 9;
const int CountPuBe = 11;

double K_mas [2][CountK] = {{1},{1.46}};
double Th_mas [2][CountTh] = {{0.25,0.32,0.37,0.53,0.56,0.68,0.79,0.81,1.00},{0.239,0.337,0.511,0.583,0.727,0.91,0.97,1.593,2.614}};
double U_mas [2][CountU]   = {{0.12,0.35,0.65,0.68,0.79,0.83,0.86,0.97,1.00},{0.30,0.35,0.61,0.77,1.12,1.24,1.38,1.76,2.20}};
double PuBe_spectrum [2][CountPuBe] = {{0.05,0.11,0.19,0.34,0.53,0.66,0.75,0.86,0.94,0.98,1.00},{0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,10.5}};

//Energy of PuBe source
double PuBe(void)
{
  double E=0;
  double x = G4UniformRand();
  for (int i=0;i<CountPuBe;i++)
    if (x<=PuBe_spectrum[0][i]) {
      E =  PuBe_spectrum[1][i]; 
      break;}
  return(E);
}

nwlParticleSource* nwlParticleSource::fgInstance = 0;


nwlParticleSource* nwlParticleSource::Instance()
{
  return fgInstance;
}      


nwlParticleSource::nwlParticleSource() : G4VUserPrimaryGeneratorAction(), fParticleGun(0)
{
    fParticleGun  = new G4ParticleGun(1);

    nwlConfigParser* cfg = nwlConfigParser::Instance();

    G4String particleName = "";
    G4String logType = cfg->GetLoggingType();
    if(logType == "NeutronNeutron")
        particleName = "neutron";
    else if (logType == "NeutronGamma")
        particleName = "neutron";
    else if (logType == "GammaGamma")
        particleName = "gamma";
    else
        G4Exception("nwlGeoModel", "ConfigFile", FatalException, ("LoggingType "+logType+" not supported yet.").c_str());

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);

    if(particle)
      fParticleGun->SetParticleDefinition(particle);
    else
      G4Exception("nwlParticleSource","Unknown particle",FatalException,("Particle "+particleName+" not found").c_str());

     nwlSourceRecord srcrecord = cfg->GetSource();

     fParticleGun->SetParticlePosition(G4ThreeVector(srcrecord.Position[0]*mm, srcrecord.Position[1]*mm, srcrecord.Position[2]*mm));
     fParticleGun->SetParticleEnergy(srcrecord.Energy[0]*MeV);
     fParticleGun->SetParticleMomentumDirection(G4ThreeVector(srcrecord.Pencil[0], srcrecord.Pencil[1], srcrecord.Pencil[2]));

 /* os <<"\nSource.Type: ["<< Source.Type << "]"<<endl;
  os <<"Source.Energy: ";
  for(int i=0; i < Source.Energy.size(); i++){ os << Source.Energy[i] << ", ";  }
  os <<"\nSource.Intensity: ";
  for(int i=0; i < Source.Intensity.size(); i++){ os << Source.Intensity[i] << ", ";  }
  os <<"\nSource.Direction: ["<< Source.Direction << "]"<<endl;
  os <<"Source.Pencil: ";
  for(int i=0; i < Source.Pencil.size(); i++){ os << Source.Pencil[i] << ", ";  }
  os <<"\nSource.Particle: ["<< Source.Particle << "]"<<endl;
  os <<"Source.Position: ";
  for(int i=0; i < Source.Position.size(); i++){ os << Source.Position[i] << ", ";  }
*/



    fgInstance = this;
}


nwlParticleSource::~nwlParticleSource()
{
  delete fParticleGun;
  fgInstance = 0;
}


void nwlParticleSource::GeneratePrimaries(G4Event* anEvent)
{
/*
  G4ThreeVector trans;
  G4VPhysicalVolume* vol = G4PhysicalVolumeStore::GetInstance()->GetVolume("Source");
  if(vol != 0)
  {
    trans = vol->GetObjectTranslation();
    G4cout << "Source Translation " << trans << G4endl;

//G4AffineTransform tf(fHistory.GetTopTransform().Inverse());
//  return tf.NetTranslation();
  }
  else
  {
    G4cout << "No Source is found" << G4endl;
  }
*/
/*  double shifty1 = 0 * mm;
  if (geom >3 ) {
        shifty1 = 9.5 * mm;
  }

  G4ThreeVector pos(0., shifty1, 499*mm);

//  G4cout << "Source Translation " << pos << G4endl;


  G4double energy = 0.;
  if(type=="mono14")
    {
      energy = 14*MeV;
//      fParticleGun->SetParticlePosition(G4ThreeVector(0,0,309*mm));
      fParticleGun->SetParticlePosition(pos);
    }
  else if(type=="PuBe")
    {
      energy = PuBe()*MeV;
//      fParticleGun->SetParticlePosition(G4ThreeVector(0,69.9*mm,0*mm));	
      fParticleGun->SetParticlePosition(pos);
    }
  
  fParticleGun->SetParticleEnergy(energy);
  //RootWriter::GetPointer()->h1[0]->Fill(energy);


  G4double ctheta = G4UniformRand()*2-1; // 4pi
//  G4double ctheta = G4UniformRand()-1; // back hemisphere
  G4double stheta = sqrt(1-ctheta*ctheta);
  G4double phi = 2*pi*G4UniformRand();
  G4double cphi = cos(phi);
  G4double sphi = sin(phi);

  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(stheta*cphi, stheta*sphi, ctheta));
  */
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

