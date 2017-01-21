//:
// \file
// \brief Class that holds category information
//
// \author Ozge C Ozcanli (ozge@lems.brown.edu)
// \date 10/15/07
//      
// \verbatim
//   Modifications
//  
// \endverbatim

//
//

#include "dborl_category_info.h"

#include <vul/vul_awk.h>
#include <vcl_string.h>
#include <vcl_sstream.h>

dborl_category_info::dborl_category_info(dborl_category_info& other)
{
  name_ = other.name_;
  id_ = other.id_;
  color_ = other.color_;
  prefix_list_.insert(prefix_list_.begin(), other.prefix_list_.begin(), other.prefix_list_.end());
}

void dborl_category_info::set_color(vil_rgb<vxl_byte> c)
{
  color_ = c;
}

void dborl_category_info::set_prefix_list(vcl_vector<vcl_string>& list)
{
  prefix_list_.insert(prefix_list_.begin(), list.begin(), list.end());
}

void dborl_category_info::add_prefix(vcl_string p)
{
  prefix_list_.push_back(p);
}

void dborl_category_info::write_xml(vcl_ostream& os)
{
  os << "\t\t<category>\n";
  os << "\t\t\t<name>" << name_ << "</name>\n";
  os << "\t\t\t<id>" << id_ << "</id>\n";
  os << "\t\t\t<color>" << unsigned(color_.R()) << " " << unsigned(color_.G()) << " " << unsigned(color_.B()) << "</color>\n";
  os << "\t\t\t<prefix>";
  for (unsigned i = 0; i < prefix_list_.size(); i++) 
    os << prefix_list_[i] << " ";
  os << "</prefix>\n";
  os << "\t\t</category>\n";
}

void dborl_category_info_set::add_category(dborl_category_info_sptr c) 
{ 
  cats_.push_back(c); 
}

//: assumes that we're given a simple file where each line is: <category_name> <category_id> or it is a comment line which start with the char: #
bool dborl_category_info_set::read_cats_from_a_simple_file(vcl_istream& is)
{
  vul_awk awk( is );
  if (!awk)
    return false;
  for( ; awk ; ++awk) {
    if ( awk.NF() < 2 ) continue; 
    vcl_string cat_name(awk[0]);    
    if (cat_name[0] == '#') continue;  // skip the comment lines
    vcl_stringstream cat_id(awk[1]);
    int id; cat_id >> id;
    dborl_category_info_sptr cinfo = new dborl_category_info(cat_name, id);
    cats_.push_back(cinfo);
  }
  return true;
}

//: return the class given by the name, return NULL if not in the list
dborl_category_info_sptr dborl_category_info_set::find_category(const vcl_string class_name)
{
  dborl_category_info_sptr out_c;
  for (unsigned i = 0; i < cats_.size(); i++) {
    if (cats_[i]->name_ == class_name) {
      out_c = cats_[i];
      break;
    }
  }
  return out_c;
}

//: Binary io, NOT IMPLEMENTED, signatures defined to use dborl_category_info_set as a brdb_value
void vsl_b_write(vsl_b_ostream & os, dborl_category_info_set const &ph)
{
  vcl_cerr << "vsl_b_write() -- Binary io, NOT IMPLEMENTED, signatures defined to use brec_part_hierarchy as a brdb_value\n";
  return;
}

void vsl_b_read(vsl_b_istream & is, dborl_category_info_set &ph)
{
  vcl_cerr << "vsl_b_read() -- Binary io, NOT IMPLEMENTED, signatures defined to use brec_part_hierarchy as a brdb_value\n";
  return;
}

void vsl_b_read(vsl_b_istream& is, dborl_category_info_set* ph)
{
  delete ph;
  bool not_null_ptr;
  vsl_b_read(is, not_null_ptr);
  if (not_null_ptr)
  {
    ph = new dborl_category_info_set();
    vsl_b_read(is, *ph);
  }
  else
    ph = 0;
}

void vsl_b_write(vsl_b_ostream& os, const dborl_category_info_set* &ph)
{
  if (ph==0)
  {
    vsl_b_write(os, false); // Indicate null pointer stored
  }
  else
  {
    vsl_b_write(os,true); // Indicate non-null pointer stored
    vsl_b_write(os,*ph);
  }
}



