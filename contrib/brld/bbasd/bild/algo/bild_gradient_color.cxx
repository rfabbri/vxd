// This is basic/dbgl/algo/dbil_gradient_color.cxx
//:
// \file

#include "dbil_gradient_color.h"

#include <vil/algo/vil_sobel_1x3.h>
#include <vil/vil_plane.h>
#include <vil/vil_math.h>


// -----------------------------------------------------------------------------
//: Compute the gradient of a color image by taking the strongest component among three planes
bool dbil_gradient_rgb_Linf(const vil_image_view<double >& image_src, 
                          vil_image_view<double >& Gx,
                          vil_image_view<double >& Gy)
{
  vil_image_view<double > src = image_src;

  // force the image to be 3-plane if it isn't
  if (src.nplanes() != 3)
  {
    src = vil_image_view<double >(image_src.ni(), image_src.nj(), 3);
    for (unsigned i =0; i < image_src.ni(); ++i)
    {
      for (unsigned j =0; j < image_src.nj(); ++j)
      {
        for (unsigned p =0; p < 3; ++p)
        {
          src(i, j, p) = image_src(i, j);
        }
      }
    }
  }

  // compute the gradient in each plane
  vil_image_view<double > Gp[3];
  vil_sobel_1x3(vil_plane(src, 0), Gp[0]);
  vil_sobel_1x3(vil_plane(src, 1), Gp[1]);
  vil_sobel_1x3(vil_plane(src, 2), Gp[2]);

  vil_image_view<double > Gp_mag2[3];
  vil_math_sum_sqr(Gp[0], Gp_mag2[0]);
  vil_math_sum_sqr(Gp[1], Gp_mag2[1]);
  vil_math_sum_sqr(Gp[2], Gp_mag2[2]);

  // now find the largest component in the 3 planes
  unsigned ni = src.ni();
  unsigned nj = src.nj();
  vil_image_view<vxl_byte > Gp_index(ni, nj);
  for (unsigned i =0; i < ni; ++i)
  {
    for (unsigned j = 0; j < nj; ++j)
    {
      double max_val = 0;
      vxl_byte max_index = 0;
      for (vxl_byte k =0; k < 3; ++k)
      {
        max_index = (Gp_mag2[k](i, j) > max_val) ? k : max_index;
        max_val = (Gp_mag2[k](i, j) > max_val) ? Gp_mag2[k](i, j) : max_val;
      }
      Gp_index(i, j) = max_index;
    }
  }

  //
  Gx.set_size(ni, nj);
  Gy.set_size(ni, nj);
  for (unsigned i =0; i < ni; ++i)
  {
    for (unsigned j =0; j < nj; ++j)
    {
      Gx(i, j) = Gp[Gp_index(i, j)](i, j, 0);
      Gy(i, j) = Gp[Gp_index(i, j)](i, j, 1);
    }
  }

  return true;
}

