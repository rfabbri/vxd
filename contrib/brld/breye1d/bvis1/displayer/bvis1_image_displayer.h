// This is breye1/bvis1/displayer/bvis1_image_displayer.h
#ifndef bvis1_image_displayer_h_
#define bvis1_image_displayer_h_

//:
// \file
// \brief This file defines the displayer class that is responsible
//        for generating a tableau out of vidpro1_image_storage_sptr.
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 10/30/03 
//
// \verbatim
//  Modifications
// \endverbatim

#include <vcl_string.h>
#include <bvis1/bvis1_displayer.h>

#include <vgui/vgui_tableau_sptr.h>
#include <bpro1/bpro1_storage_sptr.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>
#include <bgui/bgui_image_tableau_sptr.h>


class bvis1_image_displayer : public bvis1_displayer {

public:
  bvis1_image_displayer(){}

  //: Return type string "image"
  virtual vcl_string type() const { return "image"; }

  //: Create a tableau if the storage object is of type image
  virtual vgui_tableau_sptr make_tableau( bpro1_storage_sptr ) const;
  
  //: Update a tableau with a the storage object if both are of type image
  virtual bool update_tableau( vgui_tableau_sptr, const bpro1_storage_sptr& ) const;
 
protected:
  //: Perform the update once types are known
  virtual bool update_image_tableau( bgui_image_tableau_sptr, 
                             const vidpro1_image_storage_sptr&) const;

};

#endif // bvis1_image_displayer_h_
