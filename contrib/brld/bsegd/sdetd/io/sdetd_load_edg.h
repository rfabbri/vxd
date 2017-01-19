// This is brcv/seg/sdetd/algo/sdetd_load_edg.h
#ifndef sdetd_load_edg_h
#define sdetd_load_edg_h
//:
//\file
//\brief Utility functions for edge input
//\author Based on original code by  Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 01/11/07 15:56:05 EST
//
//\verbatim
//Modifications
//
//  Sep 29 2009  Ricardo Fabbri   Added support for writing gzip-compressed edg
//  Sep 16 2009  Ricardo Fabbri   Added support for reading gzip-compressed edg
//
//               Amir Tamrakar    Separated the load edge functions into two
//                                (for vsol and edgemap)
//
//               Amir Tamrakar    Modified to save and load grid locations as
//                                the integer coordinates.
//
// \endverbatim

#include <vcl_vector.h>
#include <vcl_string.h>
#include <sdet/sdet_edgemap_sptr.h>
#include <vsol/vsol_spatial_object_2d_sptr.h>

//: Loads an ascii file describing edgels.
//\author Based on original code by  Amir Tamrakar
bool sdetd_load_edg(vcl_string input_file, bool bSubPixel, bool blines, double scale,
                    vcl_vector< vsol_spatial_object_2d_sptr > &edgels);

bool sdetd_load_edg(vcl_string input_file, bool bSubPixel, double scale, sdet_edgemap_sptr &edge_map);

//: Save an edgemap as a .edg file
//\author Based on original code by  Amir Tamrakar
bool sdetd_save_edg(vcl_string filename, sdet_edgemap_sptr edgemap);

#endif // sdetd_load_edg_h

