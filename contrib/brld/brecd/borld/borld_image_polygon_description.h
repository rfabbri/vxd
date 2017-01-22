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

#if !defined(_DBORL_IMAGE_POLYGON_DESCRIPTION_H)
#define _DBORL_IMAGE_POLYGON_DESCRIPTION_H

#include "dborl_image_data_description_base.h"

#include <vsol/vsol_polygon_2d_sptr.h>

class dborl_image_polygon_description : public dborl_image_data_description_base
{
public:
  vcl_map<vcl_string, vcl_vector<vsol_polygon_2d_sptr> > data_;

  virtual unsigned get_type() { return dborl_image_data_description_type::polygonal_outline; }

  void add_polygon(vcl_string cat, vsol_polygon_2d_sptr p);
  vcl_vector<vsol_polygon_2d_sptr>& get_polygon_vector(vcl_string cat);

  bool category_exists(vcl_string cat) { return data_.find(cat) != data_.end(); }

  vcl_map<vcl_string, vcl_vector<vsol_polygon_2d_sptr> >& get_category_map() { return data_; }

  //: update version number whenever binary reader/writer is updated
  virtual unsigned version();
  virtual void b_read();
  virtual void b_write();
  virtual void write_xml(vcl_ostream& os);

  virtual dborl_image_bbox_description* cast_to_image_bbox_description() { return 0; }
  virtual dborl_image_mask_description* cast_to_image_mask_description() { return 0; }
  virtual dborl_image_polygon_description* cast_to_image_polygon_description() { return this; }
};

#endif  //_DBORL_IMAGE_POLYGON_DESCRIPTION_H
