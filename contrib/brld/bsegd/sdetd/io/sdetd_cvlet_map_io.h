// This is brcv/seg/dbdet/algo/dbdet_cvlet_map_io.h
#ifndef dbdet_cvlet_map_io_h
#define dbdet_cvlet_map_io_h
//:
//\file
//\brief Utility function for loading and saving a contourlet map
//\author Amir Tamrakar
//\date 03/15/08
//
//\verbatim
//Modifications
//
// \endverbatim

#include <vcl_fstream.h>
#include <vcl_string.h>
#include <dbdet/edge/dbdet_edgemap.h>
#include <dbdet/edge/dbdet_edgemap_sptr.h>
#include <dbdet/sel/dbdet_curvelet_map.h>

//: Loads an ascii file containing a curvelet map as well as the edgemap on which it is defined.
//\author Amir Tamrakar
bool dbdet_load_cvlet_map(vcl_string filename, dbdet_edgemap_sptr &edge_map, dbdet_curvelet_map &cvlet_map);

//: Save a curvelet map as a .cvlet file
//\author Amir Tamrakar
bool dbdet_save_cvlet_map(vcl_string filename, dbdet_curvelet_map &cvlet_map);

#endif // dbdet_cvlet_map_io_h

