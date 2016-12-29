// This is brcv/mvg/dbecl/pro/dbecl_episeg_storage.cxx

//:
// \file

#include "dbecl_episeg_storage.h"

//: Create a copy of the object on the heap.
// The caller is responsible for deletion
bpro1_storage* 
dbecl_episeg_storage::clone() const
{
  return new dbecl_episeg_storage(*this);
}


//: Set the episegments
void 
dbecl_episeg_storage::set_episegs(const vcl_vector<dbecl_episeg_sptr>& episegs)
{
  episegs_ = episegs;
}


//: Access the episegments
vcl_vector<dbecl_episeg_sptr> 
dbecl_episeg_storage::episegs() const
{
  return episegs_;
}
