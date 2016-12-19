// This is breye1/bvis1/displayer/bvis1_vtol_displayer.h
#ifndef bvis1_vtol_displayer_h_
#define bvis1_vtol_displayer_h_

//:
// \file
// \brief This file defines the displayer class that is responsible
//        for generating a tableau out of vidpro1_vtol_storage_sptr.
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 2/02/04 
//
// \verbatim
//  Modifications
// \endverbatim

#include <vcl_string.h>
#include <bvis1/bvis1_displayer.h>

#include <vgui/vgui_tableau_sptr.h>
#include <bpro1/bpro1_storage_sptr.h>


class bvis1_vtol_displayer : public bvis1_displayer {

public:
  bvis1_vtol_displayer(){}

  //: Return type string "vtol"
  virtual vcl_string type() const { return "vtol"; }

  //: Create a tableau if the storage object is of type vtol
  virtual vgui_tableau_sptr make_tableau( bpro1_storage_sptr ) const;

};

#endif // bvis1_vtol_displayer_h_
