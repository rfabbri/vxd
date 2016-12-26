// This is basic/dbsol/vis/dbsol_vsol2D_displayer.h
#ifndef dbsol_vsol2D_displayer_h_
#define dbsol_vsol2D_displayer_h_

//:
// \file
// \brief This file defines the displayer class that supports
//        dbsol_circ_arc_2d
// \author Isabel Restrepo (mir@lems.brown.edu)
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

class dbsol_vsol2D_displayer : public bvis1_displayer {

public:
  dbsol_vsol2D_displayer(){}

  //: Return type string "vsol2D"
  virtual vcl_string type() const { return "vsol2D"; }

  //: Create a tableau if the storage object is of type vsol2D
  virtual vgui_tableau_sptr make_tableau( bpro1_storage_sptr ) const;

};

#endif // dbsol_vsol2D_displayer_h_
