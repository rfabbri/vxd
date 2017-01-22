//:
// \file
// \brief 
// \author     Ozge Can Ozcanli (ozge@lems.brown.edu)
// \date        10/03/07
#include "dborl_image_polygon_description.h"

#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_point_2d.h>

#include <vcl_iostream.h>

void dborl_image_polygon_description::add_polygon(vcl_string cat, vsol_polygon_2d_sptr p)
{
  vcl_map<vcl_string, vcl_vector<vsol_polygon_2d_sptr> >::iterator iter = data_.find(cat);
  if (iter == data_.end()) {
    vcl_vector<vsol_polygon_2d_sptr> tmp(1, p);
    data_[cat] = tmp;
  } else {
    (iter->second).push_back(p);
  }
}

vcl_vector<vsol_polygon_2d_sptr>& dborl_image_polygon_description::get_polygon_vector(vcl_string cat)
{
  vcl_map<vcl_string, vcl_vector<vsol_polygon_2d_sptr> >::iterator iter = data_.find(cat);
  return iter->second;
}

unsigned dborl_image_polygon_description::version()
{
  return 0;
}

void dborl_image_polygon_description::b_read()
{
  vcl_cout << "IMPLEMENT: dborl_image_polygon_description::b_read()\n";
}

void dborl_image_polygon_description::b_write()
{
  vcl_cout << "IMPLEMENT: dborl_image_polygon_description::b_write()\n";
}

void dborl_image_polygon_description::write_xml(vcl_ostream& os)
{
  for (vcl_map<vcl_string, vcl_vector<vsol_polygon_2d_sptr> >::iterator iter = data_.begin(); iter != data_.end(); iter++) {
    for (unsigned i = 0; i < (iter->second).size(); i++) {
      os << "\t\t<instance>\n";
      os << "\t\t\t<category>" << iter->first << "</category>\n";
      os << "\t\t\t<polygon>";
      vsol_polygon_2d_sptr poly = (iter->second)[i];
      for (unsigned j = 0; j < poly->size(); j ++) 
        os << poly->vertex(j)->x() << " " << poly->vertex(j)->y() << " ";
      os << "</polygon>\n";
      os << "\t\t</instance>\n";
    }
  }
}

