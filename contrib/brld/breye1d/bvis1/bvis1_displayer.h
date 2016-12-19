// This is breye1/bvis1/bvis1_displayer.h
#ifndef bvis1_displayer_h_
#define bvis1_displayer_h_

//:
// \file
// \brief This file defines the abstract base class for bvis1 displayers.
//        A displayer takes a bpro1_storage_sptr and creates the appropriate
//        vgui_tableau_sptr for display.
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 10/30/03 
//
// \verbatim
//  Modifications
// \endverbatim

#include <vbl/vbl_ref_count.h>
#include <vcl_string.h>

#include <vgui/vgui_tableau_sptr.h>
#include <bpro1/bpro1_storage_sptr.h>
#include <bvis1/bvis1_mapper_sptr.h>

class bvis1_displayer : public vbl_ref_count{

public:
  bvis1_displayer(): mapper_(0){}
  //: Abstract function to return a string description of the type
  virtual vcl_string type() const = 0;

  //: Abstract function to generate a tableau from a storage object
  virtual vgui_tableau_sptr make_tableau( bpro1_storage_sptr ) const = 0;

  //: Abstract function to update a tableau from a storage object
  virtual bool update_tableau( vgui_tableau_sptr, const bpro1_storage_sptr& ) const { return false; }

  //: Return true if this displayer produces tableau that are safe to cache
  virtual bool cacheable() const { return true; }

  //: Designate a mapping for storage attributes
  void set_mapper(bvis1_mapper_sptr const& mapper){mapper_ = mapper;}

 protected:
  bvis1_mapper_sptr mapper_;
};

#endif // bvis1_displayer_h_
