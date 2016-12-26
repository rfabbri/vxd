// This is bbasd/buld/buld_parse_simple_file.h
#ifndef buld_parse_simple_file_h_
#define buld_parse_simple_file_h_
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma interface
#endif
//:
// \file
// \brief Parsing simple commonly-used files
//
//
// \author Based on original code by  Nhon Trinh (@lems.brown.edu)
// \date   September 18, 2008
//
// \verbatim
//  Modifications  Ricardo Fabbri : added buld_parse_number_lists
// \endverbatim

#include <vcl_string.h>
#include <vcl_vector.h>
#include <vcl_map.h>

//: Parse a file containing a list of strings (no space character), each string on one line
bool buld_parse_string_list(const vcl_string& input_file, 
                            vcl_vector<vcl_string >& string_list);

//: Parse a file containing space-separated list of numbers, one list per line.
// Each list can have a different length.
// Comments using conf-style '#' are allowed.
//
// \param[out] number_lists[i] : the list of ints for line[i]
//
// \remarks this is efficient for small files.
bool buld_parse_number_lists (
      const vcl_string &fname,
      vcl_vector< vcl_vector<int> > &number_lists);


//: Parse an ini-style parameter file
// For file format, check out: http://en.wikipedia.org/wiki/INI_file
// Example: 
// 
// ; comments
// [ccm_parameters]
// nbins_0to_pi=18
//
// ; another comment
// 
// [positive_data]
// perturb_delta_x=2
// perturb_delta_y=2
//
// Output: a map with  key  : section name
//                     value: list of key-value pairs in the section
bool buld_parse_ini_file(const vcl_string& fname,
                         vcl_map<vcl_string, vcl_map<vcl_string, vcl_string > >& section_params);

#endif // buld_parse_simple_file_h_
