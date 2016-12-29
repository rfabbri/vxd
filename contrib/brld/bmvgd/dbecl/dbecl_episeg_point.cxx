// This is brcv/mvg/dbecl/dbecl_episeg_point.cxx
//:
// \file

#include "dbecl_episeg_point.h"
#include "dbecl_episeg.h"
#include <vsol/vsol_digital_curve_2d.h>

//: Construct a point
dbecl_episeg_point::dbecl_episeg_point(dbecl_episeg_sptr seg, double i)  {
  set_point(seg,i);
}

//: Conver to a pair
vcl_pair<dbecl_episeg_sptr,double> dbecl_episeg_point::to_pair()  const {
  return vcl_make_pair(_episeg,_index);
}

//: Set the point
void dbecl_episeg_point::set_point(dbecl_episeg_sptr& seg, double i)  {
  _episeg = seg;
  _index = i;
}

  
//: Return the point in xy coordinates
vgl_point_2d<double> 
dbecl_episeg_point::pt() const 
{ 
  return _episeg->curve()->interp(_index); 
} 


//: Get the episeg
dbecl_episeg_sptr dbecl_episeg_point::episeg()  const {  return _episeg; }
//: Get the index on the curve
double dbecl_episeg_point::index()  const            {  return _index; }
