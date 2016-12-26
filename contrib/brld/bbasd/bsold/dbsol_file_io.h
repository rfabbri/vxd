// This is dbsol/dbsol_file_io.h
#ifndef dbsol_file_io_h
#define dbsol_file_io_h
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma interface
#endif

//:
// \file
// \brief File I/O for vsol_spatial_object_2d
//
// \author Nhon Trinh (ntrinh@lems.brown.edu)
// \date May 31, 2006
//
// \verbatim
//  Modifications
//   05/31/2006 Nhon Trinh  Initial version.
//   03/08/2008 Nhon Trinh  Added dbsol_save_ps_file function
//   09/16/2009 Ricardo Fabbri Added support for loading gzipped cem files
//   09/29/2009 Ricardo Fabbri Added support for savinng gzipped cem files
// \endverbatim

#include <vcl_vector.h>
#include <vcl_string.h>
#include <vsol/vsol_spatial_object_2d_sptr.h>
#include <vsol/vsol_point_2d_sptr.h>
#include <vsol/vsol_polygon_2d_sptr.h>
#include <vsol/vsol_polyline_2d_sptr.h>

#include <vil/vil_rgb.h>
#include <vil/vil_image_resource_sptr.h>


//: Load a .CON file and save results to a vector of points and whether the 
// contour is closed. Return false if loading fails
bool dbsol_load_con_file(char const* filename, 
                         vcl_vector<vsol_point_2d_sptr >& points, 
                         bool & is_closed);

//: Load a .CON file and return a smart pointer to vsol_spatial_object_2d
// This can be either a polyline or a polygon. Use cast_to_...() function to
// get the exact type
vsol_spatial_object_2d_sptr dbsol_load_con_file(char const* filename);

//: Save a polyline to a .CON file. Return false if saving fails
bool dbsol_save_con_file(char const* filename, vsol_polyline_2d_sptr polyline); 

//: Save a polygon to a .CON file. Return false if saving fails
bool dbsol_save_con_file(char const* filename, vsol_polygon_2d_sptr polygon);

//: Save a set of ordered points to a .CON file. Return false if saving fails
bool dbsol_save_con_file(char const* filename, 
                         const vcl_vector<vsol_point_2d_sptr >& pts,
                         bool is_closed);

//: Save .CEM file
bool dbsol_save_cem(vcl_vector< vsol_spatial_object_2d_sptr >& vsol_list, vcl_string filename);

//: Load .CEM file. Compressed files are supported, in the zlib/gzip format with
// extension .gz, but for that you must have Boost installed.
bool dbsol_load_cem(vcl_vector< vsol_spatial_object_2d_sptr >& contours, vcl_string filename);

//: Write an image and a vsol 2D objects to a ps file
// TODO: we currently only handle polygons. Need to handle other types
bool dbsol_save_ps_file(const vcl_string& filename,
                        const vil_image_resource_sptr& img, 
                        const vcl_vector<vsol_spatial_object_2d_sptr >& vsol_data,
                        const vcl_vector<vil_rgb<float > >& colors,
                        float line_width = 2.0f,
                        float point_size = 3.0f);

#endif // dbsol_file_io_h
