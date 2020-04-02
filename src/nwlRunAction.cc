#include "nwlRunAction.hh"

#include "nwlConfigParser.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"

#include "nwlAnalysis.hh"
#include "nwlEventAction.hh"

using namespace std;

nwlRunAction::nwlRunAction() : G4UserRunAction()
{
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  //analysisManager->SetNtupleMerging(true);
  // Note: merging ntuples is available only with Root output

  nwlConfigParser* cfg = nwlConfigParser::Instance();
  if(cfg->CreateH1())
    {
      vector<nwlH1Record> H1Ds;
      cfg->GetH1s(H1Ds);

      vector<nwlH1Record>::const_iterator it;
      for (it=H1Ds.begin(); it!=H1Ds.end(); ++it)
	{
	  // Creating histograms
	  G4int id = analysisManager->CreateH1((*it).PhysQ, (*it).PhysQ, (*it).Nbins, (*it).Xmin, (*it).Xmax);
	  H1map[id] = (*it).PhysQ;
	}
    }
  
  if(cfg->CreateH2())
    {
      vector<nwlH2Record> H2Ds;
      cfg->GetH2s(H2Ds);
      
      vector<nwlH2Record>::const_iterator it;
      for (it=H2Ds.begin(); it!=H2Ds.end(); ++it)
	{
	  // Creating histograms
	  string hname = (*it).PhysQ_x+"_"+(*it).PhysQ_y;
	  G4int id = analysisManager->CreateH2(hname, hname, (*it).Nbins_x, (*it).Xmin, (*it).Xmax,
					       (*it).Nbins_y, (*it).Ymin, (*it).Ymax);
	  pair <string, string> val ((*it).PhysQ_x, (*it).PhysQ_y); 
	  H2map[id] = val;
	}
    }
  
  // Creating ntuple
  if (cfg->WriteNtuple()) 
    {
      analysisManager->CreateNtuple("NWL", "Well Logging Simu");
      analysisManager->CreateNtupleIColumn("EventID");
      analysisManager->CreateNtupleIColumn("TrackID");
      analysisManager->CreateNtupleIColumn("ParentID");
      analysisManager->CreateNtupleIColumn("PDG");
      analysisManager->CreateNtupleDColumn("OriginX");
      analysisManager->CreateNtupleDColumn("OriginY");
      analysisManager->CreateNtupleDColumn("OriginZ");
      analysisManager->CreateNtupleDColumn("Time");
      analysisManager->CreateNtupleDColumn("OriginKine");
      analysisManager->CreateNtupleSColumn("OriginVolume");
      analysisManager->CreateNtupleSColumn("CreatorProcess");
      analysisManager->CreateNtupleIColumn("NucleusA");
      analysisManager->CreateNtupleIColumn("NucleusZ");
      analysisManager->CreateNtupleSColumn("DetectorID");
      analysisManager->CreateNtupleDColumn("EntranceX");
      analysisManager->CreateNtupleDColumn("EntranceY");
      analysisManager->CreateNtupleDColumn("EntranceZ");
      analysisManager->CreateNtupleDColumn("EntranceDirX");
      analysisManager->CreateNtupleDColumn("EntranceDirY");
      analysisManager->CreateNtupleDColumn("EntranceDirZ");
      analysisManager->CreateNtupleDColumn("DetectionTime");
      analysisManager->CreateNtupleDColumn("DetectionKine");
      analysisManager->CreateNtupleSColumn("StopInDetectorID");
      analysisManager->CreateNtupleSColumn("ReactionInDetector");
      analysisManager->CreateNtupleDColumn("Weight");
      
      analysisManager->FinishNtuple();
    }
}

nwlRunAction::~nwlRunAction()
{}

bool nwlRunAction::IsSensitive(G4String dname)
{
  for(vector<G4String>::iterator it = detectorId.begin(); it!= detectorId.end(); ++it)
    if(dname == (*it)) return true;
  return false;
}

ostream& nwlRunAction::GetStream()
{
  if(fout.is_open())
    return fout;
  return cout;
}

void nwlRunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  // calculate detector ids
  detectorId.clear();
  
  nwlConfigParser* cfg = nwlConfigParser::Instance();

  G4String logType = cfg->GetLoggingType();
  G4String particleName = "";
  if(logType == "NeutronNeutron")
    particleName = "neutron";
  else if (logType == "NeutronGamma")
    particleName = "neutron";
  else if (logType == "GammaGamma")
    particleName = "gamma";
  else
    G4Exception("nwlGeoModel", "ConfigFile", FatalException, ("LoggingType "+logType+" not supported yet.").c_str());

  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/gps/particle "+particleName);

  // output file
  // vector<nwlH1Record> H1Ds; 
  // vector<nwlH2Record> H2Ds;
  // bool WriteNtuple;

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile("run_"+std::to_string(jobID)+".dat");
  /*
    if(cfg->GetOutput(H1Ds, H2Ds, WriteNtuple))
    {
    if(WriteNtuple)
    {
    fout.open(("run_"+std::to_string(jobID)+".txt"));
    }
    }
  */
  if (IsMaster()) 
    {
      // detector list
      vector<string> cfgdet;
      if( cfg->GetDetector(cfgdet) )
	{
	  G4cout << "Detector list" << G4endl;
	  for(unsigned int i = 0; i < cfgdet.size(); i++)
	    {
	      detectorId.push_back(cfgdet[i]);
	      detectorList[cfgdet[i]] = i;
	      G4cout << "ID " << i << " " << cfgdet[i] << G4endl;
	    }
	}
      
      // process table
      process["Generator"] = 0;
      int j = 1;
      G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
      int k =0;
      for(k=0; k< ptable->entries(); ++k)
	{
	  G4ParticleDefinition* particle = ptable->GetParticle(k);
	  if(! particle) continue;
	  G4ProcessManager* pmanager = particle->GetProcessManager();
	  int i;
	  for (i=0; i<pmanager->GetProcessListLength();i++){
	    j++;
	    // G4cout << "Process " << j << " " << (*(pmanager->GetProcessList()))[i]->GetProcessName() << " of " << particle->GetParticleName() << G4endl;
	    process[(*(pmanager->GetProcessList()))[i]->GetProcessName()] = j;
	  }
	}
  
      G4cout << "Process list" << G4endl;
      std::map<G4String,int>::iterator it;
      j=0;
      for(it=process.begin(); it!=process.end(); ++it)
	{
	  (*it).second=j++;
	}
      for(it=process.begin(); it!=process.end(); ++it)
	{
	  G4cout << (*it).first << " " << (*it).second << G4endl;
	}
    }
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}

G4int nwlRunAction::GetProcessID(G4String name)
{
  return process[name];
}

G4int nwlRunAction::GetDetectorID(G4String name)
{
  return detectorList[name];
}

void nwlRunAction::EndOfRunAction(const G4Run*)
{
  // if (IsMaster()) 
  //  {
      auto analysisManager = G4AnalysisManager::Instance();
      // save histograms & ntuple
      analysisManager->Write();
      analysisManager->CloseFile();
      //  };

  fout.close();
}
