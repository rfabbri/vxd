// This is brcv/seg/sdetd/algo/sdetd_cvlet_map_io.h
#ifndef sdetd_cvlet_map_io_h
#define sdetd_cvlet_map_io_h
//:
//\file
//\brief Utility function for loading and saving a contourlet map
//\author Based on original code by  Amir Tamrakar
//\date 03/15/08
//
//\verbatim
//Modifications
//
// \endverbatim

#include <vcl_fstream.h>
#include <vcl_string.h>
#include <sdet/sdet_edgemap.h>
#include <sdet/sdet_edgemap_sptr.h>
#include <sdet/sdet_curvelet_map.h>

//: Loads an ascii file containing a curvelet map as well as the edgemap on which it is defined.
//\author Based on original code by  Amir Tamrakar
bool sdetd_load_cvlet_map(vcl_string filename, sdet_edgemap_sptr &edge_map, sdet_curvelet_map &cvlet_map);

//: Save a curvelet map as a .cvlet file
//\author Based on original code by  Amir Tamrakar
bool sdetd_save_cvlet_map(vcl_string filename, sdet_curvelet_map &cvlet_map);

#endif // sdetd_cvlet_map_io_h

