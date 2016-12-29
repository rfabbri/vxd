// This is brcv/mvg/dbecl/vis/dbecl_episeg_displayer.h
#ifndef dbecl_episeg_displayer_h_
#define dbecl_episeg_displayer_h_

//:
// \file
// \brief This file defines the displayer class that is responsible
//        for generating a tableau out of dbecl_episeg_storage_sptr.
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 11/8/04 
//
// \verbatim
//  Modifications
// \endverbatim

#include <vcl_string.h>
#include <bvis1/bvis1_displayer.h>

#include <vgui/vgui_tableau_sptr.h>
#include <bpro1/bpro1_storage_sptr.h>

#include <vgui/vgui_style.h>

class dbecl_episeg_displayer : public bvis1_displayer {

public:
  // Constructor
  dbecl_episeg_displayer();

  //: Return type string "episeg"
  virtual vcl_string type() const { return "episeg"; }

  //: Create a tableau if the storage object is of type episeg
  virtual vgui_tableau_sptr make_tableau( bpro1_storage_sptr ) const;

  //: This defines the default style used to draw all epi_segments
  static vgui_style_sptr seg_style;

};

#endif // dbecl_episeg_displayer_h_
