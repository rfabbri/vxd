// This is basic/dbpro/dbpro_vsl_processes.h
#ifndef dbpro_vsl_processes_h_
#define dbpro_vsl_processes_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief Basic sources and sinks using vsl binary IO
//
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 1/22/08
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------


#include <dbpro/dbpro_process.h>
#include <vcl_vector.h>
#include <vcl_iostream.h>
#include <vsl/vsl_binary_io.h>
#include <vcl_memory.h>


//=============================================================================
// Sources
//=============================================================================


//: Read data from a (vsl) binary istream (base class)
class dbpro_b_istream_source_base : public dbpro_source
{
  public:
    dbpro_b_istream_source_base(const vcl_string& filename) 
    : is(new vsl_b_ifstream(filename)) {}
    
    void open(const vcl_string& filename) 
    { is = vcl_auto_ptr<vsl_b_istream>(new vsl_b_ifstream(filename)); }
    
    //: Execute the process
    dbpro_signal execute()
    {
      return DBPRO_INVALID;
    }
    vcl_auto_ptr<vsl_b_istream> is;
};


//: Read data of type T from a (vsl) binary istream
template <class T >
class dbpro_b_istream_source : public dbpro_b_istream_source_base
{
 public:
  dbpro_b_istream_source(const vcl_string& filename) 
  : dbpro_b_istream_source_base(filename) {}

  //: Execute the process
  dbpro_signal execute()
  {
    if(is->is().eof()){
      return DBPRO_EOS;
    }
    T data;
    vsl_b_read(*is.get(),data);
    output(0, data);
    return DBPRO_VALID;
  }
  
};


//=============================================================================
// Sinks
//=============================================================================


//: Write data to a (vsl) binary ostream (base class)
class dbpro_b_ostream_sink_base : public dbpro_sink
  {
  public:
    dbpro_b_ostream_sink_base(const vcl_string& filename) 
    : os(new vsl_b_ofstream(filename)) {}
    
    void open(const vcl_string& filename) 
    { os = vcl_auto_ptr<vsl_b_ostream>(new vsl_b_ofstream(filename)); }
    
    bool enabled() const { return (os.get() != NULL && os->os().good()); }
    
    //: Execute the process
    dbpro_signal execute()
    {
      return DBPRO_INVALID;
    }
    vcl_auto_ptr<vsl_b_ostream> os;
  };


//: Write data of type T to a (vsl) binary ostream
template <class T >
class dbpro_b_ostream_sink : public dbpro_b_ostream_sink_base
  {
  public:
    dbpro_b_ostream_sink(const vcl_string& filename) 
    : dbpro_b_ostream_sink_base(filename) {}
    
    //: Execute the process
    dbpro_signal execute()
    {
      assert(input_type_id(0) == typeid(T));
      vsl_b_write(*os.get(), input<T>(0));
      return DBPRO_VALID;
    }
    
  };




#endif // dbpro_vsl_processes_h_
