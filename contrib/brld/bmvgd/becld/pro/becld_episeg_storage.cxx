// This is bmvgd/becld/pro/becld_episeg_storage.cxx

//:
// \file

#include "becld_episeg_storage.h"

//: Create a copy of the object on the heap.
// The caller is responsible for deletion
bpro1_storage* 
becld_episeg_storage::clone() const
{
  return new becld_episeg_storage(*this);
}


//: Set the episegments
void 
becld_episeg_storage::set_episegs(const vcl_vector<becld_episeg_sptr>& episegs)
{
  episegs_ = episegs;
}


//: Access the episegments
vcl_vector<becld_episeg_sptr> 
becld_episeg_storage::episegs() const
{
  return episegs_;
}
