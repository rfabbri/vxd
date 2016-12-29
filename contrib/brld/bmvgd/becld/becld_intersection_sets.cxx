#include "mw_intersection_sets.h"


mw_intersection_sets::
mw_intersection_sets()
{
}

//: Main computation method
//
// \param[in] curves in a view
// \param[in] epipolar_pencil associated with curve segment in another view
//                - for each curvelet point i in view 1, epipolar_pencil[i] = ep_i
//
void mw_intersection_sets::
compute(
    const vcl_vector<vsol_polyline_2d_sptr> &c, //:< list of curves
    const vcl_vector<vgl_homg_line_2d<double> > &ep //:< list of epipolars
    )
{
  /*
   - For each epipolar line j
      - for each curve i
        - intersect ep[j] and crv[i]
          - create an intersection_nhood
          - If this is a new curve, L_.push_back
          - Insert intersection nhood into L_.current_curve_position
   */

  L_.clear();
  L_.resize(c.size());

  unsigned j,i,k;
  bool stat;
  vcl_vector<bool> indices; 

  for (j=0; j<ep.size(); ++j) {
    for (i=0; i<c.size(); ++i) {
      // - intersect ep[j] and crv[i] 

      stat=mw_line_polyline_intersection_1(&(ep[j]),c[i],indices);
      if (!stat) 
        continue;
      // - identify connected components; each one goes to an intersection set.

      for (k=0; k < c[i]->size(); ++k) {
        if (indices[k])
          L_[i].intercepts.push_back(intersection_nhood_());

        while ( k < c[i]->size() && indices[k] ) {
          L_[i].intercepts.back().ep_number = j;
          L_[i].intercepts.back().index.push_back(k);
          ++k;
        }
      }
    }
  }
}

void mw_intersection_sets::
print() const
{
  vcl_cout << "==== mw_intersection_sets ====" << vcl_endl;

  unsigned intercepted_curve_cnt=0, i;

  for (i=0; i<L_.size(); ++i) {
    if ( ! (L_[i].intercepts.empty()) )
      intercepted_curve_cnt++;
  }

  vcl_cout << "Number of intercepted curves: " << intercepted_curve_cnt << vcl_endl;

  for (i=0; i<L_.size(); ++i) {
    if ( ! (L_[i].intercepts.empty()) ) {
      vcl_cout << "\n\nCURVE NUMBER " << i << vcl_endl;
      print(L_[i]);
    }
  }

  vcl_cout << "==============================" << vcl_endl;
}

void mw_intersection_sets::
print(const curve_intersections_list_ & c) const
{
  vcl_cout << "-- Intersections List --\n";

  vcl_list<intersection_nhood_>::const_iterator  itr;
  unsigned cnt=1;

  for (itr = c.intercepts.begin(); itr != c.intercepts.end(); ++itr,++cnt) {
    vcl_cout << "Intercept number: " << cnt << vcl_endl;
    vcl_cout << "Epipolar number : " << itr->ep_number << vcl_endl;
    vcl_cout << "Number of points: " << itr->index.size() << vcl_endl << vcl_endl;
  }
}

//: Lst[j] is a list of intersections (nhoods) of curve j and ep_i
void mw_intersection_sets::
nhoods_intercepting_epipolar(
    unsigned ep_i, 
    vcl_vector< vcl_list<vcl_vector<unsigned> > > &Lst
    ) const
{
  Lst.resize(L_.size());

  unsigned i;

  for (i=0; i<L_.size(); ++i) {
    // traverse L_[i] and copy to Lst_[i] only those nhoods that are of epipolar ep_i
    vcl_list<intersection_nhood_>::const_iterator ptr;


    for (ptr  = L_[i].intercepts.begin(); 
         ptr != L_[i].intercepts.end() && ptr->ep_number < ep_i; 
         ++ptr)
      ;

    if (ptr != L_[i].intercepts.end())
      while (ptr != L_[i].intercepts.end() && ptr->ep_number == ep_i) {
        Lst[i].push_back(ptr->index);
        ++ptr;
      }
  } 
}

void mw_intersection_sets::
points_intercepting_epipolar(unsigned ep_i, vcl_vector<vcl_list<unsigned> > &pts_idx) const
{
  vcl_vector< vcl_list<vcl_vector<unsigned> > > Lst;

  nhoods_intercepting_epipolar(ep_i, Lst);

  pts_idx.resize(ncurves());

  for (unsigned i=0; i<Lst.size(); ++i) {
    vcl_list<vcl_vector<unsigned> >::const_iterator ptr;

    for (ptr=Lst[i].begin(); ptr != Lst[i].end(); ++ptr) {
      for (unsigned k=0; k < ptr->size(); ++k)
        pts_idx[i].push_back((*ptr)[k]);
    }
  }
}

//void mw_intersection_sets::
//maxmin_index_iset(unsigned imin, unsigned imax, unsigned icrv)
//{
//  // traverse i-th curve's list, and get min and max indices
//
//
//
//
//}

void mw_intersection_sets::
all_points(vcl_vector<vcl_vector<unsigned> > &pts_idx) const
{
  pts_idx.resize(ncurves());

  for (unsigned i=0; i < ncurves(); ++i) {
    pts_idx[i].resize(npoints(i));
    unsigned long k=0; //:< index into pts_idx[i], e.g. pts_idx[i][k]

    vcl_list<intersection_nhood_>::const_iterator  itr;
    for (itr = L_[i].intercepts.begin(); itr != L_[i].intercepts.end(); ++itr) {
      for (unsigned jj=0; jj < itr->index.size(); ++jj) {
        pts_idx[i][k++] = itr->index[jj];
      }
    }
  }
}

//: return net number of intercection pts of curve i
unsigned long mw_intersection_sets::
npoints(unsigned i) const
{
  // traverse L_[i].intercepts
  // count number of points in there

  unsigned long cnt = 0;
  vcl_list<intersection_nhood_>::const_iterator  itr;
  for (itr = L_[i].intercepts.begin(); itr != L_[i].intercepts.end(); ++itr) {
    cnt += itr->index.size();
  }

  return cnt;
}

//: number of curves having non-zero # of intersections
unsigned  mw_intersection_sets::
n_intersecting_curves() const
{
  unsigned intercepted_curve_cnt=0, i;

  for (i=0; i<L_.size(); ++i) {
    if ( ! (L_[i].intercepts.empty()) )
      intercepted_curve_cnt++;
  }
  return intercepted_curve_cnt;
}

//unsigned long mw_intersection_sets::
//count_points(vcl_vector<unsigned> &pts_idx) const
//{
//}
