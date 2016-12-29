// This is brcv/mvg/becld/becld_episeg_point.cxx
//:
// \file

#include "becld_episeg_point.h"
#include "becld_episeg.h"
#include <vsol/vsol_digital_curve_2d.h>

//: Construct a point
becld_episeg_point::becld_episeg_point(becld_episeg_sptr seg, double i)  {
  set_point(seg,i);
}

//: Conver to a pair
vcl_pair<becld_episeg_sptr,double> becld_episeg_point::to_pair()  const {
  return vcl_make_pair(_episeg,_index);
}

//: Set the point
void becld_episeg_point::set_point(becld_episeg_sptr& seg, double i)  {
  _episeg = seg;
  _index = i;
}

  
//: Return the point in xy coordinates
vgl_point_2d<double> 
becld_episeg_point::pt() const 
{ 
  return _episeg->curve()->interp(_index); 
} 


//: Get the episeg
becld_episeg_sptr becld_episeg_point::episeg()  const {  return _episeg; }
//: Get the index on the curve
double becld_episeg_point::index()  const            {  return _index; }
