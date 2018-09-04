#ifndef H2Record_hh
#define H2Record_hh

#include<string>

struct nwlH2Record
{
  std::string PhysQ_x;
  int Nbins_x;
  double Xmin;
  double Xmax;
  std::string PhysQ_y;
  int Nbins_y;
  double Ymin;
  double Ymax;
};

#endif