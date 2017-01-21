//:
// \file
// \brief Class that holds category information
//
// \author Ozge C Ozcanli (ozge@lems.brown.edu)
// \date 10/15/07
//      
// \verbatim
//   Modifications
//      Ozge C. Ozcanli - added a class to hold a set of category_info objects, to represent a database's class structure
// \endverbatim

// see dborl/algo/dborl_category_info_parser.h for xml read/write utilities
//

#if !defined(_DBORL_CATEGORY_INFO_H)
#define _DBORL_CATEGORY_INFO_H

#include <vbl/vbl_ref_count.h>
#include <vcl_string.h>
#include <vcl_vector.h>
#include <vil/vil_rgb.h>
#include <vxl_config.h>
#include <vsl/vsl_binary_io.h>
#include "dborl_category_info_sptr.h"

class dborl_category_info : public vbl_ref_count
{
public:
  vcl_string name_;
  int id_;
  vil_rgb<vxl_byte> color_;
  vcl_vector<vcl_string> prefix_list_;

  //: the default color is red
  inline dborl_category_info() : name_(""), id_(-1), color_((vxl_byte)255, (vxl_byte)0, (vxl_byte)0) {}
  inline dborl_category_info(vcl_string name) : name_(name), id_(-1), color_((vxl_byte)255, (vxl_byte)0, (vxl_byte)0) {}
  inline dborl_category_info(vcl_string name, int id) : name_(name), id_(id), color_((vxl_byte)255, (vxl_byte)0, (vxl_byte)0) {}
  dborl_category_info(dborl_category_info& other);
  
  void set_name(vcl_string name) { name_ = name; }
  void set_id(int id) { id_ = id; }
  void set_color(vil_rgb<vxl_byte> c);
  void set_prefix_list(vcl_vector<vcl_string>& list);
  void add_prefix(vcl_string p);

  void write_xml(vcl_ostream& os);
};

//: a vector of categories
class dborl_category_info_set : public vbl_ref_count 
{
public:
  dborl_category_info_set() {}
  dborl_category_info_set(const vcl_vector<dborl_category_info_sptr>& cats) : cats_(cats) {}

  void add_category(dborl_category_info_sptr c); 
  vcl_vector<dborl_category_info_sptr>& categories() { return cats_; }

  //: assumes that we're given a simple file where each line is: <category_name> <category_id> or it is a comment line which start with the char: #
  bool read_cats_from_a_simple_file(vcl_istream& is);

  //: return the class given by the name, return NULL if not in the list
  dborl_category_info_sptr find_category(const vcl_string class_name);

protected:
  vcl_vector<dborl_category_info_sptr> cats_;
};

// Binary io, NOT IMPLEMENTED, signatures defined to use dborl_category_info_set as a brdb_value
void vsl_b_write(vsl_b_ostream & os, dborl_category_info_set const &ph);
void vsl_b_read(vsl_b_istream & is, dborl_category_info_set &ph);
void vsl_b_read(vsl_b_istream& is, dborl_category_info_set* ph);
void vsl_b_write(vsl_b_ostream& os, const dborl_category_info_set* &ph);

#endif  //_DBORL_CATEGORY_INFO_H
