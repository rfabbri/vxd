// This is bmcsd_curve_3d_sketch.h
#ifndef bmcsd_curve_3d_sketch_h
#define bmcsd_curve_3d_sketch_h
//:
//\file
//\brief File to hold 3D curves and attributes
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 11/11/09 18:24:24 EST
//

#include <bdifd/bdifd_frenet.h>
#include <mw/bmcsd_view_set.h>
#include <mw/bmcsd_curve_3d_attributes.h>
#include <mw/bmcsd_util.h>

// This class holds 3D curves together with attributes such as pointers to which
// views were used to reconstruct them, etc. It is used as a basic
// representation for communicating between multiview curve stereo and multiview
// curve photogrammetry
class bmcsd_curve_3d_sketch {
public:

  bmcsd_curve_3d_sketch() {}

  //: You should be using this always with attributes, otherwise just use the
  // curve itself.
  bmcsd_curve_3d_sketch(
      const vcl_vector< bdifd_1st_order_curve_3d > &crv3d,
      const vcl_vector< bmcsd_curve_3d_attributes > &attr)
  : 
    crv3d_(crv3d),
    attr_(attr)
  { 
    assert(attr_.size() == crv3d.size());
  }


  void set(const vcl_vector< bdifd_1st_order_curve_3d > &crv3d,
      const vcl_vector< bmcsd_curve_3d_attributes > &attr)
  {
    crv3d_ = crv3d;
    attr_ = attr;
    assert(attr_.size() == crv3d.size());
  }

  //: Quick n' dirty: write in a directory structure format. The directory will
  // contain a file for each 3D curve, and for each 3D curve there will be a
  // corresponding entry on an attributes.vsl file with the remaining attributes
  // in text format.
  bool write_dir_format(vcl_string dirname) const;
  bool read_dir_format(vcl_string dirname);
  short dir_format_version() const { return 2;}
  unsigned num_curves() const { return crv3d_.size(); }
  unsigned total_num_points() const {
    unsigned npts=0;
    for (unsigned c=0; c < num_curves(); ++c)
      npts += curves_3d()[c].size();
    return npts;
  }

  //: Getters and setters. Setters should assert for consistency.
  const vcl_vector< bdifd_1st_order_curve_3d > & curves_3d() const
  { return crv3d_; }
  const vcl_vector< bmcsd_curve_3d_attributes > &attributes() const
  { return attr_; }

  typedef vcl_vector<vgl_point_3d<double> > curve_points;

  void
  get_curves_as_vgl_points(vcl_vector< curve_points > *pts3d_ptr) const
  {
    vcl_vector< curve_points > &pts3d = *pts3d_ptr;
    pts3d.resize(crv3d_.size());

    for (unsigned c=0; c < num_curves(); ++c) {
      pts3d[c].resize(crv3d_[c].size());
      for (unsigned p=0; p < crv3d_[c].size(); ++p) {
        const bmcsd_vector_3d &pt = crv3d_[c][p].Gama;
        pts3d[c][p] = vgl_point_3d<double>(pt[0], pt[1], pt[2]);
      }
    }
  }

  bool operator==(const bmcsd_curve_3d_sketch &o) const { 
    return crv3d_ == o.crv3d_ && attr_ == o.attr_;
  }

  //: prune
  void prune_by_total_support(double tau_support)
  {
    vcl_vector< bdifd_1st_order_curve_3d > crv3d_new;
    vcl_vector< bmcsd_curve_3d_attributes > attr_new;

    crv3d_new.reserve(num_curves());
    attr_new.reserve(num_curves());

    for (unsigned i=0; i < num_curves(); ++i) {
      if (attributes()[i].total_support_ >= tau_support) {
        crv3d_new.push_back(curves_3d()[i]);
        attr_new.push_back(attributes()[i]);
      }
    }

    crv3d_ = crv3d_new;
    attr_ = attr_new;
  }

  unsigned totalMatchCount_;
  unsigned totalReconCount_;

//private:
  vcl_vector< bdifd_1st_order_curve_3d > crv3d_;
  vcl_vector< bmcsd_curve_3d_attributes > attr_;
};

#endif // bmcsd_curve_3d_sketch_h
