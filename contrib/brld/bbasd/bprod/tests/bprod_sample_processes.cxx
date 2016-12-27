// This is basic/bprod/tests/bprod_sample_processes.cxx

//:
// \file

#include "bprod_sample_processes.h"
#include <vcl_iostream.h>
#include <bprod/bprod_parameters.h>
#include <vxl_config.h>



//: Return the default set of parameters for the process
template <class T>
bprod_parameters_sptr
bprod_sum_factory<T>::default_params() const
{
  // no parameters to add
  bprod_parameters_sptr p = new bprod_parameters();
  return p;
}


//: Construct a process from a set of parameters
template <class T>
bprod_process_sptr
bprod_sum_factory<T>::create(const bprod_parameters_sptr& params) const
{
  return new bprod_sum<T>;
}


bprod_process_factory::registrar sample_reglist[] =
{
  new bprod_sum_factory<int>,
  new bprod_sum_factory<unsigned int>,
  new bprod_sum_factory<vxl_byte>,
  new bprod_sum_factory<vxl_uint_16>
};
