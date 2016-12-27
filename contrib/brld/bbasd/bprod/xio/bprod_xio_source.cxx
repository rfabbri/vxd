// This is basic/bprod/xio/bprod_xio_source.cxx

//:
// \file

#include <bprod/xio/bprod_xio_source.h>
#include <bxml/bxml_read.h>


//: Constructor
bprod_xio_source::bprod_xio_source()
  : is_(), reader_(1)
{
}


//: Constructor
bprod_xio_source::bprod_xio_source(const vcl_string& filename)
  : is_(filename.c_str()), reader_(1)
{
}


//: Destructor
bprod_xio_source::~bprod_xio_source()
{
  is_.close();
}


//: Open a new file
bool bprod_xio_source::open(const vcl_string& filename)
{
  is_.close();

  is_.open(filename.c_str());

  reader_.reset();

  if(!is_.good())
    return false;

  return true;
}


//: Runs the process
bprod_signal bprod_xio_source::execute()
{
  if(!is_.is_open())
    return BPROD_INVALID;


  unsigned int depth = 0;
  bxml_data_sptr data = reader_.next_element(is_, depth);

  if(!data || depth < 1){
    return BPROD_EOS;
  }

  output(0,data);

  return BPROD_VALID;
}


