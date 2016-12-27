// This is basic/bprod/bprod_basic_processes.h
#ifndef bprod_basic_processes_h_
#define bprod_basic_processes_h_
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


#include <bprod/bprod_process.h>
#include <vcl_vector.h>

//=============================================================================
// Filters
//=============================================================================


//: Wrap a function (or functor) with one argument
template <class _functor, class T1 >
class bprod_functor1_filter : public bprod_filter
{
 public:
  bprod_functor1_filter(const _functor& fun) : f(fun) {}
  //: Execute the process
  bprod_signal execute()
  {
    assert(input_type_id(0) == typeid(T1));
    output(0, f(input<T1>(0)));
    return BPROD_VALID;
  }
  _functor f;
};


//: Wrap a function (or functor) with two arguments
template <class _functor, class T1, class T2 >
class bprod_functor2_filter : public bprod_filter
{
 public:
  bprod_functor2_filter(const _functor& fun) : f(fun) {}
  //: Execute the process
  bprod_signal execute()
  {
    assert(input_type_id(0) == typeid(T1));
    assert(input_type_id(1) == typeid(T2));
    output(0, f(input<T1>(0),input<T2>(1)));
    return BPROD_VALID;
  }
  _functor f;
};


//: Wrap a function (or functor) with three arguments
template <class _functor, class T1, class T2, class T3 >
class bprod_functor3_filter : public bprod_filter
{
 public:
  bprod_functor3_filter(const _functor& fun) : f(fun) {}
  //: Execute the process
  bprod_signal execute()
  {
    assert(input_type_id(0) == typeid(T1));
    assert(input_type_id(1) == typeid(T2));
    assert(input_type_id(2) == typeid(T3));
    output(0, f(input<T1>(0),input<T2>(1),input<T3>(2)));
    return BPROD_VALID;
  }
  _functor f;
};


//: Static cast from \c inT to \c outT
template <class inT, class outT>
class bprod_static_cast_filter : public bprod_filter
{
 public:
  //: Execute the process
  bprod_signal execute()
  {
    assert(input_type_id(0) == typeid(inT));
    output(0, static_cast<outT>(input<inT>(0)));
    return BPROD_VALID;
  }
};



//=============================================================================
// Sources
//=============================================================================


//: Wrap a function (or functor) with no arguments
template <class _functor >
class bprod_functor_source : public bprod_source
{
 public:
  bprod_functor_source(const _functor& fun) : f(fun) {}

  //: Execute the process
  bprod_signal execute()
  {
    output(0, f());
    return BPROD_VALID;
  }
  _functor f;
};


//: A source that always provides the same data
// (unless modified explictly)
template <class T>
class bprod_static_source : public bprod_source
{
  public:
    bprod_static_source() {}
    bprod_static_source(const T& d) : data(d) {}
    
    //: Execute the process
    bprod_signal execute()
    {
      output(0, data);
      return BPROD_VALID;
    }
    
    T data;
};


template <class T>
class bprod_input_queue : public bprod_source
{
 public:
  bprod_input_queue(const vcl_vector<T>& d) : data(d) {}

  //: Execute the process
  bprod_signal execute()
  {
    if(data.empty()){
      return BPROD_EOS;
    }

    output(0, data.back());
    data.pop_back();
    return BPROD_VALID;
  }

  vcl_vector<T> data;
};


//=============================================================================
// Sinks
//=============================================================================


//: A sink that does nothing
// helps pump data to other observers
class bprod_null_sink : public bprod_sink
{
 public:
  //: Execute the process
  bprod_signal execute() { return BPROD_VALID; }
};



template <class T>
class bprod_output_queue : public bprod_sink
{
 public:
  bprod_output_queue() : data() {}

  //: Execute the process
  bprod_signal execute()
  {
    assert(input_type_id(0) == typeid(T));
    data.push_back(input<T>(0));
    return BPROD_VALID;
  }

  vcl_vector<T> data;
};



#endif // bprod_basic_processes_h_
