// This is file dborl/dborl_image_description_utils.h

#ifndef dborl_image_description_utils_h_
#define dborl_image_description_utils_h_

//:
// \file
// \brief Collection of useful functions to apply to image description
//
// \author Nhon Trinh (ntrinh@lems.brown.edu)
// \date Nov 6, 2008
//      
// \verbatim
//   Modifications
//  
// \endverbatim



#include <dborl/dborl_image_description_sptr.h>
#include <vsol/vsol_box_2d_sptr.h>
#include <vcl_string.h>
#include <vcl_vector.h>

//------------------------------------------------------------------------------
//: extract bounding boxes of objects belong to a particular category from an 
// image description. Return false if image description is not defined.
bool dborl_get_boxes(vcl_vector<vsol_box_2d_sptr>& boxes, 
                     const dborl_image_description_sptr& desc, const vcl_string& model_category);


#endif  //dborl_image_description_utils_h_
