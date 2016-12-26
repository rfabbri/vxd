#ifndef dbsol_corner_finder_h_
#define dbsol_corner_finder_h_
//-----------------------------------------------------------------------------
//:
// \file
// \author H. Can Aras
// \brief Corner finder using dbsol_interp_curve_2d
//        dbsol_interp_curve_2d that is passed to this class' constructor
//        was created from a set of data points and the corner finding 
//        algorithm only checks if these points are corners. In other words, 
//        this is not a generic corner finding algorithm that finds the corners
//        on the curve at any arbitrary point.
//        If the curve is closed, the end point of the last curve segment is not
//        checked for being a corner because it was already considered as the
//        start point of the first curve segment
// \date 02/02/07
//
// \verbatim
//  Modifications
// \endverbatim
//
//-----------------------------------------------------------------------------
#include <dbsol/dbsol_interp_curve_2d.h>
#include <vsol/vsol_point_2d.h>
#include <vcl_vector.h>

class dbsol_corner_finder {
public:
  dbsol_corner_finder(){};
  ~dbsol_corner_finder(){};
  // pre-defined thresholds are used
  void find_corners(dbsol_interp_curve_2d *c, bool is_open, vcl_string out_angle_diffs="");
  void find_corners(dbsol_interp_curve_2d *c, bool is_open, 
                    int vicinity, double dist_step, double min_tan_turn, 
                    vcl_string out_angle_diffs="");
  // threshold are specified by user
/*  void find_corners(dbsol_interp_curve_2d *c, bool is_open,
                    int VICINITY, double DIST_STEP, double MIN_TAN_TURN,
                    vcl_string out_angle_diffs="");*/
  vcl_vector<int> *get_corner_indices() { return &_extrema; };
protected:
  void get_tangent_angles();
  void compute_average_tangent_angles();
  double aveTangent(int start, int count);
  void find_extrema();

  dbsol_interp_curve_2d *curve_;
  bool is_open_;
  vcl_vector<double> _tangent;
  vcl_vector< vcl_pair<double, double> > _aveTangent;
  vcl_vector<int> _extrema;
  vcl_string out_angle_diffs_;
private:
  int VICINITY_;
  double DIST_STEP_;
  double MIN_TAN_TURN_;
};
#endif  // dbsol_corner_finder_h
