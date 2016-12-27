// This is basic/bprod/xio/bprod_xio_source.h
#ifndef bprod_xio_source_h_
#define bprod_xio_source_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief bprod processes for streaming XML input
//
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 10/11/06
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------


#include <bprod/bprod_process.h>
#include <bxml/bxml_read.h>
#include <vcl_string.h>
#include <vcl_fstream.h>




//: A source for streaming XML from a file
class bprod_xio_source : public bprod_source
{
  public:
    //: Constructor
    bprod_xio_source();

    //: Constructor
    bprod_xio_source(const vcl_string& filename);

    //: Destructor
    ~bprod_xio_source();

    //: Open a new file
    bool open(const vcl_string& filename);

    //: Runs the process
    bprod_signal execute();

  private:
    vcl_ifstream is_;
    bxml_stream_read reader_;
};



#endif // bprod_xio_source_h_
