// This is brcv/seg/dbdet/pro/dbdet_sel_storage.h

//:
// \file

#include "dbdet_sel_storage.h"

//: Return IO version number;
short
dbdet_sel_storage::version() const
{
  return 1;
}

dbdet_edgel_chain_list&
dbdet_sel_storage::all_groups_frags()
{
  vcl_list<dbdet_edgel_chain_list>::iterator l_it = c_groups_.begin();
  
  for(; l_it != c_groups_.end(); l_it++)
  {
    dbdet_edgel_chain_list_iter f_it = (*l_it).begin();
    for (; f_it != (*l_it).end(); f_it++)
    {
      all_groups_frags_.push_back(*f_it);
    }
  }
  return all_groups_frags_;
}
//: Create a copy of the object on the heap.
// The caller is responsible for deletion
bpro1_storage* 
dbdet_sel_storage::clone() const
{
  return new dbdet_sel_storage(*this);
}

