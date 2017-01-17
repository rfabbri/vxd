// This is bmvgd/becld/becld_episeg_point.h
#ifndef becld_episeg_point_h_
#define becld_episeg_point_h_

//:
// \file
// \brief  A point a long an episegment.
// \author Based on original code by  dapachec
// \date   8/18/2004

#include <vbl/vbl_ref_count.h>
#include "becld_episeg_sptr.h"
#include <vcl_utility.h>
#include <vgl/vgl_point_2d.h>

//: A point along an episegment.
class becld_episeg_point : public vbl_ref_count {
public:
  //: Construct a point
  becld_episeg_point(becld_episeg_sptr = NULL, double = 0.0);
  //: Convert to pair
  vcl_pair<becld_episeg_sptr,double> to_pair() const;
  //: Set the point
  void set_point(becld_episeg_sptr&, double);
  //: Get the curve
  becld_episeg_sptr episeg() const;
  //: Get the index
  double index() const;

  //: Return the point in xy coordinates
  vgl_point_2d<double> pt() const; 
  
protected:
  //: The curve
  becld_episeg_sptr _episeg;
  //: The index
  double _index;
};

#endif // becld_episeg_point_h_
