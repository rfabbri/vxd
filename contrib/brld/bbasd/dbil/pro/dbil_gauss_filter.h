// This is basic/dbil/pro/dbil_gauss_filter.h
#ifndef dbil_gauss_filter_h_
#define dbil_gauss_filter_h_

//:
// \file
// \brief A Gaussian smoothing filter
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 5/8/09
//
// \verbatim
//  Modifications
// \endverbatim

#include <vil/vil_new.h>
#include <vil/algo/vil_gauss_filter.h>
#include <dbpro/dbpro_process.h>

//: Filter to smooth with a Gaussian
template <class srcT, class destT>
class dbil_gauss_filter : public dbpro_filter
{
public:
  //: Constructor
  dbil_gauss_filter(double sigma=1.0, bool reuse_output=true) 
  : gp_(sigma), reuse_output_(reuse_output) {}
  
  //: Execute this process
  dbpro_signal execute()
  {
    assert(input_type_id(0) == typeid(vil_image_resource_sptr));
    vil_image_resource_sptr in_img = input<vil_image_resource_sptr>(0);
    vil_image_view<srcT> image = in_img->get_view();
    
    if(!reuse_output_)
      smooth_ = vil_image_view<destT>();
    vil_gauss_filter_5tap(image,smooth_,gp_,work_);
    output(0, vil_new_image_resource_of_view(smooth_));
    return DBPRO_VALID;
  }

  vil_gauss_filter_5tap_params gp_;
  bool reuse_output_;
  vil_image_view<destT> work_;
  vil_image_view<destT> smooth_;

};


#endif // dbil_gauss_filter_h_
