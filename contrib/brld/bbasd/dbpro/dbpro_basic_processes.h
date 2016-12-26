// This is basic/dbpro/dbpro_basic_processes.h
#ifndef dbpro_basic_processes_h_
#define dbpro_basic_processes_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief Basic processes that use standard types or are templated
//
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 6/07/06
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------


#include <dbpro/dbpro_process.h>
#include <vcl_vector.h>

//=============================================================================
// Filters
//=============================================================================


//: Wrap a function (or functor) with one argument
template <class _functor, class T1 >
class dbpro_functor1_filter : public dbpro_filter
{
 public:
  dbpro_functor1_filter(const _functor& fun) : f(fun) {}
  //: Execute the process
  dbpro_signal execute()
  {
    assert(input_type_id(0) == typeid(T1));
    output(0, f(input<T1>(0)));
    return DBPRO_VALID;
  }
  _functor f;
};


//: Wrap a function (or functor) with two arguments
template <class _functor, class T1, class T2 >
class dbpro_functor2_filter : public dbpro_filter
{
 public:
  dbpro_functor2_filter(const _functor& fun) : f(fun) {}
  //: Execute the process
  dbpro_signal execute()
  {
    assert(input_type_id(0) == typeid(T1));
    assert(input_type_id(1) == typeid(T2));
    output(0, f(input<T1>(0),input<T2>(1)));
    return DBPRO_VALID;
  }
  _functor f;
};


//: Wrap a function (or functor) with three arguments
template <class _functor, class T1, class T2, class T3 >
class dbpro_functor3_filter : public dbpro_filter
{
 public:
  dbpro_functor3_filter(const _functor& fun) : f(fun) {}
  //: Execute the process
  dbpro_signal execute()
  {
    assert(input_type_id(0) == typeid(T1));
    assert(input_type_id(1) == typeid(T2));
    assert(input_type_id(2) == typeid(T3));
    output(0, f(input<T1>(0),input<T2>(1),input<T3>(2)));
    return DBPRO_VALID;
  }
  _functor f;
};


//: Static cast from \c inT to \c outT
template <class inT, class outT>
class dbpro_static_cast_filter : public dbpro_filter
{
 public:
  //: Execute the process
  dbpro_signal execute()
  {
    assert(input_type_id(0) == typeid(inT));
    output(0, static_cast<outT>(input<inT>(0)));
    return DBPRO_VALID;
  }
};



//=============================================================================
// Sources
//=============================================================================


//: Wrap a function (or functor) with no arguments
template <class _functor >
class dbpro_functor_source : public dbpro_source
{
 public:
  dbpro_functor_source(const _functor& fun) : f(fun) {}

  //: Execute the process
  dbpro_signal execute()
  {
    output(0, f());
    return DBPRO_VALID;
  }
  _functor f;
};


//: A source that always provides the same data
// (unless modified explictly)
template <class T>
class dbpro_static_source : public dbpro_source
{
  public:
    dbpro_static_source() {}
    dbpro_static_source(const T& d) : data(d) {}
    
    //: Execute the process
    dbpro_signal execute()
    {
      output(0, data);
      return DBPRO_VALID;
    }
    
    T data;
};


template <class T>
class dbpro_input_queue : public dbpro_source
{
 public:
  dbpro_input_queue(const vcl_vector<T>& d) : data(d) {}

  //: Execute the process
  dbpro_signal execute()
  {
    if(data.empty()){
      return DBPRO_EOS;
    }

    output(0, data.back());
    data.pop_back();
    return DBPRO_VALID;
  }

  vcl_vector<T> data;
};


//=============================================================================
// Sinks
//=============================================================================


//: A sink that does nothing
// helps pump data to other observers
class dbpro_null_sink : public dbpro_sink
{
 public:
  //: Execute the process
  dbpro_signal execute() { return DBPRO_VALID; }
};



template <class T>
class dbpro_output_queue : public dbpro_sink
{
 public:
  dbpro_output_queue() : data() {}

  //: Execute the process
  dbpro_signal execute()
  {
    assert(input_type_id(0) == typeid(T));
    data.push_back(input<T>(0));
    return DBPRO_VALID;
  }

  vcl_vector<T> data;
};



#endif // dbpro_basic_processes_h_
