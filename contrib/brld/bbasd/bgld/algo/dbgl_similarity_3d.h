// This is basic/dbgl/algo/dbgl_similarity_3d.h
#ifndef dbgl_similarity_3d_h_
#define dbgl_similarity_3d_h_
//:
// \file
// \brief A class representing a 3d similarity tranform.
// \author Matt Leotta
// \date 03/12/08
//
// This is a class maintaining and applying similarity transformations
// in 3 dimensions.
//


#include <vnl/vnl_vector_fixed.h>
#include <vnl/vnl_vector_fixed_ref.h>
#include <vnl/vnl_matrix_fixed.h>
#include <vnl/vnl_double_3.h>
#include <vnl/vnl_inverse.h>
#include <vnl/vnl_cross.h>
#include <vnl/vnl_trace.h>
#include <vcl_cmath.h>
#include <vgl/vgl_fwd.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_vector_3d.h>
#include <vgl/vgl_homg_point_3d.h>
#include <vgl/algo/vgl_rotation_3d.h>
#include <vgl/algo/vgl_h_matrix_3d.h>
#include <vgl/vgl_tolerance.h>
#include <vcl_vector.h>
#include <vcl_iostream.h>

template <class T>
class dbgl_similarity_3d
{
 public:
  // Constructors:-------------------------------------

  //: Construct the identity transformation
  dbgl_similarity_3d() : s_(1), R_(), t_(0,0,0) {}

  //: Construct from scale, rotation, and translation
  dbgl_similarity_3d( const T& s, const vgl_rotation_3d<T>& R,
                      const vgl_vector_3d<T>& t ) : s_(s), R_(R), t_(t) {}
  
  //: Construct from scale, rotation, and translation
  dbgl_similarity_3d( const vnl_vector_fixed<T,7>& la )
  : s_(vcl_exp(la[0])), R_(la.extract(3,1))
    {
      const double& ls = la[0];
      vnl_vector_fixed_ref_const<double,3> r(la.data_block()+1);
      vnl_vector_fixed_ref_const<double,3> t(la.data_block()+4);
      T mag = ls*ls + r.squared_magnitude();
      // Avoid divide by zero when no rotation and no scale
      if(mag == T(0))
        t_.set(t[0],t[1],t[2]);
      else{
        vnl_vector_fixed<T,3> v(ls*t);
        v -= vnl_cross_3d(r,t);
        v = s_*(R_*v) - v;
        // Avoid divide by zero using L'Hospital rule
        if(ls == T(0))
          v += dot_product(r,t)*r;
        else
          v += (s_-1)/ls * dot_product(r,t)*r;
        v /= mag;
        t_.set(v[0],v[1],v[2]);
      }
    }


  // Accessors:--------------------------------------

    //: Returns the rotation object
  vgl_rotation_3d<T> rotation() const
    {
      return R_;
    }
    
  //: Returns the translation vector
  vgl_vector_3d<T> translation() const
    {
      return t_;
    }

  //: Returns the scale
  T scale() const
    {
      return s_;
    }


  // Modifiers:--------------------------------------

  //: Sets the rotation object
  void set_rotation(const vgl_rotation_3d<T>& R)
    {
      R_ = R;
    }

  //: Sets the translation vector
  void set_translation(const vgl_vector_3d<T>& t)
    {
      t_ = t;
    }

  //: Sets the scale
  void set_scale(const T& s)
    {
      s_ = s;
    }

  // Conversions:--------------------------------------

  //: Output the 4x4 matrix representation of this transformation.
  vnl_matrix_fixed<T,4,4> as_matrix() const
    {
      vnl_matrix_fixed<T,4,4> Tfrm(0.0);
      Tfrm.update(s_*R_.as_matrix());
      Tfrm.set_column(3,vnl_vector_fixed<T,4>(t_.x(),t_.y(),t_.z(),1));
      return Tfrm;
    }

  //: Output the matrix representation of this rotation in 4x4 form.
  vgl_h_matrix_3d<T> as_h_matrix_3d() const
    {
      return vgl_h_matrix_3d<T>(as_matrix());
    }

  // Lie Algebra:---------------------------------------

  //: Compute the matrix A such that A*log(S).t = S.t
  // This matrix is not used directly since it is more effecient
  // to compute the transformed vector directly
  vnl_matrix_fixed<T,3,3> lie_exp_t_matrix() const
    {
      vnl_matrix_fixed<T,3,3> A;

      double ls = vcl_log(s_);
      vnl_vector_fixed<double,3> r = R_.as_rodrigues();
      T mag = ls*ls + r.squared_magnitude();
      // Avoid divide by zero when no rotation and no scale
      if(mag == T(0)){
        A.set_identity();
        return A;
      }

      A = s_*R_.as_matrix();
      A(0,0) -= 1.0; A(1,1) -= 1.0; A(2,2) -= 1.0;

      double aIw[] = {ls, r[2], -r[1], -r[2], ls, r[0], r[1], -r[0], ls};
      A *= vnl_matrix_fixed<T,3,3>(aIw);

      // Avoid divide by zero using L'Hospital rule
      if(ls == T(0))
        A += outer_product(r,r);
      else
        A += ((s_-1)/ls) * outer_product(r,r);

      A /= mag;
      return A;
    }


  //: Compute the matrix A such that A*S.t = log(S).t
  // This matrix is not used directly since it is more effecient
  // to compute the transformed vector directly
  vnl_matrix_fixed<T,3,3> lie_log_t_matrix() const
    {
      vnl_matrix_fixed<T,3,3> A;

      vnl_vector_fixed<T,3> r = R_.as_rodrigues();

      vnl_quaternion<T> q(R_.as_quaternion());
      // compute the trace of the R matrix
      double trace = 3.0*q.r()*q.r() - q.x()*q.x() - q.y()*q.y() - q.z()*q.z();

      // Avoid divide by zero in the scale == 1 case
      // use L'Hospital
      if(s_ == T(1.0)){
        // Avoid another divide by zero
        if(trace == T(3.0)){
          A.set_identity();
          return A;
        }
        double r2 = r.squared_magnitude();
        double c2 = 1.0/r2 - 0.5*vcl_sqrt((1+trace)/((3-trace)*r2));
        double w[] = {0, -r[2], r[1], r[2], 0, -r[0], -r[1], r[0], 0};
        vnl_matrix_fixed<T,3,3> wx(w);
        A = c2*wx*wx - 0.5*wx;
        A(0,0) += 1; A(1,1) += 1; A(2,2) += 1;
      }
      else{
        double ls = vcl_log(s_);
        double s_2 = s_*s_;
        double s_3 = s_2*s_;
        vnl_matrix_fixed<T,3,3> R(R_.as_matrix());
        A = s_2*R.transpose();
        A += s_*R;;
        double t1 = s_*trace-1;
        A(0,0) -= t1; A(1,1) -= t1; A(2,2) -= t1;
        double aIw[] = {ls, -r[2], r[1], r[2], ls, -r[0], -r[1], r[0], ls};
        A *= vnl_matrix_fixed<double,3,3>(aIw);
        // this division is safe since the only real root of this polynomial
        // is s = 1 for all valid rotation traces
        A /= s_3 + trace*(s_ - s_2) -1;
      }

      return A;
    }

  //: Compute the basis vector in the Lie algebra
  vnl_vector_fixed<T,7> lie_algebra_basis() const
    {
      vnl_vector_fixed<T,7> s;
      vnl_vector_fixed<T,3> r = R_.as_rodrigues();
      vnl_vector_fixed<T,3> t;
      t[0]=t_.x(); t[1]=t_.y(); t[2]=t_.z();
      s[0] = vcl_log(s_);
      s.update(r,1);

      vnl_quaternion<T> q(R_.as_quaternion());
      // compute the trace of the R matrix
      double trace = 3.0*q.r()*q.r() - q.x()*q.x() - q.y()*q.y() - q.z()*q.z();

      // Avoid divide by zero in the scale == 1 case
      // use L'Hospital
      if(s_ == T(1.0)){
        // Avoid another divide by zero
        if(trace == T(3.0))
          s.update(t,4);
        else{
          vcl_cout << "using s_==1.0 case" <<vcl_endl;
          vnl_vector_fixed<T,3> r_x_t(vnl_cross_3d(r,t));
          double r2 = r.squared_magnitude();
          double c2 = 1.0/r2 - 0.5*vcl_sqrt((1+trace)/((3-trace)*r2));
          vnl_vector_fixed<T,3> v(t-0.5*r_x_t + c2*vnl_cross_3d(r,r_x_t));
          //vnl_vector_fixed<T,3> v(t+vnl_cross_3d(r,t));
          //v = ( (R_.inverse()*v)*T(2) + (R_*v) - trace*v) - (R_.inverse()*t) +t;
          //v /= 3 - trace;
          s.update(v, 4);
        }
      }
      else{
        double s_2 = s_*s_;
        double s_3 = s_2*s_;
        vnl_vector_fixed<T,3> v(s[0]*t+vnl_cross_3d(r,t));
        v = s_2*(R_.inverse()*v) + s_*(R_*v) - (s_*trace-1)*v;
        // this division is safe since the only real root of this polynomial
        // is s = 1 for all valid rotation traces
        v /= s_3 + trace*(s_ - s_2) -1;
        s.update(v, 4);
      }

      return s;
    }

  //: Compute the basis vector in the Lie algebra
  vnl_matrix_fixed<T,4,4> lie_algebra() const
    {
      vnl_matrix_fixed<T,4,4> M(T(0));
      vnl_vector_fixed<T,7> s = this->lie_algebra_basis();
      //M(3,3) = 1;
      M(0,0) = M(1,1) = M(2,2) = s[0];
      M(2,1) = s[1];  M(1,2) = -s[1];
      M(0,2) = s[2];  M(2,0) = -s[2];
      M(1,0) = s[3];  M(0,1) = -s[3];
      M(0,3) = s[4];
      M(1,3) = s[5];
      M(2,3) = s[6];
      return M;
    }



  // Operations:----------------------------------------

  //: Make the similarity the identity 
  void set_identity()
    {
      s_ = 1.0;
      R_.set_identity();
      t_.fill(0.0);
    }

  //: The inverse similarity
  dbgl_similarity_3d<T> inverse() const
    {
      vgl_rotation_3d<T> Rinv(R_.inverse());
      return dbgl_similarity_3d<T>(1.0/s_,Rinv,-(Rinv*t_)/s_);
    }

  //: Composition of two similarity
  dbgl_similarity_3d<T> operator*( const dbgl_similarity_3d<T>& rhs ) const
    {
      return dbgl_similarity_3d<T>(s_*rhs.s_, R_*rhs.R_, s_*(R_*rhs.t_)+ t_ );
    }

  //: Transform a homogeneous point.
  vgl_homg_point_3d<T> operator*( const vgl_homg_point_3d<T>& p ) const
    {
      vnl_vector_fixed<T,3> rp = s_*(R_*(vnl_vector_fixed<T,3>(p.x(),p.y(),p.z())));
      rp += p.w()*vnl_vector_fixed<T,3>(t_.x(), t_.y(), t_.z());;
      return vgl_homg_point_3d<T>(rp[0],rp[1],rp[2],p.w());
    }

  //: Transform a homogeneous plane.
  vgl_homg_plane_3d<T> operator*( const vgl_homg_plane_3d<T>& p ) const
    {
      vnl_vector_fixed<T,3> rp = R_*(vnl_vector_fixed<T,3>(p.a(),p.b(),p.c()))/s_;
      vnl_double_3 t(t_.x(), t_.y(), t_.z());
      return vgl_homg_plane_3d<T>(rp[0],rp[1],rp[2],p.d()-dot_product(rp,t));
    }

  //: Transform a homogeneous line.
  vgl_homg_line_3d_2_points<T> operator*( const vgl_homg_line_3d_2_points<T>& l ) const
    {
      return vgl_homg_line_3d_2_points<T>(this->operator*(l.point_finite()),
                                          this->operator*(l.point_infinite()));
    }

  //: Transform a point.
  vgl_point_3d<T> operator*( const vgl_point_3d<T>& p ) const
    {
      vgl_point_3d<T> o(0,0,0);
      return o + s_*(R_*(p-o))+t_;
    }

  //: Transform a plane.
  vgl_plane_3d<T> operator*( const vgl_plane_3d<T>& p ) const
    {
      vnl_vector_fixed<T,3> rp = R_*(vnl_vector_fixed<T,3>(p.a(),p.b(),p.c()))/s_;
      vnl_double_3 t(t_.x(), t_.y(), t_.z());
      return vgl_plane_3d<T>(rp[0],rp[1],rp[2],p.d()-dot_product(rp,t));
    }

  //: Transform a line.
  vgl_line_3d_2_points<T> operator*( const vgl_line_3d_2_points<T>& l ) const
    {
      return vgl_line_3d_2_points<T>(this->operator*(l.point1()),
                                     this->operator*(l.point2()));
    }

  //: Transform a line segment.
  vgl_line_segment_3d<T> operator*( const vgl_line_segment_3d<T>& l ) const
    {
      return vgl_line_segment_3d<T>(this->operator*(l.point1()),
                                    this->operator*(l.point2()));
    }

  //: Transform a vgl vector.
  // rotated and scaled but not effected by translation
  vgl_vector_3d<T> operator*( const vgl_vector_3d<T>& v ) const
    {
      return s_*(R_*v);
    }

  //: Transform a 3d vnl vector.(treat as a point)
  vnl_vector_fixed<T, 3> operator*( const vnl_vector_fixed<T,3>& v ) const
    {
      return s_*(R_*v) + vnl_vector_fixed<T,3>(t_.x(), t_.y(), t_.z());
    }

  //: Transform a 4d vnl vector.
  vnl_vector_fixed<T, 3> operator*( const vnl_vector_fixed<T,4>& v ) const
    {
      return s_*(R_*v.extract(3)) + v[3]*vnl_vector_fixed<T,3>(t_.x(), t_.y(), t_.z());
    }


 protected:
  //: Scaling
  T s_;
  //: Rotation
  vgl_rotation_3d<T> R_;
  //: Translation
  vgl_vector_3d<T>   t_;
};


// External methods for stream I/O
// ----------------------------------------------------------------

template <class T>
vcl_ostream& operator<<(vcl_ostream& s, dbgl_similarity_3d<T> const& S)
{
  return s << "[s:"<<S.scale()<<", R:"<< S.rotation()<<", t:"<<S.translation()<<"]";
}



#endif // dbgl_similarity_3d_h_
