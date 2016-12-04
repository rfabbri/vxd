#include "dbdif_3rd_order_geometry_storage.h"


//: Constructor
dbdif_3rd_order_geometry_storage::
dbdif_3rd_order_geometry_storage()
{
}

//: Destructor
dbdif_3rd_order_geometry_storage::
~dbdif_3rd_order_geometry_storage()
{
}

//: Create a copy of the object on the heap.
// The caller is responsible for deletion
bpro1_storage* 
dbdif_3rd_order_geometry_storage::clone() const
{
  return new dbdif_3rd_order_geometry_storage(*this);
}

/*

//: Binary save self to stream.
void 
dbdif_3rd_order_geometry_storage::
b_write(vsl_b_ostream &os) const
{
  vsl_b_write(os, version());
  bpro1_storage::b_write(os);
  vsl_b_write(os,c_);
}

//: Binary load self from stream.
void 
dbdif_3rd_order_geometry_storage::
b_read(vsl_b_istream &is)
{
  if (!is) return;

  short ver;
  vsl_b_read(is, ver);
  switch(ver)
  {
  case 1:
  {
    bpro1_storage::b_read(is);
    vsl_b_read(is,c_);
    break;
  }

  default:
    vcl_cerr << "I/O ERROR: dbdif_3rd_order_geometry_storage::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}
*/
