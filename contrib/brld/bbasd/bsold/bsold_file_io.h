// This is bsold/bsold_file_io.h
#ifndef bsold_file_io_h
#define bsold_file_io_h
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
//   09/16/2009 Ricardo Fabbri Added support for loading gzipped cem files
//   09/29/2009 Ricardo Fabbri Added support for savinng gzipped cem files
// \endverbatim

#include <vcl_vector.h>
#include <vcl_string.h>
#include <vsol/vsol_spatial_object_2d_sptr.h>
#include <vsol/vsol_point_2d_sptr.h>
#include <vsol/vsol_polygon_2d_sptr.h>
#include <vsol/vsol_polyline_2d_sptr.h>


//: Load a .CON file and save results to a vector of points and whether the 
// contour is closed. Return false if loading fails
bool bsold_load_con_file(char const* filename, 
                         vcl_vector<vsol_point_2d_sptr >& points, 
                         bool & is_closed);

//: Load a .CON file and return a smart pointer to vsol_spatial_object_2d
// This can be either a polyline or a polygon. Use cast_to_...() function to
// get the exact type
vsol_spatial_object_2d_sptr bsold_load_con_file(char const* filename);

//: Save a polyline to a .CON file. Return false if saving fails
bool bsold_save_con_file(char const* filename, vsol_polyline_2d_sptr polyline); 

//: Save a polygon to a .CON file. Return false if saving fails
bool bsold_save_con_file(char const* filename, vsol_polygon_2d_sptr polygon);

//: Save a set of ordered points to a .CON file. Return false if saving fails
bool bsold_save_con_file(char const* filename, 
                         const vcl_vector<vsol_point_2d_sptr >& pts,
                         bool is_closed);

//: Save .CEM file
bool bsold_save_cem(vcl_vector< vsol_spatial_object_2d_sptr >& vsol_list, vcl_string filename);

//: Load .CEM file. Compressed files are supported, in the zlib/gzip format with
// extension .gz, but for that you must have Boost installed.
bool bsold_load_cem(vcl_vector< vsol_spatial_object_2d_sptr >& contours, vcl_string filename);

#endif // bsold_file_io_h
