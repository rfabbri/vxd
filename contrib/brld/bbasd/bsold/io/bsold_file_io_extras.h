// This is bsold/io/bsold_file_io_extras.h
#ifndef bsold_file_io_extras_h
#define bsold_file_io_extras_h
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma interface
#endif

//:
// \file
// \brief File I/O for vsol_spatial_object_2d
//
// \author Based on original code by  Nhon Trinh (@lems.brown.edu)
// \date May 31, 2006
//
// \verbatim
//  Modifications
//   05/31/2006 Nhon Trinh  Initial version.
//   03/08/2008 Nhon Trinh  Added bsold_save_ps_file function
// \endverbatim

#include <vcl_vector.h>
#include <vcl_string.h>
#include <vsol/vsol_spatial_object_2d_sptr.h>
#include <vil/vil_rgb.h>
#include <vil/vil_image_resource_sptr.h>


//: Write an image and a vsol 2D objects to a ps file
// TODO: we currently only handle polygons. Need to handle other types
bool bsold_save_ps_file(const vcl_string& filename,
                        const vil_image_resource_sptr& img, 
                        const vcl_vector<vsol_spatial_object_2d_sptr >& vsol_data,
                        const vcl_vector<vil_rgb<float > >& colors,
                        float line_width = 2.0f,
                        float point_size = 3.0f);

#endif // bsold_file_io_extras_h
