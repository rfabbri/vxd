#include "dbecl_epiband_builder.h"
#include <vcl_stack.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_box_2d.h>
#include <vgl/vgl_distance.h>

//: Builds epipolar search bands by iteratively intersecting their transfers at each view.
// The user is responsible for freeing up the memory having pointers in \p epband_.
//
// \remarks
//  There is a problem with epipolar band intersection: after many intersection operations, one might
//  observe a few vertices that are really close to each other. This might confuse the intersection
//  algorithm, generating polygons with e.g. two sheets. My approach is to detect such cases, and
//  the action is to use the epipolar polygon epband[i][i] prior to the intersection. However, even after
//  weird intersection results like having two sheets, one of the sheets usually makes sense in the
//  cases that I have investigated. That sheet could be considered for further iterations, but to be
//  on the safe side we just stop the iteration and print an error message.
//  Here is an example of such intersection degeneracy. Any better ideas on how to deal with it?
//
// epband_[1][1] before the split:
// 
// 875.7843732055469       279.36942420043698 
// 345.75418706294795       170.33817609456614 
// 345.75418706294801       170.33817609456614 
// 345.75418706291936       170.33817609456025 
// 345.75418706294795       170.33817609456614 
// 159.38142733520283       132.07590214585963 
// 134.7195332407743       134.6346676475093 
// 850.42266878439614       283.19915789833294 
// 
// 
// epband[2][1] before the split
// 
// 1280.4342159955472       362.22771710714164 
// 1.1087100246308523       99.127323122305427 
// 1.1087100246307955       106.84982044246971 
// 1280.4342159955472       372.2537380802238 
// 
// 
// Their intersections give a split:
// (gdb) dumpvec epband_[1][1]->poly_.sheets_[0]
// 0:$72 = {x_ = 345.75418706293362, y_ = 170.33817609456321}
// 1:$73 = {x_ = 345.75418706294795, y_ = 170.33817609456614}
// 2:$74 = {x_ = 345.75418706294801, y_ = 170.33817609456614}
// 
// 
// (gdb) dumpvec epband_[1][1]->poly_.sheets_[1]
//  875.7843732055469       279.36942420043698 
//  345.75418706294795       170.33817609456614 
//  345.7541870629193       170.33817609456025 
//   159.3814273352028       132.07590214585963 
//   134.7195332407743       134.6346676475093 
//   850.42266878439614       283.19915789833294 
//: \param[in] v : view index being altered
//
//#define DEBUG 1
void dbecl_epiband_builder::
build_epibands_iteratively(
      bool reinitialize, unsigned v,
      const vcl_vector<bool> &is_specified,
      const vcl_vector<vsol_point_2d_sptr> &points,
      vcl_vector< vcl_vector<dbecl_epiband *> > &epband_,
      const vcl_vector <vsol_box_2d_sptr> &bbox_,
      const vcl_vector< vcl_vector<vpgl_fundamental_matrix<double> > > &fm_,
      double err_pos_
        )
{

  vcl_stack<unsigned> stk;
#ifdef DEBUG
  unsigned n_iter =0;
#endif

  vcl_vector<bool> is_in_stack;
  unsigned nviews_ = epband_.size();

  is_in_stack.resize(nviews_,false);
  

  if (reinitialize) {
    // Before anything, clean up and rebuild intersections
    for (unsigned i=0; i < nviews_; ++i) {
      if (epband_[i][i]) {
        delete epband_[i][i];
        epband_[i][i] = 0;
      }

      for (unsigned k=0; k < nviews_; ++k) {
        if (i == k)
          continue;

        if (!is_specified[k]) {
          delete epband_[k][i];
          epband_[k][i] = 0;
        } else {
          if (k != v) {
            if (!epband_[i][i]) {
              epband_[i][i] = new dbecl_epiband(*(epband_[k][i]));
            } else {
              dbecl_epiband prv(*(epband_[i][i]));
              epband_[i][i]->intersect(prv,*(epband_[k][i]));
            }
          }
        }
      }

      if (epband_[i][i]) {
        if (!is_specified[i] && !is_in_stack[i]) {
          stk.push(i);
          is_in_stack[i] = true;
        }
      }
    }
  }


  stk.push(v);
  is_in_stack[v] = true;

  while (!stk.empty()) {
    v = stk.top();
    stk.pop();
    is_in_stack[v] = false;
#ifdef DEBUG
    vcl_cout << "\nIteration #" << ++n_iter << ", view: " << v << vcl_endl;
#endif

    for (unsigned i=0; i < nviews_; ++i) {
      if (i == v)
        continue;

      if (epband_[v][i]) {
        delete epband_[v][i];
        epband_[v][i] = 0;
      }

#ifdef DEBUG
      vcl_cout << "Creating band at view " << i << vcl_endl;
#endif

      // Compute epipolars 
      // Add the polygons


      double err_theta;

      if (is_specified[v]) {
        epband_[v][i] = new dbecl_epiband(vgl_box_2d<double>(bbox_[i]->get_min_x(),bbox_[i]->get_max_x(), bbox_[i]->get_min_y(), bbox_[i]->get_max_y()));
        err_theta = epband_[v][i]->compute(points[v]->get_p(), fm_[v][i], err_pos_);
      } else {
        assert(epband_[v][v]);
        epband_[v][i] = new 
        dbecl_epiband(vgl_box_2d<double>(bbox_[i]->get_min_x(),bbox_[i]->get_max_x(), bbox_[i]->get_min_y(), bbox_[i]->get_max_y()));
        // we store in epband_[v][v] the intersection of epband_[i][v] for all i
        err_theta = epband_[v][i]->compute(*(epband_[v][v]), fm_[v][i], err_pos_);
      }


      bool new_polygon = true;

#ifdef DEBUG
      vcl_cout << "Err pos: " << err_pos_ << "  Epipolar band width view #" <<  i+1 <<  " (deg): " << (2.0*err_theta/vnl_math::pi)*180.0 << vcl_endl; 
#endif
      if (epband_[i][i]) {
        dbecl_epiband prv(*(epband_[i][i]));
        epband_[i][i]->intersect(prv,*(epband_[v][i]));
        double area1 = epband_[i][i]->area();
        double area2 = prv.area();
        assert(area1 >= 0 && area2 >= 0);
        new_polygon  = vcl_fabs(area1 - area2) > 0.1*err_pos_;
#ifdef DEBUG
        if (!new_polygon)
          vcl_cout << "Area not smaller than what we had\n";
#endif
      } else {
        epband_[i][i] = new dbecl_epiband(*(epband_[v][i]));
      }

      if ( !is_specified[i] && new_polygon && !is_in_stack[i] ) {
        stk.push(i);
        is_in_stack[i] = true;
      }

    }
  }
}
