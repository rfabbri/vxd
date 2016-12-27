// This is basic/bild/pro/bild_math_filters.h
#ifndef bild_math_filters_h_
#define bild_math_filters_h_

//:
// \file
// \brief Filters to apply vil_math operations
// \author Based on original code by  Matt Leotta (@lems.brown.edu)
// \date 6/9/06
//
// \verbatim
//  Modifications
// \endverbatim

#include <vil/vil_math.h>
#include <vil/vil_new.h>
#include <bprod/bprod_process.h>

//: Filter to cast the image to a new type
// Warning this filter modifies the input data
template <class T>
class bild_math_scale_filter : public bprod_filter
{
  public:
    //: Constructor
    bild_math_scale_filter(double scale) : scale_(scale) {}
    //: Execute this process
    bprod_signal execute()
    {
      assert(input_type_id(0) == typeid(vil_image_resource_sptr));
      vil_image_resource_sptr in_img = input<vil_image_resource_sptr>(0);
      vil_image_view<T> image = in_img->get_view();
      vil_math_scale_values(image,scale_);
      output(0, in_img);
      return BPROD_VALID;
    }
  private:
    double scale_;

};


//: Filter to cast the image to a new type
// Warning this filter modifies the input data
template <class T>
class bild_math_scale_and_offset_filter : public bprod_filter
{
  public:
    //: Constructor
    bild_math_scale_and_offset_filter(double scale, double offset)
      : scale_(scale), offset_(offset) {}
    //: Execute this process
    bprod_signal execute()
    {
      assert(input_type_id(0) == typeid(vil_image_resource_sptr));
      vil_image_resource_sptr in_img = input<vil_image_resource_sptr>(0);
      vil_image_view<T> image = in_img->get_view();
      vil_math_scale_and_offset_values(image,scale_,offset_);
      output(0, in_img);
      return BPROD_VALID;
    }
  private:
    double scale_;
    double offset_;

};

#endif // bild_math_filters_h_
