//:
// \file
// \brief 
// \author     Ozge Can Ozcanli (ozge@lems.brown.edu)
// \date        10/03/07
#include "dborl_image_bbox_description.h"

#include <vsol/vsol_box_2d.h>

#include <vcl_iostream.h>

void dborl_image_bbox_description::add_box(vcl_string cat, vsol_box_2d_sptr b)
{
  vcl_map<vcl_string, vcl_vector<vsol_box_2d_sptr> >::iterator iter = data_.find(cat);
  if (iter == data_.end()) {
    vcl_vector<vsol_box_2d_sptr> tmp(1, b);
    data_[cat] = tmp;
  } else {
    (iter->second).push_back(b);
  }
}

//: CAUTION: assumes that cat exists!! check with category_exists() before using
vcl_vector<vsol_box_2d_sptr>& dborl_image_bbox_description::get_box_vector(vcl_string cat)
{
  vcl_map<vcl_string, vcl_vector<vsol_box_2d_sptr> >::iterator iter = data_.find(cat);
  return iter->second;
}

unsigned dborl_image_bbox_description::version()
{
  return 0;
}

void dborl_image_bbox_description::b_read()
{
  vcl_cout << "IMPLEMENT: dborl_image_bbox_description::b_read()\n";
}

void dborl_image_bbox_description::b_write()
{
  vcl_cout << "IMPLEMENT: dborl_image_bbox_description::b_write()\n";
}

void dborl_image_bbox_description::write_xml(vcl_ostream& os)
{
  for (vcl_map<vcl_string, vcl_vector<vsol_box_2d_sptr> >::iterator iter = data_.begin(); iter != data_.end(); iter++) {
    for (unsigned i = 0; i < (iter->second).size(); i++) {
      os << "\t\t<instance>\n";
      os << "\t\t\t<category>" << iter->first << "</category>\n";
      os << "\t\t\t<bndbox>\n";
      vsol_box_2d_sptr box = (iter->second)[i];
      os << "\t\t\t\t<xmin>" << box->get_min_x() << "</xmin>\n";
      os << "\t\t\t\t<ymin>" << box->get_min_y() << "</ymin>\n";
      os << "\t\t\t\t<xmax>" << box->get_max_x() << "</xmax>\n";
      os << "\t\t\t\t<ymax>" << box->get_max_y() << "</ymax>\n";
      os << "\t\t\t</bndbox>\n";
      os << "\t\t</instance>\n";
    }
  }
}

