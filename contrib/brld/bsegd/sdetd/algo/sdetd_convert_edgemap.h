// This is brcv/seg/sdet/algo/sdet_edgemap_to_image.h
#ifndef sdet_edgemap_to_image_h
#define sdet_edgemap_to_image_h
//:
//\file
//\brief Convert an edgemap (sets of edgels or linked edgels) to a (binary) image
//\author Based on original code by  Nhon Trinh (@lems.brown.edu)
//\date May 29, 2008
//
//\verbatim
//  Modifications
//  firat: added support for conversion to a grayscale image
//\endverbatim

#include <vil/vil_image_view.h>

#include <sdet/sdet_edgemap.h>
#include <sdet/sdet_edgemap_sptr.h>
#include <sdet/sdet_curve_fragment_graph.h>

#include <vsol/vsol_spatial_object_2d_sptr.h>
#include <vgl/vgl_box_2d.h>

//: Function to convert an edge map (floating elements) to a BW image
// "roi_box" is the region of the edge map to convert.
bool sdetd_convert_edgemap_to_image(const sdet_curve_fragment_graph& CFG, 
                                    vil_image_view<vxl_byte >& bw_image,
                                    const vgl_box_2d<int >& roi_box);

//: Function to convert a subpixel edge map  to a BW image. 
// "roi_box" is the region of the edge map to convert.
bool sdetd_convert_edgemap_to_image(const sdet_edgemap& edgels, 
                                    vil_image_view<vxl_byte >& bw_image,
                                    const vgl_box_2d<int >& roi_box);

//: Convenience function that sets the roi_box to the dimensions of the edgemap
// buckets. This assumes the edgemap has one bucket per pixel.
bool sdetd_convert_edgemap_to_image(const sdet_edgemap& edgemap, 
                                    vil_image_view<vxl_byte >& bw_image);

//: Function to convert an edge map (floating elements) to a gray scale image
// "roi_box" is the region of the edge map to convert.
bool sdetd_convert_edgemap_to_grayscale_image(const sdet_curve_fragment_graph& CFG,
                                    vil_image_view<vxl_byte >& bw_image,
                                    const vgl_box_2d<int >& roi_box);

//: Function to convert a subpixel edge map  to a grayscale image.
// "roi_box" is the region of the edge map to convert.
bool sdetd_convert_edgemap_to_grayscale_image(const sdet_edgemap& edgels,
                                    vil_image_view<vxl_byte >& bw_image,
                                    const vgl_box_2d<int >& roi_box);

//: Convenience function that sets the roi_box to the dimensions of the edgemap
// buckets. This assumes the edgemap has one bucket per pixel.
bool sdetd_convert_edgemap_to_grayscale_image(const sdet_edgemap& edgemap,
                                    vil_image_view<vxl_byte >& bw_image);

#endif // sdet_edgemap_to_image_h
