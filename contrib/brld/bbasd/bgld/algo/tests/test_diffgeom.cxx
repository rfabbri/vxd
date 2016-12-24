// This is basic/dbgl/algo/tests/test_diffgeom.cxx
#include <testlib/testlib_test.h>
#include <vcl_iostream.h>
#include <dbgl/algo/dbgl_diffgeom.h>
#include <vnl/vnl_math.h>

//: Test differential geometry estimation functions
MAIN( test_diffgeom )
{
   vcl_vector< vgl_point_2d<double> > vertices;
   const double tolerance = 1e-8;
   //Simple test for cos(x) points
   vertices.push_back(vgl_point_2d<double>(0.000000000000000,   1.000000000000000));
   vertices.push_back(vgl_point_2d<double>(0.628318530717959,   0.809016994374947));
   vertices.push_back(vgl_point_2d<double>(1.256637061435917,   0.309016994374947));
   vertices.push_back(vgl_point_2d<double>(1.884955592153876,  -0.309016994374947));
   vertices.push_back(vgl_point_2d<double>(2.513274122871834,  -0.809016994374947));
   vertices.push_back(vgl_point_2d<double>(3.141592653589793,  -1.000000000000000));
   
  double refCurvature[] = { -0.481276166568294,
                            -0.532001598826330,
                            -0.124950156695558,
                             0.124950156695557,
                             0.532001598826329,
                             0.481276166568294 };

  vnl_vector<double> curvature;

  START("dbgl curvature estimation");

  dbgl_compute_curvature(vertices, &curvature);

  bool status = true;

  for(int i = 0; i < vertices.size(); ++i)
  {
     status &= (vnl_math::abs(curvature[i] - refCurvature[i]) < tolerance);
  }

  TEST("Test compute curvature (tol=1e-8)", status, true);

  double refNormal[] = { -0.290821022430010,  -0.956777473037887,
                         -0.447978631217491,  -0.894044263989487,
                         -0.661043677792052,  -0.750347423565349,
                         -0.661043677792052,  -0.750347423565349,
                         -0.447978631217491,  -0.894044263989487,
                         -0.290821022430010,  -0.956777473037887 };

  vcl_vector< vnl_vector_fixed<double, 2> > normal;
  dbgl_compute_normals(vertices, &normal);

  status = true;
  for(int i = 0; i < vertices.size(); ++i)
  {
     status &= ( (vnl_math::abs(normal[i][0] - refNormal[2*i]) < tolerance) &&
                 (vnl_math::abs(normal[i][1] - refNormal[2*i+1]) < tolerance) );
  }

  TEST("Test compute normal (tol=1e-8)", status, true);

  SUMMARY();
}
