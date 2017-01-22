// This is file borld/borld_image_description_utils.h

#ifndef borld_image_description_utils_h_
#define borld_image_description_utils_h_

//:
// \file
// \brief Collection of useful functions to apply to image description
//
// \author Based on original code by  Nhon Trinh (@lems.brown.edu)
// \date Nov 6, 2008
//      
// \verbatim
//   Modifications
//  
// \endverbatim



#include <borld/borld_image_description_sptr.h>
#include <vsol/vsol_box_2d_sptr.h>
#include <vcl_string.h>
#include <vcl_vector.h>

//------------------------------------------------------------------------------
//: extract bounding boxes of objects belong to a particular category from an 
// image description. Return false if image description is not defined.
bool borld_get_boxes(vcl_vector<vsol_box_2d_sptr>& boxes, 
                     const borld_image_description_sptr& desc, const vcl_string& model_category);


#endif  //borld_image_description_utils_h_
