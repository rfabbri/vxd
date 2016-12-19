// This is breye1/bvis1/displayer/bvis1_vsol2D_displayer.h
#ifndef bvis1_vsol2D_displayer_h_
#define bvis1_vsol2D_displayer_h_

//:
// \file
// \brief This file defines the displayer class that is responsible
//        for generating a tableau out of vidpro1_vsol2D_storage_sptr.
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 11/07/03 
//
// \verbatim
//  Modifications
// \endverbatim

#include <vcl_string.h>
#include <bvis1/bvis1_displayer.h>

#include <vgui/vgui_tableau_sptr.h>
#include <bpro1/bpro1_storage_sptr.h>
#include <bvis1/bvis1_mapper.h>

class bvis1_vsol2D_displayer : public bvis1_displayer {

public:
  bvis1_vsol2D_displayer(){}

  //: Return type string "vsol2D"
  virtual vcl_string type() const { return "vsol2D"; }

  //: Create a tableau if the storage object is of type vsol2D
  virtual vgui_tableau_sptr make_tableau( bpro1_storage_sptr ) const;

};

#endif // bvis1_vsol2D_displayer_h_
