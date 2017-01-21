#include "bmcsd_discrete_corresp_storage.h"


//: Constructor
bmcsd_discrete_corresp_storage::
bmcsd_discrete_corresp_storage()
  : c_(0)
{
}

//: Destructor
bmcsd_discrete_corresp_storage::
~bmcsd_discrete_corresp_storage()
{
  if (c_) delete c_;
}

//: Create a copy of the object on the heap.
// The caller is responsible for deletion
bpro1_storage* 
bmcsd_discrete_corresp_storage::clone() const
{
  return new bmcsd_discrete_corresp_storage(*this);
}


//: Binary save self to stream.
void 
bmcsd_discrete_corresp_storage::
b_write(vsl_b_ostream &os) const
{
  vsl_b_write(os, version());
  bpro1_storage::b_write(os);
  if (c_) {
    vsl_b_write(os,true);
    vsl_b_write(os,*c_);
  } else
    vsl_b_write(os,false);
}

//: Binary load self from stream.
void 
bmcsd_discrete_corresp_storage::
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

    bool is_valid;
    vsl_b_read(is,is_valid);
    if (!is_valid)
      this->c_ = 0;
    else {
      bmcsd_discrete_corresp *corresp;
      corresp = new bmcsd_discrete_corresp();
      vsl_b_read(is,*corresp);
      if (this->c_)
        delete this->c_;
      c_ = corresp;
    }

    break;
  }

  default:
    vcl_cerr << "I/O ERROR: bmcsd_discrete_corresp_storage::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}
