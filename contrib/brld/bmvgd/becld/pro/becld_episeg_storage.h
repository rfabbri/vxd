// This is bmvgd/becld/pro/becld_episeg_storage.h
#ifndef becldd_episeg_storage_h
#define becldd_episeg_storage_h

// \file
// \brief A storage class for becld episegments
// \author Based on original code by   mleotta
// \date  11/8/2004

#include <bpro1/bpro1_storage.h>
#include <becld/becld_episeg_sptr.h>
#include "becld_episeg_storage_sptr.h"

//: Storage class for becld episegments
class becld_episeg_storage : public bpro1_storage {

public:
  //: Constructor
  becld_episeg_storage() {}
  //: Destructor
  virtual ~becld_episeg_storage() {}
  virtual vcl_string type() const { return "episeg"; }

  //: Create a copy of the object on the heap.
  // The caller is responsible for deletion
  virtual bpro1_storage* clone() const;
  
  //: Return a platform independent string identifying the class
  virtual vcl_string is_a() const { return "becld_episeg_storage"; }

  //: Set the episegments
  void set_episegs(const vcl_vector<becld_episeg_sptr>& episegs);

  //: Access the episegments
  vcl_vector<becld_episeg_sptr> episegs() const;
  
private:
  vcl_vector<becld_episeg_sptr> episegs_;

};


//: Create a smart-pointer to a becld_episeg_storage.
struct becld_episeg_storage_new : public becld_episeg_storage_sptr
{
  typedef becld_episeg_storage_sptr base;

  //: Constructor - creates a default becld_episeg_storage_sptr.
  becld_episeg_storage_new() : base(new becld_episeg_storage()) { }
};

#endif
