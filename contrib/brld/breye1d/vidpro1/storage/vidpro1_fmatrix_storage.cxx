// This is brl/vidpro1/storage/vidpro1_fmatrix_storage.cxx

//:
// \file
#include <iostream>
#include "vidpro1_fmatrix_storage.h"
#include <vsl/vsl_map_io.h>


//: Constructor
vidpro1_fmatrix_storage::vidpro1_fmatrix_storage(const vcl_string& from_name)
 : from_name_(from_name)
{
}

//: Destructor
vidpro1_fmatrix_storage::~vidpro1_fmatrix_storage()
{
}


//: Create a copy of the object on the heap.
// The caller is responsible for deletion
bpro1_storage* 
vidpro1_fmatrix_storage::clone() const
{
  return new vidpro1_fmatrix_storage(*this);
}


//: Return IO version number;
short 
vidpro1_fmatrix_storage::version() const
{
  return 1;
}


//: Binary save self to stream.
void 
vidpro1_fmatrix_storage::b_write(vsl_b_ostream &os) const
{
  vsl_b_write(os, version());
  bpro1_storage::b_write(os);
  vsl_b_write(os, f_map_);
}


//: Binary load self from stream.
void 
vidpro1_fmatrix_storage::b_read(vsl_b_istream &is)
{
  if (!is) return;

  short ver;
  vsl_b_read(is, ver);
  switch(ver)
  {
  case 1:
  {
    bpro1_storage::b_read(is);
    vsl_b_read(is, f_map_);
    break;
  }

  default:
    vcl_cerr << "I/O ERROR: vidpro1_fmatrix_storage::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}


//: Add an FMatrix related to an object with \p name at \p frame
void 
vidpro1_fmatrix_storage::add_fmatrix( const vnl_double_3x3& F, 
                                     int frame, const vcl_string& name )
{
  f_map_[frame][name] = F;
}


//: Retrieve an FMatrix related to an object with \p name at \p frame
bool
vidpro1_fmatrix_storage::get_fmatrix( vnl_double_3x3& F, 
                                     int frame, const vcl_string& name ) const
{
  data_map::const_iterator s1 = f_map_.find(frame);
  if(s1 == f_map_.end())
    return false;
  vcl_map<vcl_string, vnl_double_3x3>::const_iterator s2 = s1->second.find(name);
  if(s2 == s1->second.end())
    return false;

  F = s2->second;
  return true;
}


