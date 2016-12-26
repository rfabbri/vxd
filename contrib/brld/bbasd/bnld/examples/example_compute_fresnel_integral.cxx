//---------------------------------------------------------------------
// This is basic/dbnl/examples/example_compute_fresnel_integral.cxx
//:
// \file
// \brief
//
// \author
//  Nhon Trinh
// \date 2/3/2005
// \verbatim
//  Modifications
//   <none>
// \endverbatim
//
//-------------------------------------------------------------------------

#include <dbnl/dbnl_fresnel.h>
#include <vcl_iostream.h>

int main( int argc, char* argv[] )
{
  double x = 0;
  vcl_cout << "A simple program to compute fresnel integral" << vcl_endl;
  vcl_cout << "Enter x = ";
  vcl_cin >> x;
  double xc, xs;
  dbnl_fresnel_integral(x, &xc, &xs);
  vcl_cout << "x = " << x << " \t fresnel_cos(x) = " << xc 
      << " \t fresnel_sin(x) = " << xs << vcl_endl;
  return 0;
}

