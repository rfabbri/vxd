// This is brcv/seg/dbdet/pro/dbdet_edgemap_storage.h
#ifndef dbdet_edgemap_storage_h_
#define dbdet_edgemap_storage_h_

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
#include "dbdet_edgemap_storage_sptr.h"
#include <dbdet/edge/dbdet_edgemap_sptr.h>

//: Storage class for the symbolic edge linker object
class dbdet_edgemap_storage : public bpro1_storage {
public:

  //: Constructor
  dbdet_edgemap_storage() {}

  //: Destructor
  virtual ~dbdet_edgemap_storage(){}

  //: Returns the type string "edge_map"
  virtual vcl_string type() const { return "edge_map"; }

  //: Return IO version number;
  short version() const;

  //: Create a copy of the object on the heap.
  // The caller is responsible for deletion
  virtual bpro1_storage* clone() const;
  
  //: Return a platform independent string identifying the class
  virtual vcl_string is_a() const { return "dbdet_edgemap_storage"; }

  void set_edgemap(dbdet_edgemap_sptr edgemap) { edge_map_= edgemap; }
  dbdet_edgemap_sptr get_edgemap() { return edge_map_; }

private:

  //: edge map structure
  dbdet_edgemap_sptr edge_map_;

};


//: Create a smart-pointer to a dbdet_edgemap_storage.
struct dbdet_edgemap_storage_new : public dbdet_edgemap_storage_sptr
{
  typedef dbdet_edgemap_storage_sptr base;

  //: Constructor - creates a default dbdet_edgemap_storage_sptr.
  dbdet_edgemap_storage_new() : base(new dbdet_edgemap_storage()) { }

};


#endif //dbdet_edgemap_storage_h_
