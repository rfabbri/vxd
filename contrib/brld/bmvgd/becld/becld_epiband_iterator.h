// This is dbecl_epiband_iterator.h
#ifndef dbecl_epiband_iterator_h
#define dbecl_epiband_iterator_h
//:
//\file
//\brief Iterator to go over all integer coordinates of an epipolar band
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 10/01/06 17:33:09 EDT
//

#include <vgl/vgl_polygon_scan_iterator.h>

#include <vgl/vgl_box_2d.h>
#include <vcl_stack.h>
#include <vcl_map.h>
#include <vcl_utility.h>
#include <vil/vil_image_view.h>
#include <vcl_cmath.h>


struct dbecl_grid_cover_window {
  enum pixel_state {
    in_stack,
    unvisited,
    popped,
    removed,
  };

  //: Will restrict grid_cover_iterator coordinates to be within window w (inclusive bound).
  // For regular images, you can set w.min_x = 0, w.max_x = n_cols-1; and similarly for y, with dmax
  // = 0. THIS DOESN't MEAN dmax will be the same for the polygon_grid_iterator.
  //
  // For polygon bounding boxes, just pass the floating-point bbox, then we will compute floor of
  // the mininum, and ceil of the maximum values.
  //
  // If you want to cover all integer positions at a certain distance from the polygon,
  // we restrict cover_iterator to a window computed this from the polygon's bbox plus a distance
  // from it.
  dbecl_grid_cover_window(const vgl_box_2d<double> &bbox, double dmax=1.5) 
    :
    win_((int)vcl_floor(bbox.min_x() - dmax), (int)vcl_ceil(bbox.max_x() + dmax), (int)vcl_floor(bbox.min_y() - dmax), (int)vcl_ceil(bbox.max_y() + dmax)),
    label_buf_(win_.max_x() - win_.min_x()+1,win_.max_y() - win_.min_y()+1)
  {
    label_buf_.fill(unvisited);
  }

  //: Convenience, based on computing bbox of the polygon
  inline dbecl_grid_cover_window(const vgl_polygon<double> &poly, double dmax=1.5);

  pixel_state label(vgl_point_2d<int> &p) const 
  { 
      return (pixel_state)label_buf_(p.x() - win_.min_x(), p.y() - win_.min_y()); 
  }

  void set_label(vgl_point_2d<int> &p, pixel_state l) 
  { 
    label_buf_(p.x() - win_.min_x(),p.y() - win_.min_y()) = l; 
  }

  bool valid_x (int x) const { return x >= win_.min_x() && x <= win_.max_x(); }
  bool valid_y (int y) const { return y >= win_.min_y() && y <= win_.max_y(); }

  bool is_label_buf_clean() const;

  void print() const; 

public:
  vgl_box_2d<int> win_;
  vil_image_view<int> label_buf_;
};

inline dbecl_grid_cover_window::
dbecl_grid_cover_window(const vgl_polygon<double> &poly, double dmax)
{
    if (poly.num_sheets() && poly[0].size())  {
      double min_x = poly[0][0].x();
      double min_y = poly[0][0].y();
      double max_x = min_x;
      double max_y = min_y;

      for (unsigned is=0; is < poly.num_sheets(); ++is) {
        const vcl_vector<vgl_point_2d<double> > &sheet = poly[is];
        //: determine bbox

        for (unsigned i=1; i < sheet.size(); ++i) {
          if (sheet[i].x() > max_x)
            max_x = sheet[i].x();
          if (sheet[i].y() > max_y)
            max_y = sheet[i].y();
          if (sheet[i].x() < min_x)
            min_x = sheet[i].x();
          if (sheet[i].y() < min_y)
            min_y = sheet[i].y();
        }
      }

      win_ = vgl_box_2d<int>((int)vcl_floor(min_x - dmax), (int)vcl_ceil(max_x + dmax), (int)vcl_floor(min_y - dmax), (int)vcl_ceil(max_y + dmax));
      label_buf_.set_size(win_.max_x() - win_.min_x()+1,win_.max_y() - win_.min_y()+1,1);
      label_buf_.fill(unvisited);
    }
}

//: move to dvgl? 
// \todo support for multiple sheets. We currently support any non-simple
// polygon with one sheet.
//\remarks more info in my notes of oct 5 '06
class dbecl_polygon_grid_cover_iterator {
public:

  dbecl_polygon_grid_cover_iterator( vgl_polygon<double> const &poly, dbecl_grid_cover_window &win, double dmax=1.5);

  //: Construct with a polygon, option indicating boundary condition and window (area visible)
//  dbecl_polygon_grid_cover_iterator( vgl_polygon<double> const &poly, dbecl_rasterization_type opt,
//    vgl_box_2d<double> const &window);

//  ~dbecl_polygon_grid_cover_iterator() {};

  unsigned count();
  void reset();
  bool nxt(); //:< advances point

  int x() const {return p_.x();}
  int y() const {return p_.y();}

  void clear_labels();

private:

  vgl_polygon<double> poly_;
  bool empty_poly_;
  vcl_stack<vgl_point_2d<int> > stk_;
  vcl_stack<vgl_point_2d<int> > visited_pixels_;
  vgl_point_2d<int> p_; //:< current point
  dbecl_grid_cover_window &win_;
  double dmax_;
};




class dbecl_epiband;
//: 
// \remarks This iterator becomes invalid once the original dbecl_epiband object is destroyed.
class dbecl_epiband_iterator : public dbecl_polygon_grid_cover_iterator {
public:

  dbecl_epiband_iterator( const dbecl_epiband &band , dbecl_grid_cover_window &win, double dmax=1.5);
};



//=================================================================================

//: move to dvgl? 
// \todo support for multiple sheets. We currently support any non-simple
// polygon with one sheet.
//\remarks more info in my notes of oct 5 '06
class dbecl_slow_polygon_grid_cover_iterator {
public:

  //: Construct with a polygon and option indicating boundary condition.
  dbecl_slow_polygon_grid_cover_iterator( vgl_polygon<double> const &poly, double dmax=1.5);
  
  //: Construct with a polygon, option indicating boundary condition and window (area visible)
//  dbecl_slow_polygon_grid_cover_iterator( vgl_polygon<double> const &poly, dbecl_rasterization_type opt,
//    vgl_box_2d<double> const &window);

//  ~dbecl_slow_polygon_grid_cover_iterator() {};

  unsigned count();
  void reset();
  bool nxt(); //:< advances point

  int x() const {return p_.x();}
  int y() const {return p_.y();}

  void set_window(vgl_box_2d<double> &w) {win_ = w; no_win_ = false;}

private:
  enum pixel_state {
    in_stack,
    unvisited,
    popped,
    removed,
  };

  pixel_state label(vgl_point_2d<int> &p) const
   { 
     vcl_map<vgl_point_2d<int>, pixel_state, ltpt>::const_iterator it
       = label_.find(p);
     if (it == label_.end())
       return unvisited;
     else 
       return it->second;
   }

  struct ltpt {
    bool operator()(const vgl_point_2d<int> &s1, const vgl_point_2d<int> &s2) const
      {
         return vcl_pair<int,int> (s1.x(), s1.y()) < 
                vcl_pair<int,int> (s2.x(),s2.y());
      }
  };

  bool valid_x (int x) const { return x >= win_.min_x() && x <= win_.max_x(); }
  bool valid_y (int y) const { return y >= win_.min_y() && y <= win_.max_y(); }

  vgl_polygon<double> poly_;
  bool empty_poly_;
  double dmax_;
  vgl_box_2d<double> win_;
  bool no_win_;
  vcl_stack<vgl_point_2d<int> > stk_;
  vgl_point_2d<int> p_; //:< current point
  vcl_map<vgl_point_2d<int>, pixel_state, ltpt> label_;
};



class dbecl_epiband;
//: 
// \remarks This iterator becomes invalid once the original dbecl_epiband object is destroyed.
class dbecl_slow_epipolar_band_iterator : public dbecl_slow_polygon_grid_cover_iterator {
public:

  //: Construct with a polygon and option indicating boundary condition.
  dbecl_slow_epipolar_band_iterator( const dbecl_epiband &band , double dmax=1.5);
};

#endif // dbecl_epiband_iterator_h

