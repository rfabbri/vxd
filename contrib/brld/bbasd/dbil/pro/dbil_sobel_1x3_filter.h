// This is basic/dbil/pro/dbil_sobel_1x3_filter.h
#ifndef dbil_sobel_1x3_filter_h_
#define dbil_sobel_1x3_filter_h_

//:
// \file
// \brief A Sobel 1x3 filter to produce image gradients
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 5/8/09
//
// \verbatim
//  Modifications
// \endverbatim

#include <vil/vil_new.h>
#include <vil/algo/vil_sobel_1x3.h>
#include <dbpro/dbpro_process.h>

//: A Sobel 1x3 filter to produce image gradients
// The output is a 2-plane image containing Ix and Iy
template <class srcT, class destT>
class dbil_sobel_1x3_filter : public dbpro_filter
{
public:
  //: Constructor
  dbil_sobel_1x3_filter(bool reuse_output=true) 
  : reuse_output_(reuse_output) {}
  
  //: Execute this process
  dbpro_signal execute()
  {
    assert(input_type_id(0) == typeid(vil_image_resource_sptr));
    vil_image_resource_sptr in_img = input<vil_image_resource_sptr>(0);
    vil_image_view<srcT> image = in_img->get_view();
    
    if(!reuse_output_)
      grad_ij_ = vil_image_view<destT>();
    vil_sobel_1x3(image,grad_ij_);
    output(0, vil_new_image_resource_of_view(grad_ij_));
    return DBPRO_VALID;
  }

  bool reuse_output_;
  vil_image_view<destT> grad_ij_;

};


#endif // dbil_sobel_1x3_filter_h_
