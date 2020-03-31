#ifndef nwlConfigParser_hh
#define nwlConfigParser_hh

#include "nwlSourceRecord.hh"
#include "nwlMaterialRecord.hh"
#include "nwlGeometryRecord.hh"
#include "nwlH1Record.hh"
#include "nwlH2Record.hh"

#include <string>
#include <fstream>

class nwlConfigParser
{
public:

  static nwlConfigParser* Instance();

  void ReadConfig(std::string fn);
 
  void SetFilename(std::string fn) {fname = fn;}; 

  long GetNumberOfEvents()
  {
    return m_EventNb;
  }

  std::string GetLoggingType()
  {
    return m_LoggingType;
  }
 
  std::string GetGeneratorMacro();
 
  //  nwlSourceRecord GetSource();

  bool GetMaterial(std::vector<nwlMaterialRecord>& Materials)
  {
    Materials = m_Materials;
    return b_Materials;
  };

  bool GetGeometry(std::vector<nwlGeometryRecord>& Volumes)
  {
    Volumes = m_Volumes;
    return b_Volumes;
  };

  bool GetDetector(std::vector<std::string>& Detectors)
  {
    Detectors = m_Detectors;
    return b_Detectors;
  };

  bool WriteNtuple()
  {
    return b_WriteNtuple;
  }

  bool StoreAllParticles()
  {
    return b_StoreAll;
  }

  bool CreateH1()
  {
    return b_H1;
  }

  bool CreateH2()
  {
    return b_H2;
  }
  
  void GetH1s(std::vector<nwlH1Record>& H1Ds)
  {
    H1Ds = m_H1Ds;
  }

  void GetH2s(std::vector<nwlH2Record>& H2Ds)
  {
    H2Ds = m_H2Ds;
  }
  //bool GetOutput(std::vector<nwlH1Record>& H1Ds, std::vector<nwlH2Record>& H2Ds, bool& WriteNtuple){return true;};

protected:
  std::ifstream OpenFile(); 
  std::string ParsePair(std::string key);

  void ReadNumberOfEvents();
  void ReadLoggingType();
  void ReadGeneratorMacro();
  void ReadMaterials();
  void ReadGeometry();
  void ReadDetectors();
  void ReadOutput();
  
private:
  nwlConfigParser();
  ~nwlConfigParser();
  nwlConfigParser(const nwlConfigParser&);
  nwlConfigParser& operator= (const nwlConfigParser&);

  // сonfig file name
  std::string fname;

  // configuration
  long m_EventNb;
  std::string m_GenMacro;
  std::string m_LoggingType;
  std::vector<nwlMaterialRecord> m_Materials;
  std::vector<nwlGeometryRecord> m_Volumes;
  std::vector<std::string> m_Detectors;
  std::vector<nwlH1Record> m_H1Ds;
  std::vector<nwlH2Record> m_H2Ds;

  bool b_EventNb;
  bool b_LoggingType;
  bool b_GenMacro;
  bool b_Materials;
  bool b_Volumes;
  bool b_Detectors;
  bool b_WriteNtuple;
  bool b_StoreAll;
  bool b_H1;
  bool b_H2;

  static nwlConfigParser* pInstance; 

  std::ofstream oerr;
};

#endif
