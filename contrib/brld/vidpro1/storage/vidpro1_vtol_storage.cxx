// This is brl/vidpro1/storage/vidpro1_vtol_storage.cxx

//:
// \file

#include "vidpro1_vtol_storage.h"


//: Constructor
vidpro1_vtol_storage::vidpro1_vtol_storage()
{
}


//: Destructor
vidpro1_vtol_storage::~vidpro1_vtol_storage()
{
}



//: Create a copy of the object on the heap.
// The caller is responsible for deletion
bpro1_storage* 
vidpro1_vtol_storage::clone() const
{
  return new vidpro1_vtol_storage(*this);
}


//: Clear all vtol topology objects
void
vidpro1_vtol_storage::clear()
{
  data_set_.clear();
}


//: Add a vtol topology object to the storage class
void
vidpro1_vtol_storage::add_vtol( const vtol_topology_object_sptr& top_obj )
{
  data_set_.insert(top_obj);
}


//: Remove a vtol topology object from the storage class if it exists
bool
vidpro1_vtol_storage::remove_vtol( const vtol_topology_object_sptr& top_obj )
{
  return (data_set_.erase(top_obj) > 0);
}

//: The number of topology objects stored
int
vidpro1_vtol_storage::size() const
{
  return data_set_.size();
}


//: Return a const iterator to the beginning of the data
vcl_set<vtol_topology_object_sptr>::const_iterator
vidpro1_vtol_storage::begin() const
{
  return data_set_.begin();
}


//: Return a const iterator to the end of the data
vcl_set<vtol_topology_object_sptr>::const_iterator
vidpro1_vtol_storage::end() const
{
  return data_set_.end();
}
