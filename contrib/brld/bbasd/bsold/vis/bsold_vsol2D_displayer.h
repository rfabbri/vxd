// This is bbasd/bsold/vis/bsold_vsol2D_displayer.h
#ifndef bsold_vsol2D_displayer_h_
#define bsold_vsol2D_displayer_h_

//:
// \file
// \brief This file defines the displayer class that supports
//        bsold_circ_arc_2d
// \author Based on original code by  Isabel Restrepo (@lems.brown.edu)
// \date 09/26/06 
//
// \verbatim
//  Modifications
// \endverbatim

#include <vcl_string.h>
#include <bvis1/bvis1_displayer.h>

#include <vgui/vgui_tableau_sptr.h>
#include <bpro1/bpro1_storage_sptr.h>
#include <bvis1/bvis1_mapper.h>

class bsold_vsol2D_displayer : public bvis1_displayer {

public:
  bsold_vsol2D_displayer(){}

  //: Return type string "vsol2D"
  virtual vcl_string type() const { return "vsol2D"; }

  //: Create a tableau if the storage object is of type vsol2D
  virtual vgui_tableau_sptr make_tableau( bpro1_storage_sptr ) const;

};

#endif // bsold_vsol2D_displayer_h_
