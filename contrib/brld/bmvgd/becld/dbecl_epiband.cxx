#include "dbecl_epiband.h"

#include <vnl/vnl_vector_fixed.h>
#include <vnl/vnl_math.h>
#include <vcl_limits.h>
#include <vgl/vgl_clip.h>
#include <vgl/vgl_distance.h>
#include <vgl/vgl_lineseg_test.h>
#include <dbgl/algo/dbgl_intersect.h>
#include <dbecl/dbecl_epipole_sptr.h>
#include <dbecl/dbecl_epipole.h>
#include <vcl_iostream.h>

//: clump arg from minus 1 to 1; also assert abs(arg) not much bigger than 1
static inline double clump_to_acos(double x)
{ 
  if (x > 1.0 || x < -1.0) {
    assert(vcl_fabs(vcl_fabs(x)-1) < 1e-5);
    if (x > 1.0)
      return 1.0;
    if (x < -1.0)
      return -1.0;
  }
  return x;
}

//: returns err_theta
double dbecl_epiband::
compute(const vgl_point_2d<double> &p, const vpgl_fundamental_matrix<double> &fm, double err_pos)
{
  err_pos_ = err_pos;

  // 1- compute err_theta from p and err_pos
  // - predict epipole at infinity
  //
  // 2- Given the middle line l_, and bounding lines l_min and l_max, find
  // the finite polygons representing the search space in the image.
  //    - first, form two finite polygons from epipolar lines that are big enough to be intersected with the image.
 

  // get hold of epipole
  vgl_homg_point_2d<double> er, el;
  fm.get_epipoles(er, el);
  vgl_homg_line_2d<double> l_ = fm.l_epipolar_line(vgl_homg_point_2d<double>(p));
  
  
  // detect case of epipole at infinity; ignore it for now

  if( el.ideal(1e5*vcl_numeric_limits<double>::epsilon())  || er.ideal(1e5*vcl_numeric_limits<double>::epsilon()) ) {
    vcl_cerr << "WARNING: Epipole at infinity not yet supported\n";
    abort();
  }

  
  // define the epipolar band polygon

  vgl_homg_line_2d<double> l_minus, l_plus;
  // err_theta = get_err_theta(double err_pos, vgl_homg_line_2d<double> &l_minus, vgl_homg_line_2d<double> &l_plus)
  double err_theta;
  {
    dbecl_epipole_sptr ep;
    ep = new dbecl_epipole(vgl_point_2d<double>(er));
    vnl_vector_fixed<double,2> ep_v(ep->location().x(), ep->location().y());

    const double d = (vnl_vector_fixed<double,2>(p.x(),p.y()) - ep_v).two_norm(); 

    const double rho = vcl_sqrt(d*d - err_pos*err_pos);

    if (err_pos >= d) {
      vcl_cout << "ERROR: point too close to epipole; case still not coded.\n";
      abort();
    }

    const double angle_p1    = ep->angle(p);
    const double dtheta1     = vcl_asin(err_pos / d);
    const double theta_plus  = angle_p1 + dtheta1;
    const double theta_minus = angle_p1 - dtheta1;

    vgl_homg_point_2d<double> p_plus  ( ep->to_img_coords(vgl_point_2d<double> (rho,theta_plus) ) );
    vgl_homg_point_2d<double> p_minus ( ep->to_img_coords(vgl_point_2d<double> (rho,theta_minus) ) );

    l_minus = fm.l_epipolar_line(p_minus);
    l_plus  = fm.l_epipolar_line(p_plus);
  }

  vnl_vector_fixed<double,2> n_minus(l_minus.a(), l_minus.b());
  vnl_vector_fixed<double,2> t_minus(n_minus[1],-n_minus[0]); t_minus.normalize();
  vnl_vector_fixed<double,2> n_plus(l_plus.a(), l_plus.b());
  vnl_vector_fixed<double,2> t_plus(n_plus[1],-n_plus[0]); t_plus.normalize();


//  vnl_vector_fixed<double,2> n(l_.a(), l_.b());

  vnl_vector_fixed<double,2> c(0.5*(box_.max_x() + box_.min_x()), 0.5*(box_.max_y() + box_.min_y()));

  dbecl_epipole_sptr ep_l;
  ep_l = new dbecl_epipole(vgl_point_2d<double>(el));
  vnl_vector_fixed<double,2> ep_l_v(ep_l->location().x(), ep_l->location().y());

  vnl_vector_fixed<double,2> r1(0.5*(box_.max_x() - box_.min_x()), 0.5*(box_.max_y() - box_.min_y()));

  //: the 5.0 bellow is whatever quantity > 0
  double rho = (c - ep_l_v).two_norm() +  r1.two_norm() + 5.0;

  vnl_vector_fixed<double,2> A = ep_l_v + rho*t_plus;
  vnl_vector_fixed<double,2> C = ep_l_v - rho*t_plus;
  vnl_vector_fixed<double,2> B = ep_l_v + rho*t_minus;
  vnl_vector_fixed<double,2> D = ep_l_v - rho*t_minus;

  poly_.new_sheet();
  poly_.push_back(ep_l_v[0], ep_l_v[1]);


  vgl_point_2d<double> pl1, pl2;

  vgl_line_2d<double> l_nonhomg(l_);

  l_nonhomg.get_two_points(pl1, pl2);

  if ( vgl_lineseg_test_line<double> (
        pl1.x(), pl1.y(), 
        pl2.x(), pl2.y(), 
        A[0], A[1],
        B[0], B[1]
        ) ) {
    poly_.push_back(A[0],A[1]);
    poly_.push_back(B[0],B[1]);
    poly_.push_back(ep_l_v[0],ep_l_v[1]);
    poly_.push_back(C[0],C[1]);
    poly_.push_back(D[0],D[1]);
  // This measures the smallest possible err_theta between the two epipolar lines. However, this
  // might not be the real value.
    err_theta = 0.5*vcl_acos(clump_to_acos( dot_product(t_minus,t_plus) ));
  } else {
    poly_.push_back(A[0],A[1]);
    poly_.push_back(D[0],D[1]);
    poly_.push_back(ep_l_v[0],ep_l_v[1]);
    poly_.push_back(B[0],B[1]);
    poly_.push_back(C[0],C[1]);
    err_theta = 0.5*(vnl_math::pi - vcl_acos(clump_to_acos( dot_product(t_minus,t_plus) )));
  }

  // clip it with window()
  // TODO: don't intersect unless iterator is called.

  vgl_polygon<double> domain(1);
  domain.push_back(box_.min_x(),box_.min_y());
  domain.push_back(box_.max_x(),box_.min_y());
  domain.push_back(box_.max_x(),box_.max_y());
  domain.push_back(box_.min_x(),box_.max_y());

  poly_ = vgl_clip(poly_,domain, vgl_clip_type_intersect);

//    poly_.push_back(2,3);  // A
//    poly_.push_back(5,3);  // B
//    poly_.push_back(5,0);  // C
//    poly_.push_back(10,0); // D
//    poly_.push_back(10,3); // E
//    poly_.push_back(5,3);  // F
//    poly_.push_back(5,6);  // G
//    poly_.push_back(2,6);  // H

//  vcl_cerr << "Generating dummy polygons\n";

  return err_theta;
}

//: Computes from another epipolar band.
//
// For each point in the given epipolar band in image A, there is an epipopolar band in
// another image B. The union of all such bands in image B is a big epipolar
// band, which can be seen as the epipolar band of an epipolar band. 
// It makes more sense when the input band is from e.g. img1 into image 2, and
// we want to "transfer" this epipolar band into an image 3.
//
// \todo predict case where epipole itself is one of the polygon vertices?
//
//returns err_theta
double dbecl_epiband::
compute(const dbecl_epiband &eb_a, const vpgl_fundamental_matrix<double> &fm_ab, double err_pos)
{
  err_pos_ = err_pos;
  if (eb_a.is_empty()) {
    return 0;
  }

  // - guarantee that epipole is not at infinity and is not within eb_a for now
  // 
  // - determine p_minus p_plus, the vertices of eb_a corresponding to
  // the bounding lines of the desired eb_b. Also determine p_mean.
  //
  //
  //
  // - from those 3 points, get the bounding and middle lines of eb_b: l_minus, l_plus, l_mean (simply get 
  // the epipolar lines of p_minus, p_plus, and p_mean).
  //
  // - using the same code as I had for the basic epipolar band, construct the
  // finite polygon representing the epipolar band eb_b by intersecting the
  // lines with the image domain
  //

  // get hold of epipole
  vgl_homg_point_2d<double> er, el;
  fm_ab.get_epipoles(er, el);

  
  // detect case of epipole at infinity; ignore it for now

  if( el.ideal(1e5*vcl_numeric_limits<double>::epsilon())  || er.ideal(1e5*vcl_numeric_limits<double>::epsilon()) ) {
    vcl_cerr << "WARNING: Epipole at infinity not yet supported\n";
    abort();
  }

  
  // define the epipolar band polygon

  vgl_homg_line_2d<double> l_minus, l_plus, l_mean;
  // err_theta = get_err_theta(double err_pos, vgl_homg_line_2d<double> &l_minus, vgl_homg_line_2d<double> &l_plus)
  double err_theta;
  {
    dbecl_epipole_sptr ep;
    ep = new dbecl_epipole(vgl_point_2d<double>(er));
    vnl_vector_fixed<double,2> ep_v(ep->location().x(), ep->location().y());

    const double d = eb_a.distance(vgl_point_2d<double>(er));

    if (d <= err_pos) {
      vcl_cerr << "ERROR: eb_a too close to epipole; case still not coded.\n";
      abort();
    }

    double theta_plus;
    double theta_minus;
    double theta_mean;

    dbecl_epiband::get_bounds(eb_a.polygon(),*ep,theta_plus,theta_minus,theta_mean);
#ifdef DEBUG
    vcl_cout << "Epipolar band width view 'from' (computed deg): " << 180.0* ((theta_plus - theta_minus))/vnl_math::pi << vcl_endl;
#endif

    if (vcl_fabs(theta_plus - theta_minus) >= vnl_math::pi) {
      vcl_cout<< "ERROR: Whole plane being covered -- case not yet implemented\n";
      vcl_cout << "Polygon:\n";
      eb_a.polygon().print(vcl_cout);
      vcl_cout << "Epipole_from: " << ep->location() << vcl_endl;
      abort();
    }

    const double rho = d; //: any rho will do..

    vgl_homg_point_2d<double> p_plus  ( ep->to_img_coords(vgl_point_2d<double> (rho,theta_plus) ) );
    vgl_homg_point_2d<double> p_minus ( ep->to_img_coords(vgl_point_2d<double> (rho,theta_minus) ) );
    vgl_homg_point_2d<double> p_mean ( ep->to_img_coords(vgl_point_2d<double> (rho,theta_mean) ) );

    l_minus = fm_ab.l_epipolar_line(p_minus);
    l_plus  = fm_ab.l_epipolar_line(p_plus);
    l_mean  = fm_ab.l_epipolar_line(p_mean);
  }

  vnl_vector_fixed<double,2> n_minus(l_minus.a(), l_minus.b());
  vnl_vector_fixed<double,2> t_minus(n_minus[1],-n_minus[0]); t_minus.normalize();
  vnl_vector_fixed<double,2> n_plus(l_plus.a(), l_plus.b());
  vnl_vector_fixed<double,2> t_plus(n_plus[1],-n_plus[0]); t_plus.normalize();


  vnl_vector_fixed<double,2> c(0.5*(box_.max_x() + box_.min_x()), 0.5*(box_.max_y() + box_.min_y()));

  dbecl_epipole_sptr ep_l;
  ep_l = new dbecl_epipole(vgl_point_2d<double>(el));
  vnl_vector_fixed<double,2> ep_l_v(ep_l->location().x(), ep_l->location().y());

  vnl_vector_fixed<double,2> r1(0.5*(box_.max_x() - box_.min_x()), 0.5*(box_.max_y() - box_.min_y()));
  //: the 5.0 bellow is any quantity > 0
  double rho = (c - ep_l_v).two_norm() + r1.two_norm() + 5.0;

  vnl_vector_fixed<double,2> A = ep_l_v + rho*t_plus;
  vnl_vector_fixed<double,2> C = ep_l_v - rho*t_plus;
  vnl_vector_fixed<double,2> B = ep_l_v + rho*t_minus;
  vnl_vector_fixed<double,2> D = ep_l_v - rho*t_minus;

  poly_.new_sheet();
  poly_.push_back(ep_l_v[0], ep_l_v[1]);

  vgl_point_2d<double> pl1, pl2;

  vgl_line_2d<double> l_mean_nonhomg(l_mean);

  l_mean_nonhomg.get_two_points(pl1, pl2);

  if ( vgl_lineseg_test_line<double> (
        pl1.x(), pl1.y(), 
        pl2.x(), pl2.y(), 
        A[0], A[1],
        B[0], B[1]
        ) ) {
    poly_.push_back(A[0],A[1]);
    poly_.push_back(B[0],B[1]);
    poly_.push_back(ep_l_v[0],ep_l_v[1]);
    poly_.push_back(C[0],C[1]);
    poly_.push_back(D[0],D[1]);
    err_theta = 0.5*vcl_acos(clump_to_acos( dot_product(t_minus,t_plus) ));
  } else {
    poly_.push_back(A[0],A[1]);
    poly_.push_back(D[0],D[1]);
    poly_.push_back(ep_l_v[0],ep_l_v[1]);
    poly_.push_back(B[0],B[1]);
    poly_.push_back(C[0],C[1]);
    err_theta = 0.5*(vnl_math::pi - vcl_acos(clump_to_acos( dot_product(t_minus,t_plus) )));
  }

  // clip it with window()

//  vgl_polygon<double> domain(1);
//  domain.push_back(0,0);
//  domain.push_back(nx_-1,0);
//  domain.push_back(nx_-1,ny_-1);
//  domain.push_back(0,ny_-1);

  vgl_polygon<double> domain(1);
  domain.push_back(box_.min_x(),box_.min_y());
  domain.push_back(box_.max_x(),box_.min_y());
  domain.push_back(box_.max_x(),box_.max_y());
  domain.push_back(box_.min_x(),box_.max_y());

  poly_ = vgl_clip(poly_,domain, vgl_clip_type_intersect);

  return err_theta;
}


//: Gets angles of bounding epipolar lines that intersect the polygon poly.
// This defines an interval of epipolar lines between theta_minus and
// theta_plus, counter clockwise.
//
// Outputs are not guaranteed to be within any specific angle interval.
//
// \param[out] theta_mean = middle between theta_plus and theta_minus
void dbecl_epiband::
get_bounds(
   const vgl_polygon<double> &poly,
   const dbecl_epipole &ep,
   double &theta_plus,
   double &theta_minus,
   double &theta_mean
    )
{

  if (!(poly.num_sheets() == 1 && poly[0].size() >= 2))
    vcl_cerr << vcl_cout << "ERROR: poly.num_sheets(): " << poly.num_sheets() 
                        << " poly[0].size(): " << poly[0].size() << vcl_endl;

  assert(poly.num_sheets() == 1 && poly[0].size() >= 2);

  theta_plus = ep.angle(poly[0][0]);
  theta_minus = theta_plus;
  double theta_current = theta_plus, 
         theta_previous = theta_plus;

  vgl_point_2d<double> vp = poly[0][0];
  for (unsigned i=1; i < poly[0].size(); ++i) {
    vgl_point_2d<double> vc = poly[0][i];
    double dtheta;


    vgl_line_2d<double> yaxis(0,1,-ep.location().y());
    vgl_line_segment_2d<double> edge(vp,vc);


    vgl_point_2d<double> intr;

    // intercept edge (i-1) with negative x-axis
    bool has_intercept = dbgl_intersect::line_lineseg_intersect(yaxis,edge,intr);

    if (has_intercept && intr.x() < ep.location().x() && ep.angle(vp)*ep.angle(vc) < 0)  { // edge (i-1) intercepts x-axis on negative value and crosses it
      if (ep.angle(vp) > ep.angle(vc)) {
        dtheta = ep.angle(vc) + 2*vnl_math::pi - ep.angle(vp);
      } else {
        dtheta = ep.angle(vc) - (ep.angle(vp) + 2*vnl_math::pi);
      }
    } else {
      dtheta = ep.angle(vc) - ep.angle(vp);
    }

    theta_current  = theta_previous + dtheta;
    theta_previous = theta_current;

    if (theta_current > theta_plus)
      theta_plus = theta_current;

    if (theta_current < theta_minus )
      theta_minus = theta_current;

    vp = vc;
  }

  theta_mean = (theta_plus + theta_minus)/2.0;
}


bool dbecl_epiband::
contains(const vgl_point_2d<double> &pt) const
{

  return poly_.contains(pt);
}

//: Minimum distance of pt to epipolar band.
// Interior points DO NOT have distance zero!
double dbecl_epiband::
distance(const vgl_point_2d<double> &pt) const
{
  if (is_empty())
    return vcl_numeric_limits<double>::infinity();
  return vgl_distance(poly_, pt);
}


//: - intersect two other ones, generating this one
// \return false in case any degeneracy was detected after intersection.
//
// TODO: use a specific algorithm for convex polygons
bool dbecl_epiband::
intersect( const dbecl_epiband &a, const dbecl_epiband &b)
{ 
  this->poly_ = vgl_clip(a.poly_, b.poly_, vgl_clip_type_intersect);
  assert (this->box_ == a.box_ && b.box_ == a.box_);

  if (!is_empty() && (poly_.num_sheets() > 1 || poly_[0].size() < 2)) {
    vcl_cout << "\nINTERSECTION WARNING: num_sheets: " << poly_.num_sheets() 
                        << " poly[0].size: " << poly_[0].size() << vcl_endl;

    vcl_cout << "Poly 1(area " << a.area() << "): " << a.poly_ << vcl_endl;
    vcl_cout << "Poly 2(area " << b.area() << "): " << b.poly_ << vcl_endl;

    if (poly_.num_sheets() > 1) {
      // return only the one with largest area.
      unsigned max_i=(unsigned)-1;
      double max_a = 0;
      for (unsigned i=0; i < poly_.num_sheets(); ++i) {
        const double a = vgl_area<double>(vgl_polygon<double>(poly_[i]));
        if ( a > max_a) {
          max_a = a;
          max_i = i;
        }
      }
      if (max_i == (unsigned)-1) {
        max_i = 0;
        vcl_cout  << "Probably degenerate empty poly\n";
      }
      vgl_polygon<double>::sheet_t max_sheet = poly_[max_i];
      poly_.clear();
      poly_.push_back(max_sheet);

      vcl_cout << "Resulting (maximal) poly (area " << area() << "): " << poly_ << vcl_endl;
    } else 
      vcl_cout << "Unexpected EMPTY polygon" << vcl_endl;

    return false;
  }

  return true;
}
