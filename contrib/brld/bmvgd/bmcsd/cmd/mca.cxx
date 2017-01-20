//:
//\file
//\brief Multiview Curve sketch Attributes (mca) command
//\author Ricardo Fabbri, Brown & Rio de Janeiro State U. (rfabbri.github.io)
//
#include <vul/vul_arg.h>
#include <vul/vul_file.h>
#include <buld/buld_arg.h>
#include <bmcsd/bmcsd_util.h>
#include <bmcsd/bmcsd_curve_3d_sketch.h>


#define MW_ASSERT(msg, a, b) if ((a) != (b)) { vcl_cerr << (msg) << vcl_endl; abort(); }

//: Reads curve sketch and prints its attributes to stdout in text format readable by
//other programs like matlab.
int
main(int argc, char **argv)
{
  vcl_string prefix_default(".");
  vcl_string prefix = prefix_default;

  vul_arg<vcl_string> a_csk("-curvesketch", "input 3D curve sketch file or directory", "out/");
  vul_arg<bool> a_showlengths("-showlengths", "shows curves lenghts for each attribute", true);
  vul_arg<bool> a_showviews("-showviews", "shows views for each curve", true);
  vul_arg<bool> a_showinliers("-showinliers", "shows inlier views for each curve", false);

  vul_arg_parse(argc,argv);
  vcl_cout << "\n";

  bmcsd_curve_3d_sketch *csk = new bmcsd_curve_3d_sketch;
  vcl_string csk_fname = prefix + "/" + a_csk();
  bool retval  = csk->read_dir_format(csk_fname);
  MW_ASSERT(vcl_string("Error reading 3D curve sketch: ") + csk_fname, retval, true);

  // Print out all costs to stdout.

  assert(csk->num_curves() == csk->attributes().size());
  for (unsigned i=0; i < csk->num_curves(); ++i) {
    vcl_cout << csk->attributes()[i].total_support_;
    if (a_showlengths())
      vcl_cout << " " << csk->curves_3d()[i].size();
    if (a_showviews()) {
      vcl_cout << " v: "; 
      vcl_cout << csk->attributes()[i].v_->stereo0() << " " <<
                  csk->attributes()[i].v_->stereo1() << " ";

      vcl_cout << " c: ";
      for (unsigned k=0; k < csk->attributes()[i].v_->num_confirmation_views(); ++k)
        vcl_cout << csk->attributes()[i].v_->confirmation_view(k) << " ";
    }
    if (a_showinliers()) {
      vcl_cout << " i: ";
      for (unsigned k=0; k < csk->attributes()[i].inlier_views_.size(); ++k)
        vcl_cout << csk->attributes()[i].inlier_views_[k] << " ";
    }
    vcl_cout << vcl_endl;
  }

  return 0;
}
