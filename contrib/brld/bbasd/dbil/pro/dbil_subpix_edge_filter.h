// This is basic/dbil/pro/dbil_subpix_edge_filter.h
#ifndef dbil_subpix_edge_filter_h_
#define dbil_subpix_edge_filter_h_

//:
// \file
// \brief Suppress all non-maximal edges and estimate subpixel locations
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 5/8/09
//
// \verbatim
//  Modifications
// \endverbatim

#include <vil/vil_new.h>
#include <vil/vil_plane.h>
#include <vil/algo/vil_suppress_non_max_edges.h>
#include <dbpro/dbpro_process.h>

//: Suppress all non-maximal edges and estimate subpixel locations
template <class srcT, class destT>
class dbil_subpix_edge_filter : public dbpro_filter
{
public:
  //: Constructor
  dbil_subpix_edge_filter(double mag_thresh=2.0, bool reuse_output=true) 
  : mag_thresh_(mag_thresh), reuse_output_(reuse_output) {}
  
  //: Execute this process
  dbpro_signal execute()
  {
    assert(input_type_id(0) == typeid(vil_image_resource_sptr));
    vil_image_resource_sptr in_img = input<vil_image_resource_sptr>(0);
    vil_image_view<srcT> image = in_img->get_view();
    vil_image_view<srcT> grad_i = vil_plane(image,0);
    vil_image_view<srcT> grad_j = vil_plane(image,1);
    
    if(!reuse_output_)
      grad_moo_ = vil_image_view<destT>();
    vil_suppress_non_max_edges_subpixel(grad_i,grad_j,mag_thresh_,grad_moo_);
    output(0, vil_new_image_resource_of_view(grad_moo_));
    return DBPRO_VALID;
  }

  double mag_thresh_;
  bool reuse_output_;
  vil_image_view<destT> grad_moo_;

};


#endif // dbil_subpix_edge_filter_h_
