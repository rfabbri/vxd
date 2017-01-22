//:
// \file


#include "dborl/dborl_image_description_utils.h"

#include <vsol/vsol_box_2d.h>
#include <vsol/vsol_polygon_2d.h>
#include <dborl/dborl_image_description.h>
#include <dborl/dborl_image_bbox_description.h>
#include <dborl/dborl_image_polygon_description.h>



//------------------------------------------------------------------------------
//: extract bounding boxes of objects belong to a particular category from an 
// image description. Return false if image description is not defined.
bool dborl_get_boxes(vcl_vector<vsol_box_2d_sptr>& boxes, 
                     const dborl_image_description_sptr& desc, const vcl_string& model_category)
{ 
  boxes.clear();
  vcl_vector<vsol_box_2d_sptr> temp;
  if (desc->category_data_->cast_to_image_bbox_description()) {
    if (desc->category_data_->cast_to_image_bbox_description()->category_exists(model_category)) {
      temp = desc->category_data_->cast_to_image_bbox_description()->get_box_vector(model_category);
      boxes.insert(boxes.begin(), temp.begin(), temp.end());
    }
    
  } else if (desc->category_data_->cast_to_image_polygon_description()) {
    //vcl_cout << "image polygon desc\n";

    if (desc->category_data_->cast_to_image_polygon_description()->category_exists(model_category)) {
      vcl_vector<vsol_polygon_2d_sptr>& polys = desc->category_data_->cast_to_image_polygon_description()->get_polygon_vector(model_category);
      //vcl_cout << "model_cat: " << model_category << " size of polys: " << polys.size() << "\n";
      for (unsigned i = 0; i < polys.size(); i++) {
        polys[i]->compute_bounding_box();
        boxes.push_back(polys[i]->get_bounding_box());
      }
    }
  } else {
    vcl_cout << "WARNING: description boxes could not be determined!!!!\n";
    return false;
  }
  return true;
}

