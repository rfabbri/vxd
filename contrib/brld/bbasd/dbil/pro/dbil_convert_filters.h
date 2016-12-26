// This is basic/dbil/pro/dbil_convert_filters.h
#ifndef dbil_convert_filters_h_
#define dbil_convert_filters_h_

//:
// \file
// \brief Filters to convert an image formats
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 6/9/06
//
// \verbatim
//  Modifications
// \endverbatim

#include <vil/vil_convert.h>
#include <vil/vil_new.h>
#include <dbpro/dbpro_process.h>

//: Filter to cast the image to a new type
template <class outP>
class dbil_convert_cast_filter : public dbpro_filter
{
  public:
    //: Execute this process
    dbpro_signal execute()
    {
      assert(input_type_id(0) == typeid(vil_image_resource_sptr));
      vil_image_resource_sptr in_img = input<vil_image_resource_sptr>(0);

      output(0, vil_new_image_resource_of_view(*vil_convert_cast(outP(), in_img->get_view())));
      return DBPRO_VALID;
    }

};

//: Filter to cast the image to a new type
template <class inP>
class dbil_convert_stretch_range_filter : public dbpro_filter
{
  public:
    //: Execute this process
    dbpro_signal execute()
    {
      assert(input_type_id(0) == typeid(vil_image_resource_sptr));
      vil_image_resource_sptr in_img = input<vil_image_resource_sptr>(0);
      vil_image_view<inP> in = in_img->get_view();
      vil_image_view<vxl_byte> out;
      vil_convert_stretch_range(in, out);
      output(0, vil_new_image_resource_of_view(out));
      return DBPRO_VALID;
    }

};

#endif // dbil_convert_filters_h_
