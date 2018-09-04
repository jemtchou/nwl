#ifndef nwlMaterialRecord_hh
#define nwlMaterialRecord_hh

#include <string>
#include <vector>

struct nwlMaterialRecord
{
  std::string Name;
  double Density;
  std::vector<std::string> Element;
  std::vector<double> Fraction; 
};

#endif