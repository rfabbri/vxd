#include "dbecl_epiband_iterator.h"
#include <dbecl/dbecl_epiband.h>
#include <vgl/vgl_distance.h>
#include <vil/vil_print.h>
#include <vcl_cstdlib.h>


// 8-nhood coordinates
static int n8[8][2] = {
  { 0,  1}, // 0
  {-1,  1}, // 1
  {-1,  0}, // 2
  {-1, -1}, // 3
  { 0, -1}, // 4
  { 1, -1}, // 5
  { 1,  0}, // 6
  { 1,  1}  // 7
};

#define DBECL_ROUND(X)        ((int)((X)+0.5))

//: ATTENTION: User must check validity of returned coordinates. Polygon window
// will not be used (currently, we assume output to be cliped within
// dbecl_epiband::compute).
dbecl_epiband_iterator::
dbecl_epiband_iterator( const dbecl_epiband &band ,dbecl_grid_cover_window &win, double dmax)
  :
  dbecl_polygon_grid_cover_iterator(band.polygon(), win,dmax)
{
//  vgl_box_2d<double> win = band.window();
//  set_window(win);
}

void dbecl_grid_cover_window::
print() const
{ 
  vcl_cout << "dbecl_grid_cover_window:\n";
  vcl_cout << "clip window:";
  win_.print(vcl_cout);
  vcl_cout << vcl_endl;
  vcl_cout << "label array:";
  label_buf_.print(vcl_cout);
//  vil_print_all(vcl_cout,label_buf_);
  vcl_cout << vcl_endl;
  vcl_cout << vcl_endl;
}

bool dbecl_grid_cover_window::
is_label_buf_clean() const
{
  // run trhu image and check if all pixels are marked as unvisited

  for (unsigned i=0; i < label_buf_.ni(); ++i)
    for (unsigned j=0; j < label_buf_.nj(); ++j) {
      if (label_buf_(i,j) != unvisited)
        return false;
    }

  return true;
}


//--------------------------------------------------------------------------------
// POLY
//--------------------------------------------------------------------------------


//: Construct with a polygon and option indicating boundary conditions
//
// label_buffer is an array of pixel_states. The user must pre-alloc this array. Its use is for
// efficiency: instead of alloc-ing memory for each instance of grid_cover_iterator, the user just
// allocs the buffer once and it will be used for all iterators acting on the same image.
//
// The user should use vil_fill(unvisited) on label_buffer to get this working properly.
//
// After this algorithm is run, the user might inspect label_buffer for the "painted" pixel
// coordinates as those having label != unvisited.
//
//
dbecl_polygon_grid_cover_iterator::
dbecl_polygon_grid_cover_iterator( vgl_polygon<double> const &poly, dbecl_grid_cover_window &win, double dmax)
  : poly_(poly), win_(win), dmax_(dmax)
{
  assert(dmax_ > 0.7); // sqrt(2)/2; otherwise our algorithm doesn't work
  if (!poly.num_sheets() || !poly.num_vertices()) 
    empty_poly_ = true;
  else
    empty_poly_ = false;

  reset();

  if (poly.num_sheets() > 1)
    vcl_cerr << "WARNING: multiple sheets not yet supported by grid cover iterator \n";
}


bool dbecl_polygon_grid_cover_iterator::
nxt()
{
  static const unsigned n_nhood = 8;

  if (stk_.empty() || empty_poly_) {
    clear_labels();
    return false;
  }

  p_ = stk_.top();
  stk_.pop();
  win_.set_label(p_,dbecl_grid_cover_window::removed);

  for (unsigned i=0; i < n_nhood; ++i) {
    int qx,qy;

    //: Check if q within bounds
    qx = p_.x() + n8[i][0];
    if (!win_.valid_x(qx))
      continue;
    //: unsigned may wrap around if dx < 0, but this should be eliminated by testing qx <= max_x

    qy = p_.y() + n8[i][1];
    if (!win_.valid_y(qy))
      continue;

    vgl_point_2d<int> q(qx,qy);
    dbecl_grid_cover_window::pixel_state label_q = win_.label(q); 
    if (label_q != dbecl_grid_cover_window::removed && label_q != dbecl_grid_cover_window::in_stack) {
      vgl_point_2d<double> q_double(qx,qy);
      if (poly_.contains(q_double) ||
          vgl_distance(poly_, q_double) < dmax_) {
        stk_.push(q);
        win_.set_label(q,dbecl_grid_cover_window::in_stack);
      } else {
        win_.set_label(q,dbecl_grid_cover_window::removed);
      }
      visited_pixels_.push(q);
    }
  }

  return true;
}


void dbecl_polygon_grid_cover_iterator::
clear_labels()
{
  while (!visited_pixels_.empty()) {
    vgl_point_2d<int> p = visited_pixels_.top();
    visited_pixels_.pop();
    win_.set_label(p,dbecl_grid_cover_window::unvisited);
  }
}

void dbecl_polygon_grid_cover_iterator::
reset()
{
  clear_labels();

  stk_ = vcl_stack<vgl_point_2d<int> >();
  p_.set((int)-1,(int)-1);

  // assert poly has no negative coordinates

  if (empty_poly_)
    return;

  // Select initial vertex
  
  unsigned ip = 0;
  vcl_vector<vgl_point_2d<double> > &sheet = poly_[0];

  while (ip < sheet.size() && !(win_.valid_x(DBECL_ROUND(sheet[ip].x())) && win_.valid_y(DBECL_ROUND(sheet[ip].y()))))
    ip++;

  if (ip == sheet.size()) { // empty integer regions
    // now test if any vertex of the box is to polygon
    vcl_cout << "PROBLEM: POLYGON UNEMPTY, BUT NO VERTEX IN BOX!\n";
    vcl_abort();
    return;
  }

  // select initial point
  stk_.push(vgl_point_2d<int> (DBECL_ROUND(sheet[ip].x()), DBECL_ROUND(sheet[ip].y())));
  win_.set_label(stk_.top(),dbecl_grid_cover_window::in_stack);
  visited_pixels_.push(stk_.top());
}

unsigned dbecl_polygon_grid_cover_iterator::
count()
{
  unsigned cnt = 0; reset();
  while (nxt()) {++cnt;}
  return cnt;
}



//==================================================================================

//: ATTENTION: User must check validity of returned coordinates. Polygon window
// will not be used (currently, we assume output to be cliped within
// dbecl_epiband::compute).
dbecl_slow_epipolar_band_iterator::
dbecl_slow_epipolar_band_iterator( const dbecl_epiband &band , double dmax)
  :
  dbecl_slow_polygon_grid_cover_iterator(band.polygon(),dmax)
{
//  vgl_box_2d<double> win = band.window();
//  set_window(win);
}


//--------------------------------------------------------------------------------
// POLY
//--------------------------------------------------------------------------------


//: Construct with a polygon and option indicating boundary conditions
dbecl_slow_polygon_grid_cover_iterator::
dbecl_slow_polygon_grid_cover_iterator( vgl_polygon<double> const &poly, double dmax)
  : poly_(poly), dmax_(dmax), no_win_(true)
{
  if (!poly.num_sheets() || !poly.num_vertices()) 
    empty_poly_ = true;
  else
    empty_poly_ = false;

  reset();

  if (poly.num_sheets() > 1)
    vcl_cerr << "WARNING: multiple sheets not yet supported by grid cover iterator \n";
}



bool dbecl_slow_polygon_grid_cover_iterator::
nxt()
{
  static const unsigned n_nhood = 8;

  if (stk_.empty() || empty_poly_)
    return false;

  p_ = stk_.top();
  stk_.pop();
  label_[p_] = removed;

  for (unsigned i=0; i < n_nhood; ++i) {
    int qx,qy;

    //: Check if q within bounds
    if (no_win_) {
      //: TODO: fix this -- unsigned may wrap around if dx < 0
      qx = p_.x() + n8[i][0];
      qy = p_.y() + n8[i][1];
    } else {
      qx = p_.x() + n8[i][0];
      if (!valid_x(qx))
        continue;

      qy = p_.y() + n8[i][1];
      if (!valid_y(qy))
        continue;
    }

    vgl_point_2d<int> q(qx,qy);
    pixel_state label_q = label(q); 
    if (label_q != removed && label_q != in_stack) {
      vgl_point_2d<double> q_double(qx,qy);
      if (poly_.contains(q_double) ||
          vgl_distance(poly_, q_double) < dmax_) {
        stk_.push(q);
        label_[q] = in_stack;
      } else {
        label_[q] = removed;
      }
    }
  }

  return true;
}

void dbecl_slow_polygon_grid_cover_iterator::
reset()
{
  label_.clear();
  stk_ = vcl_stack<vgl_point_2d<int> >();
  p_.set((int)-1,(int)-1);

  // assert poly has no negative coordinates

  if (empty_poly_)
    return;

  vgl_point_2d<double> p0 = poly_[0][0];

  stk_.push(vgl_point_2d<int> (DBECL_ROUND(p0.x()), DBECL_ROUND(p0.y())));
  label_[stk_.top()] = in_stack;
}

unsigned dbecl_slow_polygon_grid_cover_iterator::
count()
{
  unsigned cnt = 0; reset();
  while (nxt()) {++cnt;}
  return cnt;
}
