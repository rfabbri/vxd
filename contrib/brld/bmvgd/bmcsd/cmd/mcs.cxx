#include <vul/vul_arg.h>
#include <buld/buld_arg.h>
#include <bmcsd/bmcsd_util.h>
#include <mw/pro/bmcsd_stereo_driver.h>


#define MW_ASSERT(msg, a, b) if ((a) != (b)) { vcl_cerr << (msg) << vcl_endl; exit(1); }

int
main(int argc, char **argv)
{
  vcl_string prefix_default(".");

  vul_arg<vcl_string> a_prefix("-prefix", 
      "path to directory of files",prefix_default.c_str());
  vul_arg<vcl_string> a_cam_type("-cam_type",
      "camera type: intrinsic_extrinsic or projcamera","intrinsic_extrinsic");
  vul_arg<vcl_string> a_out_dir("-outdir", "output directory relative to -prefix", "out/");
  vul_arg<double> a_distance_threshold("-dist", 
      "(in pixels) threshold for an edgel to be an inlier to the reprojected curve in each view", 10.0);
  vul_arg<double> a_dtheta_threshold("-dtheta", 
      "(in degrees) threshold in orientation difference for an edgel to be an inlier to reprojected curve in each view", 10.0);
  vul_arg<unsigned> a_min_samples_per_curve_frag("-minsamples", 
      "Used to prune the curves by enforcing a minimum number of samples.", 30);
  vul_arg<double> a_min_length_per_curve_frag("-minlength", 
      "Used to prune the curves by enforcing a minimum length (this is xor minsamples).", 40);
  vul_arg<bool> a_prune_by_length("-prune_by_length", 
      "Prune using length? if not, use number of samples", true);
  vul_arg<double> a_min_epiangle("-minepiangle", 
      "(in degrees) minimum angle between an epipolar line and curve samples to consider", 30.0);

  vul_arg<unsigned> a_min_epipolar_overlap("-min_epipolar_overlap", 
      "minimum number of intersections a curve has to have with the epiband to be a candidate", 5);

  vul_arg<unsigned> a_min_inliers_per_view("-mininliers_view", 
      "If a view has less than this number of inliers, it will not vote at all towards the curve.", 20);
  vul_arg<unsigned> a_min_total_inliers("-mininliers_total", 
      "If a view has less than this number of inliers, it will not vote at all towards the curve.", 5);

  vul_arg<double> a_min_first_to_second_ratio("-first_to_second_ratio", 
      "minimum ratio of first to second best to consider as reliable unambiguous match. Set to zero if you always want to keep the best match", 1.5);

  vul_arg<unsigned> a_lonely_threshold("-lonely", 
      "minimum #votes of a lonely correspondence = -lonely*-first_to_second_ratio will be considered as reliable match (defaults to mininliers_total)", a_min_total_inliers());

  vul_arg<unsigned> a_max_concurrent_matchers("-nmatchers", 
      "The maximum number of matchers to run simultaneously", 1);
  vul_arg<bool> a_write_corresp("-write_corresp", 
      "write correspondence", false);
  vul_arg<bool> a_use_curvelets("-use_curvelets", 
      "(EXPERIMENTAL) Use curvelets to filter inlier edgels", false);

  vul_arg<unsigned> a_min_num_inliers_per_curvelet("-mininliers_cvlet", 
      "(EXPERIMENTAL) the minimum number of inlier edgels a curvelet must have in order to be inlier curvelet. Works only if -use_curvelets is set.", 3);

  vul_arg_parse(argc,argv);
  vcl_cout << "\n";

  bmcsd_util::camera_file_type cam_type;

  if (a_cam_type() == "intrinsic_extrinsic") {
    cam_type = bmcsd_util::MW_INTRINSIC_EXTRINSIC;
  } else {
    if (a_cam_type() == "projcamera")
      cam_type = bmcsd_util::MW_3X4;
    else  {
      vcl_cerr << "Error: invalid camera type " << a_cam_type() << vcl_endl;
      return 1;
    }
  }

  bmcsd_curve_stereo_data_path dpath;
  bool retval = 
    mw_data::read_frame_data_list_txt(a_prefix(), &dpath, cam_type);
  if (!retval) return 1;
  vcl_cout << "Dpath:\n" << dpath << vcl_endl;

  bmcsd_stereo_instance_views frames_to_match;

  retval = bmcsd_view_set::read_txt(
      a_prefix() + vcl_string("/mcs_stereo_instances.txt"), 
      &frames_to_match);
  MW_ASSERT("frames to match from file", retval, true);
  vcl_cout << "Instances:\n" << frames_to_match << vcl_endl;

  if (a_use_curvelets() && !dpath.has_curvelets()) {
    vcl_cerr << "Error: curvelets requested, but no file names found.\n";
  }

  // Run 2-view stereo with confirmation views

  bmcsd_concurrent_stereo_driver s(dpath, frames_to_match);

  s.set_dtheta_threshold(a_dtheta_threshold());
  s.set_distance_threshold(a_distance_threshold());

  if (a_prune_by_length())
    s.set_min_length_per_curve_frag(a_min_length_per_curve_frag());
  else
    s.set_min_samples_per_curve_frag(a_min_samples_per_curve_frag());
  s.set_min_inliers_per_view(a_min_inliers_per_view()) ;
  s.set_min_epiangle(a_min_epiangle()) ;
  s.set_min_epipolar_overlap(a_min_epipolar_overlap()) ;
  s.set_min_total_inliers(a_min_total_inliers()) ;
  s.set_min_first_to_second_best_ratio(a_min_first_to_second_ratio()) ;
  s.set_lonely_threshold(a_lonely_threshold()) ;
  s.set_use_curvelets(a_use_curvelets());
  s.set_min_num_inlier_edgels_per_curvelet(a_min_num_inliers_per_curvelet());

  //: How many stereo matchers can run simultaneously
  s.set_max_concurrent_matchers(a_max_concurrent_matchers());

  retval = s.init();
  MW_ASSERT("Stereo driver init return value", retval, true);

  //: Run many pairwise stereo programs, as many as
  // frames_to_match.num_instances();
  retval = s.run();
  MW_ASSERT("Stereo driver run return value", retval, true);

  //: Write 3D curves and attributes to file.
  bmcsd_curve_3d_sketch csk;
  s.get_curve_sketch(&csk);

  retval = csk.write_dir_format(a_prefix() + vcl_string("/") + a_out_dir());
  MW_ASSERT("Error while trying to write file.\n", retval, true);

  if (a_write_corresp()) {
    for (unsigned i=0; i < s.num_corresp(); ++i) {
      vcl_ostringstream ns;
      ns << i;
      vcl_string fname
        = a_prefix() + vcl_string("/") + a_out_dir() + vcl_string("/corresp.vsl") + ns.str();
      vsl_b_ofstream corr_ofs(fname);
      vsl_b_write(corr_ofs, s.corresp(i));
    }
  }

  return 0;
}
