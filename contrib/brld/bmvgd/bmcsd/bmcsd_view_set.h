// This is bmcsd_view_set.h
#ifndef bmcsd_view_set_h
#define bmcsd_view_set_h
//:
//\file
//\brief Class to represent indexes of the views to be matched.
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 08/31/2009 03:34:25 PM PDT
//

#include <vcl_vector.h>
#include <vcl_string.h>
#include <vcl_iostream.h>
#include <assert.h>
#include <vbl/vbl_ref_count.h>
#include <vsl/vsl_binary_io.h>

//: Represents the ids of the views to be used in bmcsd_*_curve_stereo
class bmcsd_stereo_views : public vbl_ref_count {
public:

  //: sets the id of the first view to be stereo-matched.
  // This id is usually a frame index of the view within a large video.
  void set_stereo0(unsigned v) { stereo_0_ = v; }

  //: sets the id of the second view to be stereo-matched.
  void set_stereo1(unsigned v) { stereo_1_ = v; }

  void reserve_num_confirmation_views(unsigned n) 
  { confirm_.reserve(n); }

  void add_confirmation_view(unsigned v) { confirm_.push_back(v); }

  unsigned stereo0() const { return stereo_0_; }
  unsigned stereo1() const { return stereo_1_; }
  unsigned confirmation_view(unsigned i) const { return confirm_[i]; }
  unsigned num_confirmation_views() const { return confirm_.size(); }


  // I/O ------------------------------------------------------------------

  //: Binary save self to stream.
  void b_write(vsl_b_ostream &os) const;

  //: Binary load self from stream.
  void b_read(vsl_b_istream &is);

  //: Return IO version number;
  short version() const { return 1; }

  bool operator==(const bmcsd_stereo_views o) const { 
    return stereo0() == o.stereo0() 
        && stereo1() == o.stereo1()
        && confirm_  == o.confirm_;
  }

private:
  unsigned stereo_0_;
  unsigned stereo_1_;
  vcl_vector<unsigned> confirm_;
};


//: Binary save
inline void vsl_b_write(vsl_b_ostream &os, const bmcsd_stereo_views &p)
{
  p.b_write(os);
}
  
//: Binary read
inline void vsl_b_read(vsl_b_istream &is,  bmcsd_stereo_views &p)
{
  p.b_read(is);
}


#include <bmcsd/bmcsd_stereo_views_sptr.h>

//: Represents indexes of views to be used in multiple instances of binocular
// stereo. Each instance of binocular stereo matches two views, but can use many
// other views for confirmation.
class bmcsd_stereo_instance_views {
public:

  void add_instance(const bmcsd_stereo_views_sptr &one_instance) 
  { v_.push_back(one_instance); }

  const bmcsd_stereo_views_sptr &instance(unsigned i) const 
  { assert(i < num_instances()); return v_[i]; }

  unsigned num_instances() const { return v_.size(); }
  bool empty() const { return v_.empty(); }

private:
  vcl_vector<bmcsd_stereo_views_sptr> v_;
};

//: Write to stream
// \relates bmcsd_stereo_instance_views
vcl_ostream&  operator<<(vcl_ostream& s, const bmcsd_stereo_instance_views& p);

//: Write to stream
// \relates bmcsd_stereo_views
vcl_ostream&  operator<<(vcl_ostream& s, const bmcsd_stereo_views & p);

//: This class gives general utilities to define the sets of views from a video
// that should be passed to subsystems of multiview curve stereo.
class bmcsd_view_set {
public:

  //: returns view instances for a specific dataset to be used in binocular
  // multiview stereo.
  static void get_capitol_subset_view_instances(
      bmcsd_stereo_instance_views *frames_to_match);

  //: Reads from text file.  For the format, see
  // tests/mcs_stereo_instances_example.txt
  static bool read_txt(
      const vcl_string &fname,
      bmcsd_stereo_instance_views *frames_to_match);
};

#endif // bmcsd_view_set_h
