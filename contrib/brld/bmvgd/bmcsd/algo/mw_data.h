// This is mw_data.h
#ifndef mw_data_h
#define mw_data_h
//:
//\file
//\brief Dataset handling for multiview stereo
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri@gmail.com)
//\date 08/29/2009 01:54:15 PM PDT
//

#include <vcl_string.h>
#include <bmcsd/bmcsd_util.h>

//: Paths/URLs for the data of 1 frame
//
// The cam_fname can be <fileprefix>*anything.
class bmcsd_curve_stereo_frame_data_path {
public:

  vcl_string cam_full_path() const { return path_ + cam_fname_; }
  vcl_string edg_full_path() const { return path_ + edg_fname_; }
  vcl_string frag_full_path() const { return path_ + frag_fname_; }
  vcl_string img_full_path() const { return path_ + img_fname_; }
  vcl_string cvlet_full_path() const { return path_ + cvlet_fname_; }
  bmcsd_util::camera_file_type cam_file_type() const  {return cam_ftype_; }

  vcl_string path_;
  vcl_string img_fname_;
  vcl_string cam_fname_;
  vcl_string edg_fname_;
  vcl_string frag_fname_;
  vcl_string cvlet_fname_;
  bmcsd_util::camera_file_type cam_ftype_;
};

//: Paths/URL for full datasets to be used in multiview stereo
class bmcsd_curve_stereo_data_path {
public:
  bmcsd_curve_stereo_data_path() {}
  unsigned nviews() const { return views_.size(); }

  //: Return the i-th element
  bmcsd_curve_stereo_frame_data_path & operator[] ( unsigned int v ) 
  { assert (v < nviews()); return views_[v]; }

  //: Return the i-th element
  const bmcsd_curve_stereo_frame_data_path & operator[] ( unsigned int v ) const 
  { assert (v < nviews()); return views_[v]; }

  //: Automatically guesses what types of files are in the path, and loads their
  // filenames.
  //
  // Currently, we assume the following format for the dataset:
  //
  // - Image files are in png format, of the form  <fnameprefix>.png
  //
  // - Camera calibration is in one of two formats
  //    - calib.intrinsic and <fnameprefix>.extrinsic pair, with optional
  //    <fnameprefix>.origin to denote a crop, and calib.scale to denote image
  //    scaling. More details in read_cam in bmcsd_util.h
  //
  //    - calibration given by projection matrix files, one for each frame, in
  //    the format <fnameprefix>.projmatrix
  //
  //    - This function tries to find *.projmatrix. If doesn't find, it loads
  //    the intrinsic/extrinsic pair.
  //
  // - In addition, for each image file, there is:
  //    - an edgemap file <fnameprefix>*.edg
  //    - a curve fragment file <fnameprefix>*.vsl
  //
  //bool populate_from_path(const vcl_string &path, const vcl_string &fnameprefix) {
    //
    // edgels:
    //   ls $path/$fnameprefix*edg
    //
    // curve frags:
    //   ls $path/$fnameprefix*edg
    // 
    // camera:
    //   has_intrinsic_extrinsic = ls $path/*.intrinsic
    //   if (test $has_intrinsic_extrinsic != "") {
    //      camera type = MW_INTRINSIC_EXTRINSIC
    //   } else  {
    //      has_projmatrix = ls $path/*.projmatrix
    //      if (test $has_projmatrix == "")
    //        error("No cameras");
    //      else
    //        camera type = MW_INTRINSIC_EXTRINSIC
    //   }
    //   
  //}

  //: Load dataset info from an xml file
  // bool populate_from_xml_config(const vcl_string &config_fname);


  bool set(const vcl_string &path, 
      const vcl_vector<vcl_string> &img_fnames, 
      const vcl_vector<vcl_string> &cam_fnames,
      const vcl_vector<vcl_string> &edg_fnames,
      const vcl_vector<vcl_string> &frag_fnames,
      bmcsd_util::camera_file_type cam_ftype
      );

  bool set_curvelets(const vcl_vector<vcl_string> &cvlet_fnames);

  bool has_curvelets() 
  { return !views_.empty() && !views_[0].cvlet_fname_.empty(); }

  // Data -------------
private:
  vcl_vector<bmcsd_curve_stereo_frame_data_path> views_;
};

//: Write to stream
// \relates bmcsd_curve_stereo_data_path 
vcl_ostream&  operator<<(vcl_ostream& s, const bmcsd_curve_stereo_data_path& p);

//: Write to stream
// \relates bmcsd_curve_stereo_frame_data_path
vcl_ostream&  operator<<(vcl_ostream& s, const bmcsd_curve_stereo_frame_data_path& p);

struct mw_data {
  // Commmon datasets I use, hardcoded.
  static void
  get_capitol_building_subset(bmcsd_curve_stereo_data_path *dataset);

  //: Reads from text file.  For the format, see
  // tests/mcs_stereo_instances_example.txt
  static bool read_frame_data_list_txt(
      const vcl_string &path,
      bmcsd_curve_stereo_data_path *dataset,
      bmcsd_util::camera_file_type cam_type);
};

#endif // mw_data_h
