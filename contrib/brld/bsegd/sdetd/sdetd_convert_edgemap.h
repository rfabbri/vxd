// This is brcv/seg/dbdet/algo/dbdet_edgemap_to_image.h
#ifndef dbdet_edgemap_to_image_h
#define dbdet_edgemap_to_image_h
//:
//\file
//\brief Convert an edgemap (sets of edgels or linked edgels) to a (binary) image
//\author Nhon Trinh (ntrinh@lems.brown.edu)
//\date May 29, 2008
//
//\verbatim
//  Modifications
//  firat: added support for conversion to a grayscale image
//\endverbatim

#include <vil/vil_image_view.h>

#include <dbdet/edge/dbdet_edgemap.h>
#include <dbdet/edge/dbdet_edgemap_sptr.h>
#include <dbdet/sel/dbdet_curve_fragment_graph.h>

#include <vsol/vsol_spatial_object_2d_sptr.h>
#include <vgl/vgl_box_2d.h>

//: Function to convert an edge map (floating elements) to a BW image
// "roi_box" is the region of the edge map to convert.
bool dbdet_convert_edgemap_to_image(const dbdet_curve_fragment_graph& CFG, 
                                    vil_image_view<vxl_byte >& bw_image,
                                    const vgl_box_2d<int >& roi_box);

//: Function to convert a subpixel edge map  to a BW image. 
// "roi_box" is the region of the edge map to convert.
bool dbdet_convert_edgemap_to_image(const dbdet_edgemap& edgels, 
                                    vil_image_view<vxl_byte >& bw_image,
                                    const vgl_box_2d<int >& roi_box);

//: Convenience function that sets the roi_box to the dimensions of the edgemap
// buckets. This assumes the edgemap has one bucket per pixel.
bool dbdet_convert_edgemap_to_image(const dbdet_edgemap& edgemap, 
                                    vil_image_view<vxl_byte >& bw_image);

//: Function to convert an edge map (floating elements) to a gray scale image
// "roi_box" is the region of the edge map to convert.
bool dbdet_convert_edgemap_to_grayscale_image(const dbdet_curve_fragment_graph& CFG,
                                    vil_image_view<vxl_byte >& bw_image,
                                    const vgl_box_2d<int >& roi_box);

//: Function to convert a subpixel edge map  to a grayscale image.
// "roi_box" is the region of the edge map to convert.
bool dbdet_convert_edgemap_to_grayscale_image(const dbdet_edgemap& edgels,
                                    vil_image_view<vxl_byte >& bw_image,
                                    const vgl_box_2d<int >& roi_box);

//: Convenience function that sets the roi_box to the dimensions of the edgemap
// buckets. This assumes the edgemap has one bucket per pixel.
bool dbdet_convert_edgemap_to_grayscale_image(const dbdet_edgemap& edgemap,
                                    vil_image_view<vxl_byte >& bw_image);

#endif // dbdet_edgemap_to_image_h
