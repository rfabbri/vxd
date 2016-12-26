#ifndef mw_util_txx_
#define mw_util_txx_

#include "mw_util.h"
#include <vcl_sstream.h>

// Most non-template implementations are in mw_util_defs.cxx

template <typename T>  void mw_util::
parse_num_list(const vcl_string &stringlist, vcl_vector<T> *values)
{
  vcl_istringstream istream;
  istream.str(stringlist);

  T value;
  while ( istream >> value )
    values->push_back(value);
}

#undef MW_UTIL_INSTANTIATE
#define MW_UTIL_INSTANTIATE(T) \
template void mw_util::parse_num_list(const vcl_string &stringlist, vcl_vector<T> *values);

#endif // mw_util_txx_
