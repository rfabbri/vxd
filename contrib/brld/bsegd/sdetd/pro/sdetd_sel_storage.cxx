// This is brcv/seg/sdetd/pro/sdetd_sel_storage.h

//:
// \file

#include "sdetd_sel_storage.h"

//: Return IO version number;
short
sdetd_sel_storage::version() const
{
  return 1;
}

sdetd_edgel_chain_list&
sdetd_sel_storage::all_groups_frags()
{
  vcl_list<sdetd_edgel_chain_list>::iterator l_it = c_groups_.begin();
  
  for(; l_it != c_groups_.end(); l_it++)
  {
    sdetd_edgel_chain_list_iter f_it = (*l_it).begin();
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
sdetd_sel_storage::clone() const
{
  return new sdetd_sel_storage(*this);
}

