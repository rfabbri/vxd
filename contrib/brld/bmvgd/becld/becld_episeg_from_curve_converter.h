// This is brcv/mvg/dbecl/dbecl_episeg_from_curve_converter.h
#ifndef dbecl_episeg_from_curve_converter_h_
#define dbecl_episeg_from_curve_converter_h_

//:
// \file
// \brief  Converts a vsol_digital_curve_2d to a set of episegments
// \author Based on original code by  dapachec
// \date   8/25/2004

#include <vcl_vector.h>

#include <vnl/vnl_math.h>
#include <vbl/vbl_ref_count.h>
#include <vbl/vbl_smart_ptr.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_digital_curve_2d.h>
#include <vsol/vsol_digital_curve_2d_sptr.h>
#include <bbld/bbld_subsequence.h>
#include <dbgl/dbgl_dist.h>

#include <dbecl/dbecl_epipole.h>
#include <dbecl/dbecl_epipole_sptr.h>
#include <dbecl/dbecl_episeg_sptr.h>

//: Converts a vsol_digital_curve_2d to a set of episegments
class dbecl_episeg_from_curve_converter : public vbl_ref_count {
public:
  //: Constructor takes an epipole
  dbecl_episeg_from_curve_converter(dbecl_epipole_sptr);
  
  //: Convert a digital curve to episegments
  vcl_vector<dbecl_episeg_sptr> convert_curve(vsol_digital_curve_2d_sptr);

  //: Minimum angle that an element of each episeg does with the epipolar lines.
  // To be used in of convert_curve_using_tangents.
  // The value is in radians.
  void set_tangent_threshold(double tau_dtheta)
  { delta_theta_ = tau_dtheta; }

  //: Convert a digital curve to episegments
  // \param[in] tangents: tangent direction for each sampe of the digital curve.
  // The angle is in radians between [0,2*vnl_math::pi). It is taken to
  // represent an undirected tangent direction at the sample point; it thus can
  // be in any range, but the range [0,vnl_math::pi) is enough to represent this
  // information
  // 
  // \return vcl_vector with partition into episegs for which all tangents with
  // epipolar lines are either greater or smaller than or equal \p tau_dtheta as
  // set in set_tangent_threshold.
  vcl_vector<dbecl_episeg_sptr> convert_curve_using_tangents(
      vsol_digital_curve_2d_sptr curve,
      vcl_vector<double> tangents)
  {
    bbld_subsequence_set partition;
    return convert_curve_using_tangents(curve, tangents, &partition);
  }

  //: Overloaded version to output a bbld_subsequence representation of the
  // episegs which is solely based on indexing.
  vcl_vector<dbecl_episeg_sptr>
  convert_curve_using_tangents(
      vsol_digital_curve_2d_sptr curve,
      vcl_vector<double> tangents,
      bbld_subsequence_set *partition,
      unsigned curve_id=0);
  
protected:
  //: The epipole
  dbecl_epipole_sptr epipole_;
  double delta_theta_;

  //: Get the epipolar angle of the given point on the curve
  double angle(vsol_digital_curve_2d_sptr curve, int idx) const;

  //: Link the episegs into a linked list
  void link_episegs(vcl_vector<dbecl_episeg_sptr>& segs) const;
};

// Functor object(i) == true if angle between the epipolar line through
// curve->vertex(i) and the tangent at i is more than a threshold.
// 
// The user should get rid of this object right after using it, as it keeps
// internal references to stack-allocated objects of the calling envirnoment.
//
class dbecl_delta_angle_predicate : public vcl_unary_function<unsigned, bool> {
public:
  //: delta_theta is effective if it is between 0 and vnl_math::pi/2. 
  dbecl_delta_angle_predicate(
      const vsol_digital_curve_2d_sptr &curve,
      const vcl_vector<double> &tangents,
      const dbecl_epipole_sptr &epipole, 
      double delta_theta)
    :
      curve_(curve),
      tangents_(tangents),
      epipole_(epipole),
      tau_delta_theta_(delta_theta)
  { }

  bool operator() (unsigned i) const {
    double epiangle = epipole_->angle(curve_->point(i)->get_p());

    // Epiangle is between -pi and pi. Make it between 0 and pi
    if (epiangle < 0)
      epiangle += vnl_math::pi;

    return dbgl_undirected_angle_distance(tangents_[i], epiangle) > tau_delta_theta_;
  }

private:
  vsol_digital_curve_2d_sptr curve_;
  const vcl_vector<double> &tangents_;
  dbecl_epipole_sptr epipole_;
  double tau_delta_theta_;
};

#endif // dbecl_episeg_from_curve_converter_h_
