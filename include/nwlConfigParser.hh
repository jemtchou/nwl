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
 
 void SetFilename(std::string fn) {fname = fn;}; 

 long GetNumberOfEvents();

 std::string GetLoggingType();

 nwlSourceRecord GetSource();

 bool GetMaterial(std::vector<nwlMaterialRecord>& Materials);

 bool GetGeometry(std::vector<nwlGeometryRecord>& Volumes);

 bool GetDetector(std::vector<std::string>& Detectors);

 bool GetOutput(std::vector<nwlH1Record>& H1Ds, std::vector<nwlH2Record>& H2Ds, bool WriteNtuple);

private:
 nwlConfigParser();
 ~nwlConfigParser();
 nwlConfigParser(const nwlConfigParser&);
 nwlConfigParser& operator= (const nwlConfigParser&);

 std::ifstream OpenFile(); 

 // сonfig file name
 std::string fname;

 static nwlConfigParser* pInstance; 

 std::ofstream oerr;
};
#endif
