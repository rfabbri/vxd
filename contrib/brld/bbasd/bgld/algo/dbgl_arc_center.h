// This is brcv/shp/dbsk2d/dbsk2d_geometry_utils.h

#ifndef dbgl_arc_center_h_
#define dbgl_arc_center_h_

// \file
// \brief This is  
// \author Amir Tamrakar
// \date 02/02/05
//
// 
// \verbatim
//  Modifications
//   Amir Tamrakar 02/02/2005    Initial version. Conversion to VXL standard.
// \endverbatim

#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_line_segment_2d.h>
#include <vgl/vgl_vector_2d.h>
#include <vcl_cassert.h>
#include <vcl_cmath.h>
#include <vnl/vnl_math.h>

//#include <dbsk2d/dbsk2d_utils.h>


//: get the center of the circular arc fitting to the three points
// (x1,y1)-----(x3,y3)-----(x2,y2)
inline vgl_point_2d<double> getArcCenterFromThreePoints (double x1, double y1, 
                        double x2, double y2,
                        double x3, double y3)
{
  vgl_point_2d<double> center;

  double start1x = (x1+x3)/2;
  double start1y = (y1+y3)/2;

  double start2x = (x2+x3)/2;
  double start2y = (y2+y3)/2;

  double psi1 = vcl_atan2(y3-y1,x3-x1) + vnl_math::pi_over_2;
  double psi2 = vcl_atan2(y3-y2,x3-x2) + vnl_math::pi_over_2;

  double psihat = vcl_atan2(start2y - start1y, start2x - start1x);

  if (vcl_sin(psi2 - psi1)==0){// parallel lines
    center.set(100000, 100000);
  }
  else {
    double test1 = vcl_sin(psi2 - psihat )/vcl_sin(psi2 - psi1);
    double newH = vcl_sqrt( (start1y - start2y)*(start1y - start2y) +
                  (start1x - start2x)*(start1x - start2x) );

    center.set( start1x + newH*test1*vcl_cos(psi1),
                start1y + newH*test1*vcl_sin(psi1));
  }
  return center;
}


#endif // dbgl_arc_center_h_

