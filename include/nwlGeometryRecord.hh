#ifndef nwlGeometryRecord_hh
#define nwlGeometryRecord_hh

#include <string>
#include <vector>

struct nwlGeometryRecord
{
  std::string Name;
  std::string exVolume;
  std::string Material;
  std::string Type;
  std::vector<double> Parameters;
  std::vector<double> Position;
  std::vector<double> Rotation;
  double Weight;
};

#endif
