#include "mw_data.h"
#include <buld/buld_parse_simple_file.h>
#include <vul/vul_file.h>

void mw_data::
get_capitol_building_subset(bmcsd_curve_stereo_data_path *d)
{
  vcl_string path("/usr/local/moredata/subset/");
  vcl_vector<vcl_string> edgemaps_fnames;
  vcl_vector<vcl_string> img_fnames; 
  vcl_vector<vcl_string> cam_fnames;
  vcl_vector<vcl_string> edg_fnames;
  vcl_vector<vcl_string> frag_fnames;

  edgemaps_fnames.push_back("frame_00001-grad_thresh5.edg.gz"); //0 0 
  edgemaps_fnames.push_back("frame_00066-grad_thresh5.edg.gz"); //1 2
  edgemaps_fnames.push_back("frame_00137-grad_thresh5.edg.gz"); //2 4
  edgemaps_fnames.push_back("frame_00177-grad_thresh5.edg.gz"); //3 5
  edgemaps_fnames.push_back("frame_00030-grad_thresh5.edg.gz"); //4 1
  edgemaps_fnames.push_back("frame_00100-grad_thresh5.edg.gz"); //5 3

  frag_fnames.push_back("frame_00001.vsl");
  frag_fnames.push_back("frame_00066.vsl");
  frag_fnames.push_back("frame_00137.vsl");
  frag_fnames.push_back("frame_00177.vsl");
  frag_fnames.push_back("frame_00030.vsl");
  frag_fnames.push_back("frame_00100.vsl");

  img_fnames.push_back("frame_00001.png");
  img_fnames.push_back("frame_00066.png");
  img_fnames.push_back("frame_00137.png");
  img_fnames.push_back("frame_00177.png");
  img_fnames.push_back("frame_00030.png");
  img_fnames.push_back("frame_00100.png");

  cam_fnames.push_back("frame_00001.png");
  cam_fnames.push_back("frame_00066.png");
  cam_fnames.push_back("frame_00137.png");
  cam_fnames.push_back("frame_00177.png");
  cam_fnames.push_back("frame_00030.png");
  cam_fnames.push_back("frame_00100.png");

  bmcsd_util::camera_file_type cam_type = bmcsd_util::BMCS_INTRINSIC_EXTRINSIC;
  d->set(path, img_fnames, cam_fnames, edgemaps_fnames, frag_fnames, cam_type);
}

bool bmcsd_curve_stereo_data_path::
set(const vcl_string &path, 
    const vcl_vector<vcl_string> &img_fnames, 
    const vcl_vector<vcl_string> &cam_fnames,
    const vcl_vector<vcl_string> &edg_fnames,
    const vcl_vector<vcl_string> &frag_fnames,
    bmcsd_util::camera_file_type cam_ftype
    )
{
  unsigned nviews = img_fnames.size();
  if (cam_fnames.size() != nviews)  {
    vcl_cerr << "Error in bmcsd_curve_stereo_data_path::set():"
             << "Num of camera files different than num of images.\n";
    return false;
  }
  if (edg_fnames.size() != nviews) {
    vcl_cerr << "Error in bmcsd_curve_stereo_data_path::set():"
             << "Num of edge files different than num of images.\n";
    return false;
  }
  if (frag_fnames.size() != nviews) {
    vcl_cerr << "Error in bmcsd_curve_stereo_data_path::set():"
             << "Num of curve fragments different than num of images.\n";
    return false;
  }
  views_.resize(nviews);

  for (unsigned i=0; i < nviews; ++i) {
    views_[i].path_ = path + vcl_string("/");
    views_[i].img_fname_ = img_fnames[i];
    views_[i].edg_fname_ = edg_fnames[i];
    views_[i].cam_fname_ = cam_fnames[i];
    views_[i].frag_fname_ = frag_fnames[i];
    views_[i].cam_ftype_ = cam_ftype;
  }
  return true;
}

bool bmcsd_curve_stereo_data_path::
set_curvelets(const vcl_vector<vcl_string> &cvlet_fnames)
{
  unsigned nviews = views_.size();
  if (cvlet_fnames.size() != nviews)  {
    vcl_cerr << "Error in bmcsd_curve_stereo_data_path::set():"
             << "Num of camera files different than num of images.\n";
    return false;
  }

  for (unsigned i=0; i < nviews; ++i) {
    views_[i].cvlet_fname_ = cvlet_fnames[i];
  }

  return true;
}

bool mw_data::
read_frame_data_list_txt(
    const vcl_string &path,
    bmcsd_curve_stereo_data_path *dataset,
    bmcsd_util::camera_file_type cam_type
    )
{
  vcl_string fname = path + vcl_string("/mcs_img_list.txt");
  vcl_cout << "Reading " << fname << vcl_endl;
  vcl_vector<vcl_string > img_fnames;
  bool retval = buld_parse_string_list(fname, img_fnames);
  if (!retval) return false;

  fname = path + vcl_string("/mcs_edg_list.txt");
  vcl_cout << "Reading " << fname << vcl_endl;
  vcl_vector<vcl_string > edg_fnames;
  retval = buld_parse_string_list(fname, edg_fnames);
  if (!retval) return false;

  fname = path + vcl_string("/mcs_frag_list.txt");
  vcl_cout << "Reading " << fname << vcl_endl;
  vcl_vector<vcl_string > frag_fnames;
  retval = buld_parse_string_list(fname, frag_fnames);
  if (!retval) return false;
  
  retval = 
   dataset->set(path, img_fnames, img_fnames, edg_fnames, frag_fnames, cam_type);
  if (!retval)  {
    vcl_cerr << "Problem calling set in read_frame_data_list_txt\n";
    return false;
  }

  fname = path + vcl_string("/mcs_cvlet_list.txt");
  vcl_vector<vcl_string > cvlet_fnames;
  if (vul_file::exists(fname)) {
    vcl_cout << "Reading " << fname << vcl_endl;
    retval = buld_parse_string_list(fname, cvlet_fnames);
    if (!retval) return false;

    retval = 
     dataset->set_curvelets(cvlet_fnames);
    if (!retval)  {
      vcl_cerr << "Problem calling set_cvlet in read_frame_data_list_txt\n";
      return false;
    }
  } else {
    vcl_cout << "No curvelets in this dataset.\n";
  }

  return true;
}

// #define MW_VERBOSE_DEBUG 0
vcl_ostream&  operator<<(vcl_ostream& s, const bmcsd_curve_stereo_data_path& p)
{
  s << "nviews: " << p.nviews() << vcl_endl;
  assert(p.nviews());

#ifdef MW_VERBOSE_DEBUG
  for (unsigned i=0; i < p.nviews(); ++i) {
    vcl_cout << "pth data view[" << i << "]:\n" << p[i] << vcl_endl;
  }
#else
  s << "path data view[0]:\n" << p[0] << vcl_endl;
  s << "path data view[end]:\n" << p[p.nviews()-1] << vcl_endl;
#endif

  return s;
}

vcl_ostream&  operator<<(vcl_ostream& s, const bmcsd_curve_stereo_frame_data_path& p)
{
  s << "path: " << p.path_ << vcl_endl;
  s << "cam_ftype: " << p.cam_ftype_ << vcl_endl;
  s << "img_fname: " << p.img_fname_ << vcl_endl;
  s << "edg_fname: " << p.edg_fname_ << vcl_endl;
  s << "frag_fname: " << p.frag_fname_ << vcl_endl;
  s << "cvlet_fname: " << p.cvlet_fname_ << vcl_endl;
  return s;
}
