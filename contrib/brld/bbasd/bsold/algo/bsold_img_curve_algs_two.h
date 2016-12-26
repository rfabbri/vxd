// This is bbasd/bsold/algo/bsold_img_curve_algs_two.h
#ifndef bsold_img_curve_algs_two_h_
#define bsold_img_curve_algs_two_h_
//:
// \file
// \brief Functions to that works on curves and images
// \author Based on original code by  Ozge C. Ozcanli
//
// \verbatim
//  Modifications
//   O.C.Ozcanli  02/18/2007   added curve pruner that uses image contrast around the curve 
//
// \endverbatim 

#include <bsold/bsold_interp_curve_2d_sptr.h>
#include <vil/vil_image_view.h>
#include <vbl/vbl_array_2d.h>

//function that gives the color difference around a curve
vbl_array_2d<double> get_changed_color_distance_of_curve_regions(bsold_interp_curve_2d_sptr curve, float region_width,
                                           vil_image_view<float>& L, 
                                           vil_image_view<float>& A, 
                                           vil_image_view<float>& B, 
                                           double color_gamma);

//function that gives the intensity difference around a curve
double get_changed_intensity_distance_of_curve_regions(bsold_interp_curve_2d_sptr curve, float region_width, 
                                               vil_image_view<vxl_byte>& img, double intensity_gamma);


#endif // bsold_img_curve_algs_two_h_

