// This is brcv/seg/sdet/pro/sdetd_edgemap_storage.h
#ifndef sdetd_edgemap_storage_h_
#define sdetd_edgemap_storage_h_

//:
// \file
// \brief Storage class for edge maps
// \author Amir Tamrakar
// \date 09/11/06
//
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_storage.h>
#include <sdet/sdet_edgemap_sptr.h>
#include <sdetd/pro/sdetd_edgemap_storage_sptr.h>

//: Storage class for the symbolic edge linker object
class sdetd_edgemap_storage : public bpro1_storage {
public:

  //: Constructor
  sdetd_edgemap_storage() {}

  //: Destructor
  virtual ~sdetd_edgemap_storage(){}

  //: Returns the type string "edge_map"
  virtual vcl_string type() const { return "edge_map"; }

  //: Return IO version number;
  short version() const;

  //: Create a copy of the object on the heap.
  // The caller is responsible for deletion
  virtual bpro1_storage* clone() const;
  
  //: Return a platform independent string identifying the class
  virtual vcl_string is_a() const { return "sdetd_edgemap_storage"; }

  void set_edgemap(sdet_edgemap_sptr edgemap) { edge_map_= edgemap; }
  sdet_edgemap_sptr get_edgemap() { return edge_map_; }

private:

  //: edge map structure
  sdet_edgemap_sptr edge_map_;

};


//: Create a smart-pointer to a sdetd_edgemap_storage.
struct sdetd_edgemap_storage_new : public sdetd_edgemap_storage_sptr
{
  typedef sdetd_edgemap_storage_sptr base;

  //: Constructor - creates a default sdetd_edgemap_storage_sptr.
  sdetd_edgemap_storage_new() : base(new sdetd_edgemap_storage()) { }

};


#endif //sdetd_edgemap_storage_h_
