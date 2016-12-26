#ifndef dbsol_curve_algs_h_
#define dbsol_curve_algs_h_
//-----------------------------------------------------------------------------
//:
// \file
// \author Ozge Can Ozcanli
// \brief dbsol_interp_curve_2d algorithms
// \date 02/25/05
//
// \verbatim
//  Modifications
//   Initial version November 25, 2002
//   Ozge Can Ozcanli, Feb 26 2006, added interpolate_linear(dbsol_interp_curve_2d *c,
//                                                           vsol_polygon_2d_sptr poly);
//
//   Ozge C Ozcanli April 03, 08 --
//             changed all the loops in the following fashion with no paranthesis:
//             for (unsigned i=0; i<ints.size(); ++i) 
//                ints[i] = new dbgl_poly_curve_line(pts[i], pts[i+1]);
//
//             to the one below:
//             for (unsigned i=0; i<ints.size(); i++) { 
//                ints[i] = new dbgl_poly_curve_line(pts[i], pts[i+1]);
//             }
//
//             there is a bug in an older version of gcc which is still used on one of our linux machines
// \endverbatim
//
//-----------------------------------------------------------------------------
#include <dbsol/dbsol_interp_curve_2d.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_polygon_2d_sptr.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_spatial_object_2d_sptr.h>
#include <dbgl/dbgl_arc.h>
#include <vgl/vgl_point_2d.h>
#include <vnl/vnl_vector.h>

// see bdgl_curve_algs.h for an example of such a class

//: Interpolators and other algorithms constructing or acting on
//dbsol_intepr_curve_2d. They have not been made methods of the curve class so
//that the curve class can be independent of them. For example, changing their
//implementation or interface should not require a change of the curve class
//declaration or a rebuild of the basic dbsol library nor of software that use it.
//

class dbsol_curve_algs {
public:
  
  ~dbsol_curve_algs();

  //:Sample the curve and produce given number of samples
  static bool sample(dbsol_interp_curve_2d const &c, int size,
                     vcl_vector<vsol_point_2d_sptr>& pts);

  //: return the samples and the tangents at those samples
  //  no max length threshold to sample if negative
  static bool sample(dbsol_interp_curve_2d const &c, double ds,
                     vcl_vector<vsol_point_2d_sptr>& pts, vcl_vector<double>& tangents, double length_threshold = -1);

  //: sample a "region" evenly around this interp curve, 
  // (including the points right on the curve is optional, if not they start from delta away on the normal direction)
  static void sample_region_along_curve(dbsol_interp_curve_2d const &curve, vcl_vector<vsol_point_2d_sptr>& region_pts, double delta, double length_threshold, float region_width, bool add_curve_points);

  static bool interpolate_linear(dbsol_interp_curve_2d *c,
                       vcl_vector<vsol_point_2d_sptr> const &pts, bool closed=false);

  static bool interpolate_linear(dbsol_interp_curve_2d *c,
                       vcl_vector<vgl_point_2d<double> > const &pts, bool closed=false);

  static bool interpolate_linear(dbsol_interp_curve_2d *c,
                                 vsol_polygon_2d_sptr poly);   // closed 

  static bool interpolate_linear(dbsol_interp_curve_2d *c,
                                 vsol_polyline_2d_sptr poly);   // open 
  
  static bool interpolate_linear2(dbsol_interp_curve_2d *c,
                       vcl_vector<vsol_point_2d_sptr> const &pts, bool closed=false, int time=0);

  static bool interpolate_eno(dbsol_interp_curve_2d *c,
                              vcl_vector<vsol_point_2d_sptr> const &pts,
                              vnl_vector<double> &sample_pts);

  static bool interpolate_eno(dbsol_interp_curve_2d *c,
                              vcl_vector<vsol_point_2d> const &pts,
                              vnl_vector<double> &sample_pts);

  static bool interpolate_eno(dbsol_interp_curve_2d *c,
                              vcl_vector< vgl_point_2d<double> > const &pts,
                              vnl_vector<double> &sample_pts);

  static bool interpolate_eno_main(dbsol_interp_curve_2d *c,
                                   vnl_vector<double> const &data_x,
                                   vnl_vector<double> const &data_y,
                                   vnl_vector<double> &sample_pts);

  //: delete k = ~log_2(perc_d/divident) many segments from p whose total length is ~perc_d percent of the original length
  //  the mean length of each deleted segment is ~perc_d/k percent of the total length with variance perc_sigma
  //  fromula from [Ghosh, Petkov, PAMI05], the divident is fixed at 8 in that paper
  static bool segment_wise_deletion(const vsol_polygon_2d_sptr& p, 
                                    vcl_vector<vsol_polyline_2d_sptr>& pieces, 
                                    vcl_vector<vsol_polyline_2d_sptr>& del_pieces, 
                                    float perc_d, float perc_sigma, float divident); 

  //: add k = ~ (perc_d*total_length)/divident many segments to p whose total length is ~perc_d percent of the original length
  //  the mean length of each newly added segment is ~perc_d/k percent of the total length with variance perc_sigma
  // add the segment with a random orientation from x-axis at a random region in or out of the contour
  // output is a spatial object vector to be saved as a .cem file
  static bool segment_addition(const vsol_polygon_2d_sptr& p, 
                                    vcl_vector<vsol_spatial_object_2d_sptr>& pieces, 
                                    float perc_d, float perc_sigma, float divident); 

  //: add k = ~ (perc_d*total_length)/divident many segments to p whose total length is ~perc_d percent of the original length
  //  the mean length of each newly added segment is ~perc_d/k percent of the total length with variance perc_sigma
  //  add the segment with a random orientation from x-axis at a random region in or out of the contour
  //  and
  //  delete k = ~log_2(perc_d/divident) many segments from p whose total length is ~perc_d percent of the original length
  //  the mean length of each deleted segment is ~perc_d/k percent of the total length with variance perc_sigma
  //  fromula from [Ghosh, Petkov, PAMI05], the divident is fixed at 8 in that paper
  //  output is a spatial object vector to be saved as a .cem file
  static bool segment_addition_and_deletion(const vsol_polygon_2d_sptr& p, 
                                            vcl_vector<vsol_spatial_object_2d_sptr>& pieces, 
                                            vcl_vector<vsol_polyline_2d_sptr>& del_pieces,
                                            float add_perc_d, float add_perc_sigma, float add_divident,
                                            float del_perc_d, float del_perc_sigma, float del_divident); 


 private:
  dbsol_curve_algs();
};


//: returns 0 if anything goes wrong, check the output pointer!
vsol_polyline_2d_sptr 
fit_lines_to_contour(vsol_polyline_2d_sptr poly, double rms) ;

//: returns 0 if anything goes wrong, check the output pointer!
vsol_polygon_2d_sptr fit_lines_to_contour(vsol_polygon_2d_sptr poly, double rms);

//: returns a vector of size 0 if anything goes wrong
void fit_lines_to_contour(vcl_vector<vsol_point_2d_sptr>& poly, double rms,
                          vcl_vector<vsol_point_2d_sptr>& new_pts); 

//: This function subsamples the points in the contour according to the c_ds value
//  just like Thomas sebastian did
vsol_polygon_2d_sptr dbsol_subsample_contour(vsol_polygon_2d_sptr poly, double c_ds=1.0);

//: This function subsamples the points in the contour according to the c_ds value
vsol_polygon_2d_sptr dbsol_subsample_contour_smart(vsol_polygon_2d_sptr poly, double c_ds=1.0);

#endif  // dbsol_curve_algs_h
