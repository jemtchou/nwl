#ifndef nwlSourceRecord_hh
#define nwlSourceRecord_hh

#include <string>
#include <vector>

struct nwlSourceRecord
{
  std::string Type;
  std::vector<double> Energy;
  std::vector<double> Intensity;
  std::string Direction;
  std::vector<double> Pencil;
  std::string Particle;
  std::vector<double> Position;
};

#endif