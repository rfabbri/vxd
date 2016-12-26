// This is bmcsd_curve_3d_attributes.h
#ifndef bmcsd_curve_3d_attributes_h
#define bmcsd_curve_3d_attributes_h
//:
//\file
//\brief Attributes to a 3D curve
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 08/31/2009 04:05:46 PM PDT
//

#include <bmcsd/bmcsd_util.h>
#include <bmcsd/bmcsd_view_set.h>
#include <bmcsd/bmcsd_stereo_views_sptr.h>
#include <vsl/vsl_binary_io.h>

class bmcsd_curve_3d_attributes {
public:
  bmcsd_curve_3d_attributes()
    :
      total_support_ (static_cast<unsigned>(-1)),
      i0_(static_cast<unsigned>(-1)),
      i1_(static_cast<unsigned>(-1))
  {
  }
  void set_views(const bmcsd_stereo_views_sptr &v) { v_ = v; }

  //: the views it was reconstructed from.
  bmcsd_stereo_views_sptr v_;

  //: the inlier views when available.
  vcl_vector<unsigned> inlier_views_;

  //: the total support this curve received during matching.
  unsigned total_support_;

  //: the ids of the two curves in v_->stereo0() and v_->stereo1() that
  // generated this one.
  unsigned i0() const { return i0_; }
  unsigned i1() const { return i1_; }
  void set_i0_i1(unsigned i0, unsigned i1) { 
    i0_ = i0;
    i1_ = i1;
  }

  bool operator==(const bmcsd_curve_3d_attributes &o) const {
    return *v_ == *o.v_ && inlier_views_ == o.inlier_views_
      && o.i0_ == i0_ && o.i1_ == i1_ && o.total_support_ == total_support_;
  }

  // I/O ---------------------------------
  short version() const { return 2; }
  void b_read(vsl_b_istream &is);
  void b_write(vsl_b_ostream &os) const;

  //: Print an ascii summary to the stream
  void print_summary(vcl_ostream &os) const;

  vcl_string is_a() const { return "bmcsd_curve_3d_attributes"; }
private:
  unsigned i0_;
  unsigned i1_;
};

//: Binary save
inline void vsl_b_write(vsl_b_ostream &os, const bmcsd_curve_3d_attributes &p)
{
  p.b_write(os);
}
  
//: Binary read
inline void vsl_b_read(vsl_b_istream &is,  bmcsd_curve_3d_attributes &p)
{
  p.b_read(is);
}

inline void vsl_print_summary(vcl_ostream &os, const bmcsd_curve_3d_attributes &p)
{
  p.print_summary(os);
}


#endif // bmcsd_curve_3d_attributes_h
