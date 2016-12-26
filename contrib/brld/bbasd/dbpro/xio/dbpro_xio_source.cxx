// This is basic/dbpro/xio/dbpro_xio_source.cxx

//:
// \file

#include <dbpro/xio/dbpro_xio_source.h>
#include <bxml/bxml_read.h>


//: Constructor
dbpro_xio_source::dbpro_xio_source()
  : is_(), reader_(1)
{
}


//: Constructor
dbpro_xio_source::dbpro_xio_source(const vcl_string& filename)
  : is_(filename.c_str()), reader_(1)
{
}


//: Destructor
dbpro_xio_source::~dbpro_xio_source()
{
  is_.close();
}


//: Open a new file
bool dbpro_xio_source::open(const vcl_string& filename)
{
  is_.close();

  is_.open(filename.c_str());

  reader_.reset();

  if(!is_.good())
    return false;

  return true;
}


//: Runs the process
dbpro_signal dbpro_xio_source::execute()
{
  if(!is_.is_open())
    return DBPRO_INVALID;


  unsigned int depth = 0;
  bxml_data_sptr data = reader_.next_element(is_, depth);

  if(!data || depth < 1){
    return DBPRO_EOS;
  }

  output(0,data);

  return DBPRO_VALID;
}


