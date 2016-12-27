// This is basic/bprod/tests/bprod_sample_processes.h
#ifndef bprod_sample_processes_h_
#define bprod_sample_processes_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief Sample processes used in test cases
//
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 5/31/06
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------


#include <bprod/bprod_process.h>
#include <bprod/bprod_process_factory.h>
#include <vcl_cmath.h>
#include <vcl_vector.h>

//=============================================================================
// Filters
//=============================================================================

template <class T>
class bprod_sum : public bprod_filter
{
 public:
  //: Execute the process
  bprod_signal execute()
  {
    assert(input_type_id(0) == typeid(T));
    assert(input_type_id(1) == typeid(T));
    T val1 = input<T>(0);
    T val2 = input<T>(1);
    output(0, T(val1+val2));
    return BPROD_VALID;
  }
};


template <class T>
class bprod_diff : public bprod_filter
{
 public:
  //: Execute the process
  bprod_signal execute()
  {
    assert(input_type_id(0) == typeid(T));
    assert(input_type_id(1) == typeid(T));
    T val1 = input<T>(0);
    T val2 = input<T>(1);
    output(0, T(val1-val2));
    return BPROD_VALID;
  }
};


template <class inT, class outT>
class bprod_static_cast : public bprod_filter
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



class bprod_sqrt : public bprod_filter
{
  public:
  //: Execute the process
  bprod_signal execute()
  {
    assert(input_type_id(0) == typeid(double));
    output(0, vcl_sqrt(input<double>(0)));
    return BPROD_VALID;
  }
};


//=============================================================================
// Sources
//=============================================================================


template <class T>
class bprod_input_queue : public bprod_source
{
 public:
  bprod_input_queue(const vcl_vector<T>& d) : data(d) {}

  //: Execute the process
  bprod_signal execute()
  {
    if(data.empty()){
      output(0, BPROD_EOS);
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


//=============================================================================
// Factories
//=============================================================================

template <class T>
class bprod_sum_factory : public bprod_process_factory
{
  public:
    //: Return the default set of parameters for the process
    virtual bprod_parameters_sptr default_params() const;

    //: Construct a process from a set of parameters
    virtual bprod_process_sptr create(const bprod_parameters_sptr& params) const;

    //: The name of the process
    virtual vcl_string name() const { return vcl_string("Sum<")+typeid(T).name()+">"; }

    virtual ~bprod_sum_factory() {}
};

#endif // bprod_sample_processes_h_
