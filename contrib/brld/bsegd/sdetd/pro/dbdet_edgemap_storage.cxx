// This is brcv/seg/dbdet/pro/dbdet_edgemap_storage.h

//:
// \file

#include "dbdet_edgemap_storage.h"

//: Return IO version number;
short
dbdet_edgemap_storage::version() const
{
  return 1;
}


//: Create a copy of the object on the heap.
// The caller is responsible for deletion
bpro1_storage* 
dbdet_edgemap_storage::clone() const
{
  return new dbdet_edgemap_storage(*this);
}

