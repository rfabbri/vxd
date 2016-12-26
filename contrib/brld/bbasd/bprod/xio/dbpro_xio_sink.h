// This is basic/dbpro/xio/dbpro_xio_sink.h
#ifndef dbpro_xio_sink_h_
#define dbpro_xio_sink_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief dbpro process for streaming XML output
//
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 10/11/06
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------


#include <dbpro/dbpro_process.h>
#include <vcl_string.h>
#include <vcl_fstream.h>



//: A source for streaming XML to a file
class dbpro_xio_sink : public dbpro_sink
{
  public:
    //: Constructor
    dbpro_xio_sink();

    //: Constructor
    dbpro_xio_sink(const vcl_string& filename);

    //: Destructor
    ~dbpro_xio_sink();

    //: Open a new file
    bool open(const vcl_string& filename);

    //: Runs the process
    dbpro_signal execute();

    //: Only execute if the file stream is valid
    // the const_cast here is a workaround for a bug
    // in some versions of gcc that mistakenly define is_open as non-const
    bool enabled() const { return const_cast<vcl_ofstream&>(os_).is_open(); }

  protected:
    void write_header();
    void write_footer();

  private:
    vcl_ofstream os_;
};



#endif // dbpro_xio_sink_h_
