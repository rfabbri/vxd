// This is becld_epiband.h
#ifndef becld_epiband_h
#define becld_epiband_h
//:
//\file
//\brief Epipolar error search region for matching
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 10/01/06 16:37:39 EDT
//

#include <vpgl/vpgl_fundamental_matrix.h>
#include <vgl/vgl_polygon.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_box_2d.h>
#include <vgl/vgl_area.h>

class becld_epipole;

//: Represents epipolar search region as a 2D cone (sector) of epipolar lines
// around the predicted epipolar line.  The width of the cone can be thought of as reflecting the
// localization error of a point that corresponds to this epipolar line on another image. This class
// supports efficient computation of the epipolar band, as well as its intersection with other
// epipolar bands, determining an optimal region for correspondence search. 
//
// \seealso becld_epiband_iterator for a class that can traverse integer coordinates
// corresponding to the epipolar band, allowing for efficient search of image features along the
// epipolar band.
//
// A polygon poly_ is stored, which is a finite representation of this
// sector. If you run through the pixels of these polygons (with clipping region equal to the image
// size), you get integer positions in the image where the search for correspondences shall be
// conducted.
//
// This class allows for fast intersection of epipolar search regions (say, coming from two other
// images into the current image), as well as very efficient access to pixel positions to be
// searched.
//
// If you want to search subpixel positions inside the polygon, I suggest you bucket your subpixels
// into pixel buckets then traverse the bucket's integer coordinates. 
//
// 
// \sealso notes at /vision/projects/kimia/multiview/rfabbri/doc/epipolar-constraint-coding.tex
//
class becld_epiband {
public:

  //: example of bounding box: xmin = ymin = 0; xmax = ncols-1;  ymax = nrows-1 of an image
  becld_epiband (const vgl_box_2d<double> &b) : box_(b) {}

  //: Copy constructor
  becld_epiband (becld_epiband const & b)
    :
      poly_(b.poly_),
      box_(b.box_),
      err_pos_(b.err_pos_)
  { 
  }

  double
  compute(const vgl_point_2d<double> &p, const vpgl_fundamental_matrix<double> &fm, double err_pos);

  double 
  compute(const becld_epiband &eb_a, const vpgl_fundamental_matrix<double> &fm_ab, double err_pos);

  vgl_box_2d<double> window() const { return box_; }

  bool contains(const vgl_point_2d<double> &p) const;

  double distance(const vgl_point_2d<double> &p) const;

  bool intersect( const becld_epiband &a, const becld_epiband &b);

  double get_err_theta(double err_pos, vgl_homg_line_2d<double> &l_minus, vgl_homg_line_2d<double> &l_plus);

  double err_pos() const {return err_pos_;}

  friend class becld_epiband_iterator;
  
  //: Implementation-dependent: 
  const vgl_polygon<double> &polygon() const { return poly_; }

  double area() const { return vgl_area<double>(polygon()); }

  bool is_empty() const { return poly_.num_sheets() == 0 || poly_[0].size() == 0; }

  static void get_bounds(
     const vgl_polygon<double> &poly,
     const becld_epipole &ep,
     double &theta_plus,
     double &theta_minus,
     double &theta_mean
      );

protected:
  vgl_polygon<double> poly_;
  vgl_box_2d<double> box_;
  double err_pos_;
};

#endif // becld_epiband_h
