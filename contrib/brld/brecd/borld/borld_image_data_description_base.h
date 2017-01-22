//:
// \file
// \brief Base class for image description data
//
// \author Ozge C Ozcanli (ozge@lems.brown.edu)
// \date 10/03/07
//      
// \verbatim
//   Modifications
//  
// \endverbatim


#if !defined(_DBORL_IMAGE_DATA_DESCRIPTION_BASE_H)
#define _DBORL_IMAGE_DATA_DESCRIPTION_BASE_H

#include <vbl/vbl_ref_count.h>
#include <vcl_vector.h>
#include <vcl_map.h>
#include <vcl_string.h>
#include <vcl_iostream.h>

class dborl_image_data_description_type
{
public:
  enum type {
    category_mask,
    bounding_box,
    polygonal_outline
  };
};

class dborl_image_bbox_description;
class dborl_image_mask_description;
class dborl_image_polygon_description;

class dborl_image_data_description_base : public vbl_ref_count
{
public: 
  virtual unsigned get_type() = 0;

  virtual unsigned version() = 0;
  virtual void b_read() = 0;
  virtual void b_write() = 0;
  virtual void write_xml(vcl_ostream& os) = 0;

  virtual dborl_image_data_description_base* cast_to_image_data_description_base() { return this; }
  virtual dborl_image_bbox_description* cast_to_image_bbox_description() = 0;
  virtual dborl_image_mask_description* cast_to_image_mask_description() = 0;
  virtual dborl_image_polygon_description* cast_to_image_polygon_description() = 0;
};

#endif  //_DBORL_IMAGE_DATA_DESCRIPTION_BASE_H
