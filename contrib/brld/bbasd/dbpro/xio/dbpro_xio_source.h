// This is basic/dbpro/xio/dbpro_xio_source.h
#ifndef dbpro_xio_source_h_
#define dbpro_xio_source_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief dbpro processes for streaming XML input
//
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 10/11/06
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------


#include <dbpro/dbpro_process.h>
#include <bxml/bxml_read.h>
#include <vcl_string.h>
#include <vcl_fstream.h>




//: A source for streaming XML from a file
class dbpro_xio_source : public dbpro_source
{
  public:
    //: Constructor
    dbpro_xio_source();

    //: Constructor
    dbpro_xio_source(const vcl_string& filename);

    //: Destructor
    ~dbpro_xio_source();

    //: Open a new file
    bool open(const vcl_string& filename);

    //: Runs the process
    dbpro_signal execute();

  private:
    vcl_ifstream is_;
    bxml_stream_read reader_;
};



#endif // dbpro_xio_source_h_
