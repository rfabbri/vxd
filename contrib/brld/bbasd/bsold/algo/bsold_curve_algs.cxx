//:
// \file

#include <vcl_cmath.h>

#include "dbsol_curve_algs.h"
#include <dbsol/dbsol_interp_curve_2d.h>
#include <dbgl/dbgl_poly_curve_line.h>
#include <dbgl/dbgl_arc.h>
#include <dbgl/dbgl_eno_curve.h>
#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_box_2d.h>

#include <vcl_iostream.h>
#include <dbnl/algo/dbnl_eno.h>
#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>
#include <vnl/algo/vnl_svd.h>

#include <vgl/algo/vgl_fit_lines_2d.h>
#include <vgl/vgl_distance.h>
#include <vnl/vnl_random.h>
#include <vcl_ctime.h>

//: Destructor
dbsol_curve_algs::~dbsol_curve_algs()
{
}

//-------------------------------------------------------------
//: sample the input curve at the rate length()/size
bool dbsol_curve_algs::
sample(dbsol_interp_curve_2d const &c, int size,
       vcl_vector<vsol_point_2d_sptr>& pts)
{
  double L = c.length();
  double ds = L/size;
#ifndef NDEBUG
  vcl_cout << "Length of curve: " << L << " rate of sampling is: " << ds << vcl_endl;
  vcl_cout << "Output size wanted is: " << size << vcl_endl;
#endif

  pts.reserve(size);

  //for (int i = 0; i<c.intervals().size(); i++)
  //  vcl_cout << "interval: " << i << "  length: " << (*c)[i].length() << vcl_endl; 

  double sum = 0;
  while (sum <= L)
  {
    pts.push_back(c.point_at(sum));
    //vcl_cout << "sum: " << sum << " point: " << *(c.point_at(sum)) << vcl_endl;
    sum += ds;
  }

  if (*(c.point_at(0)) != *(c.point_at(L)))  // if not closed curve
    pts.push_back(c.point_at(L));  

#ifndef NDEBUG
  vcl_cout << "after sampling pts size: " << pts.size() << " (should be " << size << ")\n";
#endif
  return true;

}

//-------------------------------------------------------------
//: sample the input curve at the rate length()/size
//: return the samples and the tangents (in radian) at those samples
//  no max length threshold to sample if negative
bool dbsol_curve_algs::
sample(dbsol_interp_curve_2d const &c, double ds,
       vcl_vector<vsol_point_2d_sptr>& pts, vcl_vector<double>& tangents, double length_threshold)
{
  double L;
  if (length_threshold > 0)
    L = length_threshold > c.length() ? c.length() : length_threshold;
  else
    L = c.length();

  // there is a run time bug in gcc 3.4.6 compiler when sum starts exactly from zero, please keep this as is
  //double sum = 0;
  double sum = ds/1000;  
  while (sum <= L)
  {
    pts.push_back(c.point_at(sum));
    tangents.push_back(c.tangent_angle_at(sum));
    sum += ds;
  }

  return true;
}

//: sample a "region" evenly around this interp curve, 
// (including the points right on the curve is optional, if not they start from delta away on the normal direction)
void dbsol_curve_algs::
sample_region_along_curve(dbsol_interp_curve_2d const &c, vcl_vector<vsol_point_2d_sptr>& region_pts, double delta, double length_threshold, float width, bool add_curve_points)
{
  vcl_vector<double > tangents;
  vcl_vector<vsol_point_2d_sptr> curve_pts;
  sample(c, delta, curve_pts, tangents, length_threshold);  

  //: for each point, sample along the line that is normal to the curve as far as "width"
  //width += ds; // make sure that points at "width" are also used
  vcl_vector<vsol_point_2d_sptr> plus_pts, minus_pts;  // plus from either side of the contour will be passed differently
  // so that user can pass them to different procedures
  // but if curve points are also added half of curve points 
  // will be considered in plus region and the other half in minus region
  // because there is no way of knowing how many curve points will be added
  for (unsigned j = 0; j < curve_pts.size(); j++) {  
    vgl_point_2d<double> pt = curve_pts[j]->get_p();
    
    //double normal = angle0To2Pi(tangents[j]+vnl_math::pi/2.0f);
    double normal = tangents[j]+vnl_math::pi/2.0f;
    double sum = delta;
    while (sum <= width) {
      vsol_point_2d_sptr new_pt1 = new vsol_point_2d(pt.x() + sum*cos(normal), pt.y() + sum*sin(normal));
      vsol_point_2d_sptr new_pt2 = new vsol_point_2d(pt.x() - sum*cos(normal), pt.y() - sum*sin(normal));
      plus_pts.push_back(new_pt1);
      minus_pts.push_back(new_pt2);
      sum += delta;
    }
  }

  if (add_curve_points)
    region_pts.insert(region_pts.end(), curve_pts.begin(), curve_pts.end());

  region_pts.insert(region_pts.end(), plus_pts.begin(), plus_pts.end());
  region_pts.insert(region_pts.end(), minus_pts.begin(), minus_pts.end());
}

bool dbsol_curve_algs::
interpolate_linear(dbsol_interp_curve_2d *c, vcl_vector<vsol_point_2d_sptr> const &pts, 
                   bool closed)
{
  if (!closed) {
    vcl_vector<dbgl_param_curve *> ints(pts.size()-1);

    for (unsigned i=0; i<ints.size(); i++) {
      ints[i] = new dbgl_poly_curve_line(pts[i]->get_p(), pts[i+1]->get_p());
    }

    c->make(ints);
  } else {

    vcl_vector<dbgl_param_curve *> ints(pts.size());

    unsigned i;
    //: ozge changed all the loops in the following fashion with no paranthesis to the one below
    //  there is a bug in an older version of gcc which is still used on one of our linux machines
    //for (unsigned i=0; i<ints.size(); ++i) 
    for (i=0; i<ints.size()-1; i++) {
      ints[i] = new dbgl_poly_curve_line(pts[i]->get_p(), pts[i+1]->get_p());
    }
    ints[i] = new dbgl_poly_curve_line(pts[i]->get_p(), pts[0]->get_p());

    c->make(ints);
  }

  return true;
}

bool dbsol_curve_algs::
interpolate_linear(dbsol_interp_curve_2d *c, vcl_vector<vgl_point_2d<double> > const &pts, bool closed)
{
  if (!closed) {
    vcl_vector<dbgl_param_curve *> ints(pts.size()-1);

    //: ozge changed all the loops in the following fashion with no paranthesis to the one below
    //  there is a bug in an older version of gcc which is still used on one of our linux machines
    //for (unsigned i=0; i<ints.size(); ++i) 
    for (unsigned i=0; i<ints.size(); i++) { 
      ints[i] = new dbgl_poly_curve_line(pts[i], pts[i+1]);
    }

    c->make(ints);
  } else {

    vcl_vector<dbgl_param_curve *> ints(pts.size());

    unsigned i;
    for (i=0; i<ints.size()-1; i++) {
      ints[i] = new dbgl_poly_curve_line(pts[i], pts[i+1]);
    }
    ints[i] = new dbgl_poly_curve_line(pts[i], pts[0]);

    c->make(ints);
  }

  return true;
}

bool dbsol_curve_algs::interpolate_linear(dbsol_interp_curve_2d *c,
                                          vsol_polygon_2d_sptr poly)
{
  vcl_vector<dbgl_param_curve *> ints(poly->size());

  unsigned i;
  for (i=0; i<poly->size()-1; i++) {
    ints[i] = new dbgl_poly_curve_line(poly->vertex(i)->get_p(), poly->vertex(i+1)->get_p());
  }
  ints[i] = new dbgl_poly_curve_line(poly->vertex(i)->get_p(), poly->vertex(0)->get_p());

  c->make(ints);
  return true;
}


bool dbsol_curve_algs::interpolate_linear(dbsol_interp_curve_2d *c,
                                 vsol_polyline_2d_sptr poly)
{
  vcl_vector<dbgl_param_curve *> ints(poly->size()-1);

  unsigned i;
  for (i=0; i<poly->size()-1; i++) {
    ints[i] = new dbgl_poly_curve_line(poly->vertex(i)->get_p(), poly->vertex(i+1)->get_p());
  }
  
  c->make(ints);
  return true;
}

bool dbsol_curve_algs::
interpolate_linear2(dbsol_interp_curve_2d *c, vcl_vector<vsol_point_2d_sptr> const &pts, 
                    bool closed, int time)
{
  if (!closed) {
    vcl_vector<dbgl_param_curve *> ints(pts.size()-1);

    for (unsigned i=0; i<ints.size(); i++) {
      ints[i] = new dbgl_poly_curve_line(pts[i]->get_p(), pts[i+1]->get_p());
    }

    c->make(ints);
  } else {

    vcl_vector<dbgl_param_curve *> ints(pts.size());

    int size_looptarget = static_cast<int>(ints.size())-1;
    int i, j;
    for (i=0, j = time; j<size_looptarget; i++, j++) {
      ints[i] = new dbgl_poly_curve_line(pts[j]->get_p(), pts[j+1]->get_p());
    }
    ints[i] = new dbgl_poly_curve_line(pts[j]->get_p(), pts[0]->get_p());
    for (i++, j = 0; j<time; i++, j++) {
      ints[i] = new dbgl_poly_curve_line(pts[j]->get_p(), pts[j+1]->get_p());
    }

    c->make(ints);
  }

  return true;
}

bool dbsol_curve_algs::interpolate_eno(dbsol_interp_curve_2d *c, 
                                       vcl_vector<vsol_point_2d_sptr> const &pts,
                                       vnl_vector<double> &sample_pts)
{
  int num_points = pts.size();

  vnl_vector<double> data_x(num_points);
  vnl_vector<double> data_y(num_points);
  // interpolate along x
  for(int i=0; i < num_points; i++)
  {
    data_x(i) = pts[i]->get_p().x();
    data_y(i) = pts[i]->get_p().y();
  }

  return interpolate_eno_main(c, data_x, data_y, sample_pts);
}

bool dbsol_curve_algs::interpolate_eno(dbsol_interp_curve_2d *c, 
                                       vcl_vector<vsol_point_2d> const &pts,
                                       vnl_vector<double> &sample_pts)
{
  int num_points = pts.size();

  vnl_vector<double> data_x(num_points);
  vnl_vector<double> data_y(num_points);
  // interpolate along x
  for(int i=0; i < num_points; i++)
  {
    data_x(i) = pts[i].get_p().x();
    data_y(i) = pts[i].get_p().y();
  }

  return interpolate_eno_main(c, data_x, data_y, sample_pts);
}

bool dbsol_curve_algs::interpolate_eno(dbsol_interp_curve_2d *c, 
                                       vcl_vector<vgl_point_2d<double> > const &pts,
                                       vnl_vector<double> &sample_pts)
{
  int num_points = pts.size();

  vnl_vector<double> data_x(num_points);
  vnl_vector<double> data_y(num_points);
  // interpolate along x
  for(int i=0; i < num_points; i++)
  {
    data_x(i) = pts[i].x();
    data_y(i) = pts[i].y();
  }

  return interpolate_eno_main(c, data_x, data_y, sample_pts);
}

bool dbsol_curve_algs::interpolate_eno_main(dbsol_interp_curve_2d *c,
                                            vnl_vector<double> const &data_x,
                                            vnl_vector<double> const &data_y,
                                            vnl_vector<double> &sample_pts)
{
  int num_points = data_x.size();
  // compute sampling point indices using the simple arclength between each datapoint
  // if the sample_pts array is empty
  if(sample_pts.size() == 0)
  {
    sample_pts.set_size(num_points);
    double cumulative_length = 0.0;
    sample_pts(0) = cumulative_length;
    for(int i=1; i < num_points; i++)
    {
      double diffx2 = vcl_pow(data_x[i] - data_x[i-1], 2.0);
      double diffy2 = vcl_pow(data_y[i] - data_y[i-1], 2.0);
      double length = vcl_sqrt(diffx2 + diffy2);
      cumulative_length += length;
      sample_pts(i) = cumulative_length;
    }
  }

  vcl_vector<dbgl_param_curve *> ints(num_points-1);
  vcl_vector<double> coefs_x;
  vcl_vector<double> coefs_y;

  if(num_points > 3)
  {
    dbnl_eno_1d eno_x;
    dbnl_eno_1d eno_y;

    eno_x.interpolate(&data_x, &sample_pts);
    eno_y.interpolate(&data_y, &sample_pts);

    // get the interpolants
    vcl_vector<dbnl_eno_interp> eno_interp_x = eno_x.interp();
    vcl_vector<dbnl_eno_interp> eno_interp_y = eno_y.interp();

    for(int i=0; i < num_points-1; i++)
    {
      for(int j=0; j<3; j++)
      {
        coefs_x.push_back(eno_interp_x.at(i).coeff(j));
        coefs_y.push_back(eno_interp_y.at(i).coeff(j));
      }
      ints[i] = new dbgl_eno_curve(coefs_x, coefs_y, sample_pts[i], sample_pts[i+1]);
      coefs_x.clear();
      coefs_y.clear();
    }
  }
  else if(num_points == 3)
  {
    vnl_matrix<double> y1(3,1,0.0);
    y1(0,0) = data_x(0); y1(1,0) = data_x(1); y1(2,0) = data_x(2);

    vnl_matrix<double> y2(3,1,0.0);
    y2(0,0) = data_y(0); y2(1,0) = data_y(1); y2(2,0) = data_y(2);
    
    vnl_matrix<double> x(3,3,1.0);
    x(0,0) = vcl_pow(sample_pts(0),2.0); x(0,1) = sample_pts(0);
    x(1,0) = vcl_pow(sample_pts(1),2.0); x(1,1) = sample_pts(1);
    x(2,0) = vcl_pow(sample_pts(2),2.0); x(2,1) = sample_pts(2);

    vnl_svd<double> A(x);
    vnl_matrix<double> cx = A.solve(y1);

    vnl_matrix<double> cy = A.solve(y2);

    for(int i=0; i < num_points-1; i++)
    {
      for(int j=2; j>=0; j--)
      {
        coefs_x.push_back(cx(j,0));
        coefs_y.push_back(cy(j,0));
      }
      ints[i] = new dbgl_eno_curve(coefs_x, coefs_y, sample_pts[i], sample_pts[i+1]);
      coefs_x.clear();
      coefs_y.clear();
    }
  }
  else if(num_points == 2)
  {
    double x1 = sample_pts(0); double x2 = sample_pts(1);
    double y1 = data_x(0); double y2 = data_x(1);
    double a = (y1-y2) / (x1-x2);
    double b = (-x2*y1+x1*y2) / (x1-x2);
    coefs_x.push_back(b);
    coefs_x.push_back(a);
    coefs_x.push_back(0.0);

    y1 = data_y(0); y2 = data_y(1);
    a = (y1-y2) / (x1-x2);
    b = (-x2*y1+x1*y2) / (x1-x2);
    coefs_y.push_back(b);
    coefs_y.push_back(a);
    coefs_y.push_back(0.0);

    ints[0] = new dbgl_eno_curve(coefs_x, coefs_y, sample_pts[0], sample_pts[1]);
  }
  else
  {
    vcl_cout << "Check the number of data points specified for ENO interpolation" << vcl_endl;
    return false;
  }
  c->make(ints);

  return true;
}

//: returns 0 if anything goes wrong, check the output pointer!
vsol_polyline_2d_sptr 
fit_lines_to_contour(vsol_polyline_2d_sptr poly, double rms) {
  if (rms > 0 && poly->size() > 0) {  // vgl_fit
    int min_fit_length = 2;
    vgl_fit_lines_2d<double> fitter; 
    fitter.set_min_fit_length(min_fit_length);
    fitter.set_rms_error_tol(rms);
    //vcl_cout << "original polyline size: " << poly->size() << " ";
    for (unsigned int i = 0; i<poly->size(); i++) {
      vgl_point_2d<double> p = poly->vertex(i)->get_p();
      fitter.add_point(p);
    }
    fitter.fit();
    vcl_vector<vgl_line_segment_2d<double> >& segs = fitter.get_line_segs();
    if (!segs.size())
      return 0;

    vcl_vector<vsol_point_2d_sptr > new_pts;
    new_pts.push_back(new vsol_point_2d(segs[0].point1().x(),segs[0].point1().y()));
    new_pts.push_back(new vsol_point_2d(segs[0].point2().x(),segs[0].point2().y()));
    for (unsigned int i = 1; i<segs.size(); i++) {
      new_pts.push_back(new vsol_point_2d(segs[i].point2().x(),segs[i].point2().y()));
    }
    //vcl_cout << "fitted polyline size: " << new_pts.size() << vcl_endl;
    return new vsol_polyline_2d(new_pts);
 } else
   return 0;
}

//: returns 0 if anything goes wrong, check the output pointer!
vsol_polygon_2d_sptr fit_lines_to_contour(vsol_polygon_2d_sptr poly, double rms) {
  if (rms > 0 && poly->size() > 0) {  // vgl_fit
    int min_fit_length = 2;
    vgl_fit_lines_2d<double> fitter; 
    fitter.set_min_fit_length(min_fit_length);
    fitter.set_rms_error_tol(rms);
    //vcl_cout << "original polygon size: " << poly->size() << " ";
    for (unsigned int i = 0; i<poly->size(); i++) {
      vgl_point_2d<double> p = poly->vertex(i)->get_p();
      fitter.add_point(p);
    }
    fitter.fit();
    vcl_vector<vgl_line_segment_2d<double> >& segs = fitter.get_line_segs();
    if (!segs.size())
      return 0;

    vcl_vector<vsol_point_2d_sptr > new_pts;
    new_pts.push_back(new vsol_point_2d(segs[0].point1().x(),segs[0].point1().y()));
    new_pts.push_back(new vsol_point_2d(segs[0].point2().x(),segs[0].point2().y()));
    for (unsigned int i = 1; i<segs.size(); i++) {
      new_pts.push_back(new vsol_point_2d(segs[i].point2().x(),segs[i].point2().y()));
    }
    //vcl_cout << "fitted polygon size: " << new_pts.size() << vcl_endl;
    return new vsol_polygon_2d(new_pts);
 } else
   return 0;
}

//: returns a vector of size 0 if anything goes wrong
void fit_lines_to_contour(vcl_vector<vsol_point_2d_sptr>& poly, double rms,
                          vcl_vector<vsol_point_2d_sptr>& new_pts) 
{
  new_pts.clear();

  if (rms > 0 && poly.size() > 0) {  // vgl_fit
    int min_fit_length = 2;
    vgl_fit_lines_2d<double> fitter; 
    fitter.set_min_fit_length(min_fit_length);
    fitter.set_rms_error_tol(rms);
    //vcl_cout << "original polygon size: " << poly->size() << " ";
    for (unsigned int i = 0; i<poly.size(); i++) {
      vgl_point_2d<double> p = poly[i]->get_p();
      fitter.add_point(p);
    }
    fitter.fit();
    vcl_vector<vgl_line_segment_2d<double> >& segs = fitter.get_line_segs();

    if (segs.size()) {
      new_pts.push_back(new vsol_point_2d(segs[0].point1().x(),segs[0].point1().y()));
      new_pts.push_back(new vsol_point_2d(segs[0].point2().x(),segs[0].point2().y()));
      for (unsigned int i = 1; i<segs.size(); i++) {
        new_pts.push_back(new vsol_point_2d(segs[i].point2().x(),segs[i].point2().y()));
      }
      //vcl_cout << "fitted polygon size: " << new_pts.size() << vcl_endl;
    }
 }
}

//: delete k = ~log_2(perc_d/divident) many segments from p whose total length is ~perc_d percent of the original length
//  the mean length of each deleted segment is ~perc_d/k percent of the total length with variance perc_sigma
//  fromula from [Ghosh, Petkov, PAMI05], the divident is fixed at 8 in that paper
bool dbsol_curve_algs::
segment_wise_deletion(const vsol_polygon_2d_sptr& p, 
             vcl_vector<vsol_polyline_2d_sptr>& pieces, 
             vcl_vector<vsol_polyline_2d_sptr>& del_pieces, float perc_d, float perc_sigma, float divident)
{
  vcl_cout << "in segment_wise_deletion\n";
  if (!(p->size() > 3)) {
    vcl_cout << "In dbsol_algos::segment_wise_deletion() - polygon has too few vertices\n";
    return false;
  }

  dbsol_interp_curve_2d curve;
  dbsol_curve_algs::interpolate_linear(&curve, p);

  float length = (float)curve.length();
  vcl_cout << "curve length:" << length << vcl_endl;

  //int k = (int)vcl_ceil(log(perc_d/8.0)/log(2.0));
  int k = (int)vcl_ceil(log(perc_d/divident)/log(2.0));
  if (!k)
    k = 1;
  vcl_cout << "will delete " << k << " segments\n";

  //: delete k segments with a total of perc_d percent of the length 
  float to_be_deleted = (perc_d * length)/100.0f;
  vcl_cout << "total length: " << length << " deletion percentage: " << perc_d << " will delete: " << to_be_deleted << vcl_endl;
  
  float mean = to_be_deleted/k;
  vcl_cout << "mean to be deleted: " << mean << vcl_endl;

  //: keep a bool list for each vertex id whether it has been deleted or not
  unsigned vert_size = p->size();
  vcl_cout << "there are " << vert_size << " vertices in the polygon\n";
  vcl_vector<bool> deleted_vertices(vert_size, false);

  vnl_random gen;
  gen.reseed((unsigned long)vcl_time(NULL));
    
  for (int i = 0; i < k; i++)
  {  
    double x = gen.normal();  // X ~ N(0,1)

    // I want Y ~ N(mean_to_be_deleted, perc_sigma^2), 
    // if X ~ N(0, 1^2) then a*X + b ~ N(a*0 + b, (a*1)^2)
    // then set mean_to_be_deleted = b, and perc_sigma = a and get Y = aX + b
    double y = perc_sigma*x + mean;
    vcl_cout << "i: " << i << " y: " << y << vcl_endl;

    //: create a vector of possible starting points for deletion which has
    //  enough room for deletion up untill the first deleted segment
    vcl_vector<unsigned> possibilities;
    for (unsigned jj = 0; jj < deleted_vertices.size(); jj++) {
      if (deleted_vertices[jj])
        continue;
      //: find the next deleted one after this
      bool passed_origin = false;
      bool encountered_a_deleted_vertex = false;
      for (unsigned kk = jj + 1; kk != jj ; kk++) {
        if (kk == deleted_vertices.size()) {
          kk = 0;  // go back to vertex 0 if reached end of the polygon
          passed_origin = true;
          if (kk == jj)
            break;
        }

        if (deleted_vertices[kk]) {
          if (!passed_origin && (curve.length_at(kk) - curve.length_at(jj)) > y) {
            possibilities.push_back(jj);
          } else if (passed_origin && (curve.length_at(kk) + (length - curve.length_at(jj))) > y) {
            possibilities.push_back(jj);
          }
          encountered_a_deleted_vertex = true;
          break;
        }
      }
      if (!encountered_a_deleted_vertex)
        possibilities.push_back(jj);
    }
      
    if (!possibilities.size()) {
      vcl_cout << "cannot find any starting point with enough length\n";
      return false;
    }

    //: now pick one of the possibilities randomly
    unsigned ind = (unsigned)gen.drand32(0, possibilities.size()-1);
    ind = possibilities[ind];  // assign the index in the possibilities
      
    bool passed_origin = false;
    deleted_vertices[ind] = true;
    for (unsigned kk = ind + 1; kk != ind ; kk++) {
      if (kk == deleted_vertices.size()) {
        kk = 0;  // go back to vertex 0 if reached end of the polygon
        passed_origin = true;
        if (kk == ind)
          break;
      }
      if (!passed_origin) {
        if ((curve.length_at(kk) - curve.length_at(ind)) <= y)
          deleted_vertices[kk] = true;
        else
          break;
      } else {
        if ((curve.length_at(kk) + (length - curve.length_at(ind))) <= y)
          deleted_vertices[kk] = true;
        else
          break;
      }      
    }
    
  }

  //: now create the deleted and remaining polyline sets
  pieces.clear();
  del_pieces.clear();
  //: first fill the pieces (non-deleted portions)
    // if 0 is non-deleted skip till hit a deleted one
  unsigned jj = 0;
  for ( ; jj < deleted_vertices.size(); jj++)  
    if (deleted_vertices[jj])
      break;

  while (jj < deleted_vertices.size()) {
    
    for ( ; jj < deleted_vertices.size(); jj++)  // find next remaining piece
      if (!deleted_vertices[jj])
        break;
    
    if (jj < deleted_vertices.size()) {
      vcl_vector<vsol_point_2d_sptr> pts;
      pts.push_back(p->vertex(jj));
      for (unsigned kk = jj + 1; kk != jj; kk++) {
        if (kk == deleted_vertices.size()) {
          kk = 0;
          if (kk == jj)
            break;
        }
        if (deleted_vertices[kk])
          break;
        pts.push_back(p->vertex(kk));
      }
      vsol_polyline_2d_sptr pline = new vsol_polyline_2d(pts);
      pieces.push_back(pline);
      jj += pts.size();
    }

  }
  
  //: now fill the del_pieces (deleted portions)
    // if 0 is deleted skip till hit a non-deleted one
  jj = 0;
  for ( ; jj < deleted_vertices.size(); jj++)  
    if (!deleted_vertices[jj])
      break;

  while (jj < deleted_vertices.size()) {
    
    for ( ; jj < deleted_vertices.size(); jj++)  // find next deleted piece
      if (deleted_vertices[jj])
        break;
    
    if (jj < deleted_vertices.size()) {
      vcl_vector<vsol_point_2d_sptr> pts;
      pts.push_back(p->vertex(jj));
      for (unsigned kk = jj + 1; kk != jj; kk++) {
        if (kk == deleted_vertices.size()) {
          kk = 0;
          if (kk == jj)
            break;
        }
        if (!deleted_vertices[kk])
          break;
        pts.push_back(p->vertex(kk));
      }
      vsol_polyline_2d_sptr pline = new vsol_polyline_2d(pts);
      del_pieces.push_back(pline);
      jj += pts.size();
    }
  }

  return true;
  }

//: add k = ~log_2(perc_d/divident) many segments to p whose total length is ~perc_d percent of the original length
//  the mean length of each newly added segment is ~perc_d/k percent of the total length with variance perc_sigma
//  add the segment with a random orientation from x-axis at a random region in or out of the contour
//  output is a spatial object vector to be saved as a .cem file
bool dbsol_curve_algs::segment_addition(const vsol_polygon_2d_sptr& p,
                        vcl_vector<vsol_spatial_object_2d_sptr>& pieces,
                        float perc_d, float perc_sigma, float divident)
{

  vcl_cout << "in segment_addition\n";
  if (!(p->size() > 3)) {
    vcl_cout << "In dbsol_algos::segment_addition() - polygon has too few vertices\n";
    return false;
  }

  dbsol_interp_curve_2d curve;
  dbsol_curve_algs::interpolate_linear(&curve, p);

  float length = (float)curve.length();
  vcl_cout << "curve length:" << length << vcl_endl;

  //int k = (int)vcl_ceil(log(perc_d/8.0)/log(2.0));
  int k = (int)vcl_ceil(log(perc_d/divident)/log(2.0));
  //int k = (int)vcl_ceil((perc_d*length)/divident);
  if (!k)
    k = 1;
  vcl_cout << "will add " << k << " segments\n";

  //: add k segments with a total of perc_d percent of the length 
  float to_be_added = (perc_d * length)/100.0f;
  vcl_cout << "total length: " << length << " addition percentage: " << perc_d << " will add: " << to_be_added << vcl_endl;
  
  float mean = to_be_added/k;
  vcl_cout << "mean to be added: " << mean << vcl_endl;

  // find the area of the region
  p->compute_bounding_box();
  vsol_box_2d_sptr box = p->get_bounding_box();
  // enlarge the box to be able to add segments on the boundary
  //box->add_point(box->get_min_x()-(box->width()/10.0), box->get_min_y()-(box->height()/10.0));
  //box->add_point(box->get_max_x()+(box->width()/10.0), box->get_max_y()+(box->height()/10.0));
  float box_minx = (float)box->get_min_x();
  float box_miny = (float)box->get_min_y();
  float box_width = (float)box->width();
  float box_height = (float)box->height();

  vnl_random gen;
  //gen.reseed((unsigned long)vcl_time(NULL));
  gen.reseed((unsigned long)vcl_clock() + (unsigned long)vcl_time(NULL));
    
  pieces.clear();
  pieces.push_back(p->cast_to_spatial_object());

  for (int i = 0; i < k; i++)
  {  
    double x = gen.normal();  // X ~ N(0,1)

    // I want Y ~ N(mean_to_be_added, perc_sigma^2), 
    // if X ~ N(0, 1^2) then a*X + b ~ N(a*0 + b, (a*1)^2)
    // then set mean_to_be_added = b, and perc_sigma = a and get Y = aX + b
    double y = perc_sigma*x + mean;
    vcl_cout << "i: " << i << " y: " << y << vcl_endl;

    //: find a random pt1 for the line segment
    float rw = (float)gen.drand32(0, box_width);
    float rx = box_minx + rw;
    float rh = (float)gen.drand32(0, box_height);
    float ry = box_miny + rh;

    //: find a random orientation
    vgl_vector_2d<float> v(0, 1);
    float random_angle = (float)gen.drand32(0, 2*vnl_math::pi);
    vgl_vector_2d<float> r_v = rotated(v, random_angle);
    vgl_vector_2d<float> r_final_v = r_v*y;
    vgl_point_2d<float> pt1;
    pt1.set(rx, ry);
    vgl_point_2d<float> pt2 = pt1 + r_final_v;

    vcl_vector<vsol_point_2d_sptr> pts;
    pts.push_back(new vsol_point_2d(rx, ry));
    pts.push_back(new vsol_point_2d(pt2.x(), pt2.y()));
    vsol_polyline_2d_sptr poly = new vsol_polyline_2d(pts);
    pieces.push_back(poly->cast_to_spatial_object());
  }

  return true;
}

 
//: add k = ~ (perc_d*total_length)/divident many segments to p whose total length is ~perc_d percent of the original length
//  the mean length of each newly added segment is ~perc_d/k percent of the total length with variance perc_sigma
//  add the segment with a random orientation from x-axis at a random region in or out of the contour
//  and
//  delete k = ~log_2(perc_d/divident) many segments from p whose total length is ~perc_d percent of the original length
//  the mean length of each deleted segment is ~perc_d/k percent of the total length with variance perc_sigma
//  fromula from [Ghosh, Petkov, PAMI05], the divident is fixed at 8 in that paper
//  output is a spatial object vector to be saved as a .cem file
bool dbsol_curve_algs::segment_addition_and_deletion(const vsol_polygon_2d_sptr& p, 
                                            vcl_vector<vsol_spatial_object_2d_sptr>& pieces, 
                                            vcl_vector<vsol_polyline_2d_sptr>& del_pieces,
                                            float add_perc_d, float add_perc_sigma, float add_divident,
                                            float del_perc_d, float del_perc_sigma, float del_divident)
{
  vcl_vector<vsol_polyline_2d_sptr> temp_pieces;
  
  //: first delete the segments
  del_pieces.clear();
  if (!segment_wise_deletion(p, temp_pieces, del_pieces, del_perc_d, del_perc_sigma, del_divident))
    return false;

  pieces.clear();
  if (!segment_addition(p, pieces, add_perc_d, add_perc_sigma, add_divident))
    return false;

  //: delete the first piece which is the original polygon
  pieces.erase(pieces.begin());

  //: now add the temp_pieces
  for (unsigned i = 0; i < temp_pieces.size(); i++) {
    pieces.push_back(temp_pieces[i]->cast_to_spatial_object());
  }

  return true;
}

//: This function subsamples the points in the contour according to the c_ds value
//  just like Thomas sebastian did
vsol_polygon_2d_sptr dbsol_subsample_contour(vsol_polygon_2d_sptr poly, double c_ds)
{
  vcl_vector<vsol_point_2d_sptr > new_pts;

  vgl_point_2d<double> last_pt = poly->vertex(0)->get_p();
  for (unsigned int i = 1; i<poly->size(); i++){
    vgl_point_2d<double> pt = poly->vertex(i)->get_p();
 
    if (vgl_distance(last_pt, pt)>c_ds){
      new_pts.push_back(poly->vertex(i));
      last_pt = pt;
    }
  }
  
  return new vsol_polygon_2d(new_pts);
}

//: This function subsamples the points in the contour according to the c_ds value
vsol_polygon_2d_sptr dbsol_subsample_contour_smart(vsol_polygon_2d_sptr poly, double c_ds)
{
  vcl_vector<vsol_point_2d_sptr > new_pts;
  double d = 0;
  for (unsigned int i = 1; i<poly->size(); i++)
  {
    vgl_point_2d<double> pt = poly->vertex(i)->get_p();
    vgl_point_2d<double> pt_before = poly->vertex(i-1)->get_p();

    d += vgl_distance(pt_before, pt);
    if (d > c_ds){
      new_pts.push_back(poly->vertex(i));
      d = 0;
    }
  }  
  return new vsol_polygon_2d(new_pts);
}

