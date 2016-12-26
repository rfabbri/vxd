// This is basic/dbil/pro/dbil_transform3_1_filter.h
#ifndef dbil_transform3_1_filter_h_
#define dbil_transform3_1_filter_h_

//:
// \file
// \brief Apply a function of 3 variables to map 3 planes into 1
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 5/8/09
//
// \verbatim
//  Modifications
// \endverbatim

#include <vil/vil_new.h>
#include <dbpro/dbpro_process.h>

//: Apply a function of 3 variables to map 3 planes into 1
template <class srcT, class destT, class Op>
class dbil_transform3_1_filter : public dbpro_filter
{
public:
  //: Constructor
  dbil_transform3_1_filter(Op functor, bool reuse_output=true) 
  : functor_(functor), reuse_output_(reuse_output) {}
  
  //: Execute this process
  dbpro_signal execute()
  {
    assert(input_type_id(0) == typeid(vil_image_resource_sptr));
    vil_image_resource_sptr in_img = input<vil_image_resource_sptr>(0);
    vil_image_view<srcT> image = in_img->get_view();
    assert(image.nplanes() == 3);
    
    const unsigned int ni = image.ni();
    const unsigned int nj = image.nj();
    
    if(!reuse_output_)
      out_img_ = vil_image_view<destT>();
    out_img_.set_size(ni,nj,1);
    
    vcl_ptrdiff_t istep_in=image.istep(), jstep_in=image.jstep(); 
    vcl_ptrdiff_t pstep_in=image.planestep();
    vcl_ptrdiff_t istep_out=out_img_.istep(), jstep_out=out_img_.jstep();
    const srcT* row_in = image.top_left_ptr();
    destT* row_out = out_img_.top_left_ptr();
    
    for (unsigned j=0; j<nj; ++j, row_in += jstep_in, row_out += jstep_out)
    {
      const srcT* pixel_in = row_in;
      destT* pixel_out = row_out;
      for (unsigned i=0; i<ni; ++i, pixel_in+=istep_in, pixel_out+=istep_out){
        const srcT& a = *pixel_in;
        const srcT& b = *(pixel_in+pstep_in);
        const srcT& c = *(pixel_in+2*pstep_in);
        *pixel_out = functor_(a,b,c);
      }
    }
    
    output(0, vil_new_image_resource_of_view(out_img_));
    return DBPRO_VALID;
  }

  Op functor_;
  bool reuse_output_;
  vil_image_view<destT> out_img_;

};


#endif // dbil_transform3_1_filter_h_
