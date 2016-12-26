// This is breye/dbpro/dbpro_parameters.hxx
#ifndef dbpro_parameters_txx_
#define dbpro_parameters_txx_
//:
// \file
// \brief Templated code for dbpro parameters.
// \author Matt Leotta, (mleotta@lems.brown.edu)
// \date 7/2/2004


#include "dbpro_parameters.h"
#include <vcl_utility.h>
#include <vcl_iostream.h>


//: Set the current value to \p val
template<class T>
bool
dbpro_param_type<T>::set_value( const T& val )
{
  if( has_bounds_ && (val < min_value_ || max_value_ < val) )
    return false;

  value_ = val;
  return true;
}


//: Create a string representation of the value
template<class T>
vcl_string
dbpro_param_type<T>::create_string(const T& val) const
{
  vcl_stringstream stm;
  stm << val;
  return stm.str();
}


//: Parse a string representation of the value
template<class T>
T
dbpro_param_type<T>::parse_string(const vcl_string& input) const
{
  T val;
  vcl_istringstream strm(input);
  strm >> val;
  return val;
}

//===========================================================================================

//: Use this macro to instantiate dbpro_parameters for each parameter type
#define DBPRO_PARAMETERS_INSTANTIATE(T) \
template class dbpro_param_type< T >;

#endif // dbpro_parameters_txx_
