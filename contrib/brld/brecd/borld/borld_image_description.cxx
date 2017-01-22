//:
// \file
// \brief 
// \author     Ozge Can Ozcanli (ozge@lems.brown.edu)
// \date        010/03/07
#include "dborl_image_description.h"
#include <dborl/dborl_image_data_description_base.h>
#include <dborl/dborl_image_bbox_description.h>
#include <dborl/dborl_image_polygon_description.h>
#include <dborl/dborl_image_mask_description.h>

#include <vcl_iostream.h>

dborl_image_description::dborl_image_description(dborl_image_bbox_description_sptr box_data)
{
  vcl_map<vcl_string, vcl_vector<vsol_box_2d_sptr> >& map = box_data->get_category_map();

  //: add each category and counts one by one
  for (vcl_map<vcl_string, vcl_vector<vsol_box_2d_sptr> >::iterator i = map.begin(); i != map.end(); i++) 
    category_list_[i->first] = (i->second).size();

  category_data_ = box_data->cast_to_image_data_description_base();
}
dborl_image_description::dborl_image_description(dborl_image_polygon_description_sptr poly_data)
{
  vcl_map<vcl_string, vcl_vector<vsol_polygon_2d_sptr> >& map = poly_data->get_category_map();

  //: add each category and counts one by one
  for (vcl_map<vcl_string, vcl_vector<vsol_polygon_2d_sptr> >::iterator i = map.begin(); i != map.end(); i++) 
    category_list_[i->first] = (i->second).size();

  category_data_ = poly_data->cast_to_image_data_description_base();
}

//: assuming the names in the vector are names of categories for corresponding indices 
//  (the vector may contain way more cats than that exists in the mask)
dborl_image_description::dborl_image_description(dborl_image_mask_description_sptr mask_data, vcl_vector<vcl_string>& cats)
{
  category_data_ = mask_data->cast_to_image_data_description_base();
  for (unsigned i = 0; i < cats.size(); i++) {
    if (category_exists(cats[i]))
      add_to_category_cnt(cats[i], 1);  // increments the count by 1
    else
      add_category(cats[i]);  // sets the count to 1
  }
}

unsigned dborl_image_description::version()
{
  return 0;
}

void dborl_image_description::b_read()
{
  vcl_cout << "IMPLEMENT: dborl_image_description::b_read()\n";
}

void dborl_image_description::b_write()
{
  vcl_cout << "IMPLEMENT: dborl_image_description::b_write()\n";
}

unsigned dborl_image_description::get_object_type()
{
  return dborl_object_type::image;
}

void dborl_image_description::write_xml(vcl_ostream& os)
{
  os << "<type name = \"image\">\n";
  os << "\t<description>\n";

  if (category_data_->cast_to_image_polygon_description()) {
    category_data_->cast_to_image_polygon_description()->write_xml(os); 
  } else if (category_data_->cast_to_image_bbox_description()) {
    category_data_->cast_to_image_bbox_description()->write_xml(os); 
  } else if (category_data_->cast_to_image_mask_description()) {
      for (vcl_map<vcl_string, int>::iterator iter = category_list_.begin(); iter != category_list_.end(); iter++) {
        for (int i = 0; i < iter->second; i++) {
          os << "\t\t<instance>\n";
          os << "\t\t\t<category>" << iter->first << "</category>\n";
          os << "\t\t</instance>\n";
        }
      }
  }

  os << "\t</description>\n";
  os << "</type>\n";
}

