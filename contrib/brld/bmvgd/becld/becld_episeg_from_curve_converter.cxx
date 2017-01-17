// This is bmvgd/becld/becld_episeg_from_curve_converter.cxx
//:
// \file

#include "becld_episeg_from_curve_converter.h"

#include "becld_episeg.h"

static const double uninitialized_delta_theta = -11091283;

//: Constructor takes an epipole
becld_episeg_from_curve_converter::becld_episeg_from_curve_converter(becld_epipole_sptr e) :
  epipole_(e),
  delta_theta_(uninitialized_delta_theta)
{}

//: Convert a digital curve to an episegment
vcl_vector<becld_episeg_sptr>
becld_episeg_from_curve_converter::convert_curve(vsol_digital_curve_2d_sptr curve)
{
  vcl_vector<becld_episeg_sptr> to_return;

  // We need at least 2 points to make an episegment
  if(curve->size() < 2)
    return to_return;

  double prev_angle = angle(curve,0);
  double curr_angle = angle(curve,1);
  // find the first differing angle (usually idx=1)
  unsigned int idx=1;
  while( prev_angle == curr_angle ){ 
    // if we hit the end of the curve we are done
    if( ++idx >= curve->size() )
      return to_return;
    curr_angle = angle(curve,idx);
  }

  bool dir = curr_angle > prev_angle;
  unsigned int min = idx-1;

  prev_angle = curr_angle;
  for (idx +=1; idx<curve->size(); ++idx) {
    curr_angle = angle(curve,idx);

    // Perfectly epi-colinear segments are very rare, but do occur.
    if ( curr_angle == prev_angle ) {
      to_return.push_back(new becld_episeg(epipole_, curve, min, idx-1));
      while( prev_angle == curr_angle ){ 
        // if we hit the end of the curve we are done
        if( ++idx >= curve->size() ){
          link_episegs(to_return);
          return to_return;
        }
        curr_angle = angle(curve,idx);
      } 
      min = idx-1;
      dir = curr_angle > prev_angle;
    }

    // if tangent to an epipolar line at this point start a new episeg
    else if (dir != (curr_angle > prev_angle) ) {
      to_return.push_back(new becld_episeg(epipole_, curve, min, idx-1));
      min = idx-1;
      dir = !dir;
    }
    prev_angle = curr_angle;
  }
  to_return.push_back(new becld_episeg(epipole_, curve, min, idx-1));

  link_episegs(to_return);
  return to_return;
}

vcl_vector<becld_episeg_sptr>
becld_episeg_from_curve_converter::convert_curve_using_tangents(
    vsol_digital_curve_2d_sptr curve,
    vcl_vector<double> tangents,
    bbld_subsequence_set *partition,
    unsigned curve_id
    )
{
  assert(tangents.size() == curve->size());
  assert(delta_theta_ != uninitialized_delta_theta);
  vcl_vector<becld_episeg_sptr> to_return;

  // We need at least 1 points to make an episegment
  if (curve->size() < 1)
    return to_return;

  bbld_subsequence_set &ss = *partition;
  bbld_subsequence original_seq(0, curve->size(), curve_id);

  becld_delta_angle_predicate 
    is_angle_acceptable(curve, tangents, epipole_, delta_theta_);

  bbld_contiguous_partition(original_seq, is_angle_acceptable, &ss);
  assert(!curve->size() || ss.num_subsequences());

  for (unsigned i=0; i < ss.num_subsequences(); ++i) {
    // if ( ss[i].size() && is_angle_acceptable(ss[i].ini()) )
      to_return.push_back(new becld_episeg(epipole_, curve, ss[i].ini(), 
            static_cast<double>(ss[i].end())-1));
  }

  link_episegs(to_return);
  return to_return;
}


//: Get the epipolar angle of the given point on the curve
double
becld_episeg_from_curve_converter::angle(vsol_digital_curve_2d_sptr curve, int idx) const
{
  return epipole_->angle(curve->point(idx)->get_p());
}


//: Link the episegs into a linked list
void 
becld_episeg_from_curve_converter::link_episegs(vcl_vector<becld_episeg_sptr>& segs) const
{
  if(segs.size() < 2)
    return;

  for( vcl_vector<becld_episeg_sptr>::iterator itr = segs.begin();
       itr+1 != segs.end();  ++itr )
  {
    (*itr)->next_seg_ = (itr+1)->ptr();
    (*(itr+1))->prev_seg_ = itr->ptr();
  }
}
