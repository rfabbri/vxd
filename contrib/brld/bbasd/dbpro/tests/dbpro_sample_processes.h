// This is basic/dbpro/tests/dbpro_sample_processes.h
#ifndef dbpro_sample_processes_h_
#define dbpro_sample_processes_h_
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


#include <dbpro/dbpro_process.h>
#include <dbpro/dbpro_process_factory.h>
#include <vcl_cmath.h>
#include <vcl_vector.h>

//=============================================================================
// Filters
//=============================================================================

template <class T>
class dbpro_sum : public dbpro_filter
{
 public:
  //: Execute the process
  dbpro_signal execute()
  {
    assert(input_type_id(0) == typeid(T));
    assert(input_type_id(1) == typeid(T));
    T val1 = input<T>(0);
    T val2 = input<T>(1);
    output(0, T(val1+val2));
    return DBPRO_VALID;
  }
};


template <class T>
class dbpro_diff : public dbpro_filter
{
 public:
  //: Execute the process
  dbpro_signal execute()
  {
    assert(input_type_id(0) == typeid(T));
    assert(input_type_id(1) == typeid(T));
    T val1 = input<T>(0);
    T val2 = input<T>(1);
    output(0, T(val1-val2));
    return DBPRO_VALID;
  }
};


template <class inT, class outT>
class dbpro_static_cast : public dbpro_filter
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



class dbpro_sqrt : public dbpro_filter
{
  public:
  //: Execute the process
  dbpro_signal execute()
  {
    assert(input_type_id(0) == typeid(double));
    output(0, vcl_sqrt(input<double>(0)));
    return DBPRO_VALID;
  }
};


//=============================================================================
// Sources
//=============================================================================


template <class T>
class dbpro_input_queue : public dbpro_source
{
 public:
  dbpro_input_queue(const vcl_vector<T>& d) : data(d) {}

  //: Execute the process
  dbpro_signal execute()
  {
    if(data.empty()){
      output(0, DBPRO_EOS);
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


//=============================================================================
// Factories
//=============================================================================

template <class T>
class dbpro_sum_factory : public dbpro_process_factory
{
  public:
    //: Return the default set of parameters for the process
    virtual dbpro_parameters_sptr default_params() const;

    //: Construct a process from a set of parameters
    virtual dbpro_process_sptr create(const dbpro_parameters_sptr& params) const;

    //: The name of the process
    virtual vcl_string name() const { return vcl_string("Sum<")+typeid(T).name()+">"; }

    virtual ~dbpro_sum_factory() {}
};

#endif // dbpro_sample_processes_h_
