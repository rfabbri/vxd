// This is basic/dbul/dbul_parse_simple_file.cxx
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma implementation
#endif

//:
// \file

#include "dbul_parse_simple_file.h"
//#include <vcl_cstdlib.h>

#include <vcl_iostream.h>
#include <vcl_fstream.h>
#include <vul/vul_string.h>
#include <vul/vul_awk.h>
#include <vcl_cstring.h>
#include <vcl_utility.h>

bool dbul_parse_string_list(const vcl_string& input_file, 
                            vcl_vector<vcl_string >& string_list)
{
  string_list.clear();

  // parse image list
  vcl_ifstream ifs(input_file.c_str());
  if (!ifs) 
  {
    vcl_cout << "ERROR: Unable to open input file " << input_file << vcl_endl;
    return false;
  }

  // read the image names, one by one
  while (!ifs.eof()) 
  {
    // read the full line in
    vcl_string line;
    vcl_getline(ifs, line);
    
    // ignore lines starting with "#"
    if (line.empty() || line[0] == '#') 
      continue;

    vcl_string name = line.substr(0, line.find_first_of(' '));
    if (!name.empty())
      string_list.push_back(name);
  }
  ifs.close();
  return true;
}

bool 
dbul_parse_number_lists (
      const vcl_string &fname,
      vcl_vector< vcl_vector<int> > &number_lists)
{
  vcl_ifstream infp(fname.c_str(), vcl_ios::in);
  if (!infp) {
    vcl_cerr << "dbul: Error, unable to open file name " 
      << fname << vcl_endl;
    return false;
  }

  vul_awk awk(infp, vul_awk::strip_comments);

  for (; awk; ++awk) {
    if (!awk.NF())
      continue;
    vcl_vector<int> line_list(awk.NF());
    for (int i=0; i < awk.NF(); ++i) {
      // vcl_cout << "Line: " << awk.NR() << ", element[" << i << "] = " << awk[i] << vcl_endl;
      line_list[i] = vul_string_atoi(awk[i]);
    }
    number_lists.push_back(line_list);
  }

  return true;
}










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
bool dbul_parse_ini_file(const vcl_string& fname,
                         vcl_map<vcl_string, vcl_map<vcl_string, vcl_string > >& section_params)
{
  section_params.clear();
  vcl_map<vcl_string, vcl_string >* cur_sec = 0;


  vcl_ifstream infp(fname.c_str(), vcl_ios::in);

  // Read in each line
  char line_buffer[1024];
  int line_number = 0;
  while (infp.getline(line_buffer, 1024)) 
  {
    ++line_number;

    //ignore comment lines and empty lines
    if (vcl_strlen(line_buffer)<2 || line_buffer[0]=='#' || line_buffer[0]==';')
      continue;

    vcl_string line(line_buffer);

    // begin a new section
    if (line[0] == '[') 
    {
      // fine the closing bracket
      vcl_string::size_type closing_bracket = line.find_first_of(']');
      if (closing_bracket == vcl_string::npos)
      {
        vcl_cout << "\nERROR: can't process line " << line_number << ".";
        return false;
      }

      vcl_string section_name = line.substr(1, closing_bracket-1);
      cur_sec = &(section_params[section_name]);
    }
    // parse value-pair
    else
    {
      // ignore everyting before the first section
      if (!cur_sec)
        continue;

      vcl_string::size_type equal_sign = line.find_first_of('=');
      if (equal_sign == vcl_string::npos)
      {
        vcl_cout << "\nERROR: can't process line " << line_number << ".";
        return false;
      }
      
      // key-value pair
      vcl_string key = line.substr(0, equal_sign);
      vcl_string value = line.substr(equal_sign+1, line.size()-equal_sign-1);

      if (key.empty())
        continue;

      (*cur_sec)[key] = value;
    }
  }
  return true;
}
