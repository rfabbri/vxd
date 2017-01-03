#include "dbcsi_curve_distance.h"
#include <dbgl/dbgl_dist.h>
#include <dbdet/sel/dbdet_edgel.h>
#include <dbdet/edge/dbdet_edgemap.h>

#include <dbspi/dbspi_curve_distance.h>

unsigned dbcsi_curve_distance::
inlier_curvelets_dt(
    const vsol_digital_curve_2d &crv, double d_threshold, 
    const vil_image_view<vxl_uint_32> &dt,
    const vil_image_view<vxl_uint_32> &label,
    const dbdet_curvelet_map &cm,
    curvelet_list *inlier_curvelets
    )
{
  //: compute inlier edgels
  vcl_set<unsigned> inliers;
  dbspi_curve_distance::inliers_dt(crv, d_threshold, dt, label, &inliers); 

  // For each inlier edgel
  //      Check that all edgels are also inliers
  for (
      vcl_set<unsigned>::const_iterator in = inliers.begin();
      in != inliers.end();
      in++) {

    int icell = *in % dt.ni();
    int jcell = *in / dt.ni();

    const vcl_vector<dbdet_edgel*>& edgels_in_cell 
      = cm.EM_->cell(icell, jcell);

    for (unsigned ie = 0; ie < edgels_in_cell.size(); ie++) {

#ifndef NDEBUG
      unsigned x = dbdet_round(edgels_in_cell[ie]->pt.x()); 
      unsigned y = dbdet_round(edgels_in_cell[ie]->pt.y()); 
      assert( dt(x, y) == dt(icell, jcell) );
#endif

      bool all_inliers = true;

      const curvelet_list &curvelets = cm.curvelets(edgels_in_cell[ie]->id);
      for (
        curvelet_list_const_iter cv_it = curvelets.begin();
        cv_it != curvelets.end(); 
        cv_it++) {

        dbdet_curvelet *cvlet = *cv_it;

        for (unsigned i=0; i < cvlet->edgel_chain.size(); ++i) {
          unsigned c = dbdet_round(cvlet->edgel_chain[i]->pt.x());
          unsigned r = dbdet_round(cvlet->edgel_chain[i]->pt.y());
          if (dt(c, r) > d_threshold) {
            all_inliers = false;
            break;
          }
        }

        if (all_inliers) {
#ifndef NDEBUG
          vcl_cout << "Adding a curvelet\n";
#endif
          inlier_curvelets->push_back(cvlet);
        }
      }
    }
  }
  return inlier_curvelets->size();
}

unsigned dbcsi_curve_distance::
inlier_curvelets_dt_oriented(
    const dbcsi_edgel_seq &c, double d_threshold, double dtheta_threshold, 
    const vil_image_view<vxl_uint_32> &dt,
    const vil_image_view<vxl_uint_32> &label,
    const dbdet_curvelet_map &cm,
    unsigned min_num_inlier_edgels,
    curvelet_list *inlier_curvelets
    )
{
  //: compute inlier edgels
  vcl_set<unsigned> inliers;
  dbcsi_curve_distance::inliers_dt_oriented(c, d_threshold, dtheta_threshold,
      dt, label, *cm.EM_, &inliers); 

  // For each inlier edgel
  //      Check that all edgels are also inliers
  for (
      vcl_set<unsigned>::const_iterator in = inliers.begin();
      in != inliers.end();
      in++) {

    int icell = *in % dt.ni();
    int jcell = *in / dt.ni();

    const vcl_vector<dbdet_edgel*>& edgels_in_cell 
      = cm.EM_->cell(icell, jcell);

    for (unsigned ie = 0; ie < edgels_in_cell.size(); ie++) {

#ifndef NDEBUG
      unsigned x = dbdet_round(edgels_in_cell[ie]->pt.x()); 
      unsigned y = dbdet_round(edgels_in_cell[ie]->pt.y()); 
      assert( dt(x, y) == dt(icell, jcell) );
#endif


      const curvelet_list &curvelets = cm.curvelets(edgels_in_cell[ie]->id);
      for (
        curvelet_list_const_iter cv_it = curvelets.begin();
        cv_it != curvelets.end(); 
        cv_it++) {

        bool enough_inliers = true;

        dbdet_curvelet *cvlet = *cv_it;
        unsigned num_inliers = cvlet->edgel_chain.size();
        for (unsigned i=0; i < cvlet->edgel_chain.size(); ++i) {
          unsigned lx = dbdet_round(cvlet->edgel_chain[i]->pt.x()); 
          unsigned ly = dbdet_round(cvlet->edgel_chain[i]->pt.y()); 
          unsigned l = ly*dt.ni() + lx;

          if (inliers.find(l) == inliers.end()) {
            num_inliers--;
            if (num_inliers < min_num_inlier_edgels) {
              enough_inliers = false;
              break;
            }
          }
        }

        if (enough_inliers) {
#ifndef NDEBUG
          vcl_cout << "Adding a curvelet\n";
#endif
          inlier_curvelets->push_back(cvlet);
          break; // max 1 curvelet per edgel counts.
        }
      }
    }
  }
  return inlier_curvelets->size();
}


unsigned dbcsi_curve_distance::
inliers_dt_oriented(
    const dbcsi_edgel_seq &c, double d_threshold, double dtheta_threshold,
    const vil_image_view<vxl_uint_32> &dt,
    const vil_image_view<vxl_uint_32> &label,
    const dbdet_edgemap &em,
    vcl_set<unsigned> *inliers
  )
{
#ifndef NDEBUG
  if (dtheta_threshold > vnl_math::pi/2)
    vcl_cerr << "Warning: a threshold of > vnl_math::pi/2 means no threshold.\n";
#endif
  for (dbcsi_edgel_seq_const_iter ep = c.begin(); ep != c.end(); ++ep) {
    unsigned p_i = static_cast<unsigned>(ep->pt.x()+0.5);
    unsigned p_j = static_cast<unsigned>(ep->pt.y()+0.5);

    if (dt(p_i, p_j) < d_threshold) {
      vxl_uint_32 l = label(p_i, p_j);
      const vcl_vector<dbdet_edgel*> &ev = em.edge_cells.begin()[l];

      for (unsigned i=0; i < ev.size(); ++i) {
        double dtheta = dbgl_undirected_angle_distance(ev[i]->tangent, ep->tangent);
        /*
#ifndef NDEBUG
        vcl_cout 
          << "Curve point: " << ep->pt  
          << "<tgt " << ep->tangent*180./vnl_math::pi << ">"
          << " edgel point: " << ev[i]->pt
          << "<tgt " << ev[i]->tangent*180./vnl_math::pi << ">";

        vcl_cout << " dtheta(deg): " << dtheta*180./vnl_math::pi << vcl_endl;
#endif
*/
        if (dtheta < dtheta_threshold) {
          inliers->insert(l);
          break;
        }
      }
    }
  }
  
  return inliers->size();
}

unsigned dbcsi_curve_distance::
num_inliers_dt_oriented(
    const dbcsi_edgel_seq &c, double d_threshold, double dtheta_threshold,
    const vil_image_view<vxl_uint_32> &dt,
    const vil_image_view<vxl_uint_32> &label,
    const dbdet_edgemap &em)
{
  vcl_set<unsigned> inliers;

  return inliers_dt_oriented(c, d_threshold, dtheta_threshold, dt, label, em, 
      &inliers);
}

unsigned dbcsi_curve_distance::
inliers_dt_oriented_all_samples(
    const dbcsi_edgel_seq &c, double d_threshold, double dtheta_threshold,
    const vil_image_view<vxl_uint_32> &dt,
    const vil_image_view<vxl_uint_32> &label,
    const dbdet_edgemap &em,
    vcl_list<unsigned> *inliers
  )
{
#ifndef NDEBUG
  if (dtheta_threshold > vnl_math::pi/2)
    vcl_cerr << "Warning: a threshold of > vnl_math::pi/2 means no threshold.\n";
#endif
  for (dbcsi_edgel_seq_const_iter ep = c.begin(); ep != c.end(); ++ep) {
    unsigned p_i = static_cast<unsigned>(ep->pt.x()+0.5);
    unsigned p_j = static_cast<unsigned>(ep->pt.y()+0.5);

    if (dt(p_i, p_j) < d_threshold) {
      vxl_uint_32 l = label(p_i, p_j);
      const vcl_vector<dbdet_edgel*> &ev = em.edge_cells.begin()[l];

      for (unsigned i=0; i < ev.size(); ++i) {
        double dtheta = dbgl_undirected_angle_distance(ev[i]->tangent, ep->tangent);
        /*
#ifndef NDEBUG
        vcl_cout 
          << "Curve point: " << ep->pt  
          << "<tgt " << ep->tangent*180./vnl_math::pi << ">"
          << " edgel point: " << ev[i]->pt
          << "<tgt " << ev[i]->tangent*180./vnl_math::pi << ">";

        vcl_cout << " dtheta(deg): " << dtheta*180./vnl_math::pi << vcl_endl;
#endif
*/
        if (dtheta < dtheta_threshold) {
          inliers->push_back(l);
          break;
        }
      }
    }
  }
  
  return inliers->size();
}


unsigned dbcsi_curve_distance::
num_inliers_dt_oriented_all_samples(
    const dbcsi_edgel_seq &c, double d_threshold, double dtheta_threshold,
    const vil_image_view<vxl_uint_32> &dt,
    const vil_image_view<vxl_uint_32> &label,
    const dbdet_edgemap &em)
{
  vcl_list<unsigned> inliers;

  return inliers_dt_oriented_all_samples(c, d_threshold, dtheta_threshold, dt, label, em, 
      &inliers);
}
