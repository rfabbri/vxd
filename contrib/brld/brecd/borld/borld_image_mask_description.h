//:
// \file
// \brief Class that ..
//
// \author Ozge C Ozcanli (ozge@lems.brown.edu)
// \date 10/03/07
//      
// \verbatim
//   Modifications
//  
// \endverbatim

//
// see dborl/algo/dborl_image_desc_parser.h for xml read/write utilities
//

#if !defined(_DBORL_IMAGE_MASK_DESCRIPTION_H)
#define _DBORL_IMAGE_MASK_DESCRIPTION_H

#include "dborl_image_data_description_base.h"

#include <vil/vil_image_resource_sptr.h>

class dborl_image_mask_description : public dborl_image_data_description_base
{
public:
  vil_image_resource_sptr mask_;

  dborl_image_mask_description(vil_image_resource_sptr m) : mask_(m) {}
  dborl_image_mask_description() {}

  vil_image_resource_sptr get_mask() { return mask_; }
  void set_mask(vil_image_resource_sptr m) { mask_ = m; }

  virtual unsigned get_type() { return dborl_image_data_description_type::category_mask; }

  //: update version number whenever binary reader/writer is updated
  virtual unsigned version();
  virtual void b_read();
  virtual void b_write();

  //: nothing to write to the xml file --> just empty implementation to prevent abstract class instantiation error (error C2259)
  virtual void write_xml(vcl_ostream& os);
  
  virtual dborl_image_bbox_description* cast_to_image_bbox_description() { return 0; }
  virtual dborl_image_mask_description* cast_to_image_mask_description() { return this; }
  virtual dborl_image_polygon_description* cast_to_image_polygon_description() { return 0; }
};

#endif  //_DBORL_IMAGE_MASK_DESCRIPTION_H
