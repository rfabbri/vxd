// This is brcv/seg/dbdet/pro/sdetd_edgemap_storage.h

//:
// \file

#include "sdetd_edgemap_storage.h"

//: Return IO version number;
short
sdetd_edgemap_storage::version() const
{
  return 1;
}


//: Create a copy of the object on the heap.
// The caller is responsible for deletion
bpro1_storage* 
sdetd_edgemap_storage::clone() const
{
  return new sdetd_edgemap_storage(*this);
}

