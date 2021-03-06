// This is basic/bild/pro/bild_convert_filters.h
#ifndef bild_convert_filters_h_
#define bild_convert_filters_h_

//:
// \file
// \brief Filters to convert an image formats
// \author Based on original code by  Matt Leotta (@lems.brown.edu)
// \date 6/9/06
//
// \verbatim
//  Modifications
// \endverbatim

#include <vil/vil_convert.h>
#include <vil/vil_new.h>
#include <bprod/bprod_process.h>

//: Filter to cast the image to a new type
template <class outP>
class bild_convert_cast_filter : public bprod_filter
{
  public:
    //: Execute this process
    bprod_signal execute()
    {
      assert(input_type_id(0) == typeid(vil_image_resource_sptr));
      vil_image_resource_sptr in_img = input<vil_image_resource_sptr>(0);

      output(0, vil_new_image_resource_of_view(*vil_convert_cast(outP(), in_img->get_view())));
      return BPROD_VALID;
    }

};

//: Filter to cast the image to a new type
template <class inP>
class bild_convert_stretch_range_filter : public bprod_filter
{
  public:
    //: Execute this process
    bprod_signal execute()
    {
      assert(input_type_id(0) == typeid(vil_image_resource_sptr));
      vil_image_resource_sptr in_img = input<vil_image_resource_sptr>(0);
      vil_image_view<inP> in = in_img->get_view();
      vil_image_view<vxl_byte> out;
      vil_convert_stretch_range(in, out);
      output(0, vil_new_image_resource_of_view(out));
      return BPROD_VALID;
    }

};

#endif // bild_convert_filters_h_
