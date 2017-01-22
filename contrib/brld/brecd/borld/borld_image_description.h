//:
// \file
// \brief Base class for all image descriptions
//
// \author Ozge C Ozcanli (ozge@lems.brown.edu)
// \date 10/03/07
//      
// \verbatim
//   Modifications
//  
// \endverbatim

// see dborl/algo/dborl_image_desc_parser.h for xml read/write utilities
//


#if !defined(_DBORL_IMAGE_DESCRIPTION_H)
#define _DBORL_IMAGE_DESCRIPTION_H

#include "dborl_description_base.h"
#include <dborl/dborl_image_data_description_base_sptr.h>
#include <dborl/dborl_image_bbox_description_sptr.h>
#include <dborl/dborl_image_polygon_description_sptr.h>
#include <dborl/dborl_image_mask_description_sptr.h>
#include <vcl_vector.h>

class dborl_image_description : public dborl_description_base
{
public:
  dborl_image_data_description_base_sptr category_data_;

  dborl_image_description(dborl_image_bbox_description_sptr box_data);
  dborl_image_description(dborl_image_polygon_description_sptr poly_data);
  
  //: assuming the names in the vector are the names of the categories that exist in the mask 
  //  if the category names are repeated in the vector, then it means more than one instance from that category exists in the image
  //  the ids of the categories whose names are given should be retrieved from the dborl_category_info class when needed
  dborl_image_description(dborl_image_mask_description_sptr mask_data, vcl_vector<vcl_string>& categories);
  
  //: update version number whenever binary reader/writer is updated
  unsigned version();
  virtual void b_read();
  virtual void b_write();

  virtual void write_xml(vcl_ostream& os);

  unsigned get_object_type();

  virtual dborl_image_description* cast_to_image_description() { return this; }
};

#endif  //_DBORL_IMAGE_DESCRIPTION_H
