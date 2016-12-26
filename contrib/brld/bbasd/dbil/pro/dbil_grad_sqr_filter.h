// This is basic/dbil/pro/dbil_grad_sqr_filter.h
#ifndef dbil_grad_sqr_filter_h_
#define dbil_grad_sqr_filter_h_

//:
// \file
// \brief Compute the squares and cross terms of gradient.
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 5/8/09
//
// \verbatim
//  Modifications
// \endverbatim

#include <vil/vil_new.h>
#include <vil/vil_plane.h>
#include <vil/vil_math.h>
#include <dbpro/dbpro_process.h>


//: Compute the squares and cross terms of gradient.
// This is most often used in computing interest points.
// The input is a 2-plane image containing Ix and Iy
// The output is a 3-plane image containing Ix^2, Iy^2, and Ix*Iy
template <class srcT, class destT>
class dbil_grad_sqr_filter : public dbpro_filter
{
public:
  //: Constructor
  dbil_grad_sqr_filter(bool reuse_output=true) 
  : reuse_output_(reuse_output) {}
  
  class sqr_functor
  {
  public:
    destT operator()(const srcT& x) const { return destT(x*x); }
  };
  
  //: Execute this process
  dbpro_signal execute()
  {
    assert(input_type_id(0) == typeid(vil_image_resource_sptr));
    vil_image_resource_sptr in_img = input<vil_image_resource_sptr>(0);
    vil_image_view<srcT> image = in_img->get_view();
    vil_image_view<srcT> grad_i = vil_plane(image,0);
    vil_image_view<srcT> grad_j = vil_plane(image,1);
    
    if(!reuse_output_)
      grad_sqr_ = vil_image_view<destT>();
    
    grad_sqr_.set_size(image.ni(),image.nj(),3);
    vil_image_view<destT> grad_ii = vil_plane(grad_sqr_,0);
    vil_image_view<destT> grad_jj = vil_plane(grad_sqr_,1);
    vil_image_view<destT> grad_ij = vil_plane(grad_sqr_,2);
    
    vil_transform(grad_i,grad_ii,sqr_functor());
    vil_transform(grad_j,grad_jj,sqr_functor());
    vil_math_image_product(grad_i,grad_j,grad_ij);
    
    output(0, vil_new_image_resource_of_view(grad_sqr_));
    return DBPRO_VALID;
  }

  bool reuse_output_;
  vil_image_view<destT> grad_sqr_;

};


#endif // dbil_grad_sqr_filter_h_
