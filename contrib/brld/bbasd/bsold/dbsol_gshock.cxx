#include "dbsol_gshock.h"
#include <vcl_cmath.h>
#include <dbgl/algo/dbgl_arc_algo.h>

#if !VCL_STATIC_CONST_INIT_FLOAT_NO_DEFN
const double dbsol_gshock::default_shock_point_dist_threshold
   VCL_STATIC_CONST_INIT_FLOAT_DEFN(1.5);
#endif

unsigned dbsol_gshock::
make(const dbgl_param_curve *prv, const dbgl_param_curve *cur,
     const dbgl_param_curve *nxt, const double shock_point_dist_threshold)
{
   unsigned n_intercepts;
   double mid_x, mid_y, d1, d2;
   vgl_point_2d<double> i1, i2;

   const dbgl_arc *aprv, *acur, *anxt;
   aprv = dynamic_cast<const dbgl_arc *> (prv);
   acur = dynamic_cast<const dbgl_arc *> (cur);
   anxt = dynamic_cast<const dbgl_arc *> (nxt);

   if (!aprv || !acur || !anxt) {
     vcl_cerr << "dbsol_shock::make - only arcs are currently supported" << vcl_endl;
     return 0;
   }



   mid_x = (cur->point_at(0).x() + cur->point_at(1).x()) / 2;
   mid_y = (cur->point_at(0).y() + cur->point_at(1).y()) / 2;

   n_intercepts = dbgl_arc_algo::
      compute_intersections( *aprv, *anxt, &i1, &i2);

   if (n_intercepts == 1) {
        d1 = vnl_math::hypot(i1.x() - mid_x, i1.y() - mid_y);

        if (d1 < shock_point_dist_threshold) {
            loc_.push_back(i1);
            fix_outofbound_shock(aprv,acur,anxt);
        } else
           place_shock_at_endpoints(acur);


   } else if (n_intercepts == 2) {
        // keep only the shock closest to the midpoint
        d1 = vnl_math::hypot(i1.x() - mid_x, i1.y() - mid_y);
        d2 = vnl_math::hypot(i2.x() - mid_x, i2.y() - mid_y);

        i1 = (d1 < d2) ? i1 : i2;
        d1 = (d1 < d2) ? d1 : d2;

        if (d1 < shock_point_dist_threshold) {
            loc_.push_back(i1);
            fix_outofbound_shock(aprv, acur, anxt);
        } else
           place_shock_at_endpoints(acur);

   } else if (!n_intercepts)
      place_shock_at_endpoints(acur);
   else
      vcl_cerr << "weird: invalid number of intersections!!\n";

   return loc_.size();
}

//: TODO: what does this do exactly?
bool dbsol_gshock::
fix_outofbound_shock(const dbgl_arc *prv, const dbgl_arc *cur, const dbgl_arc *nxt)
{
   vgl_point_2d<double> pcenter, ncenter;

   double theta1, theta2, theta3, theta;
   double pdist, ndist;

   if (n_intercepts() != 1) return false;

   pcenter = prv->center();
   ncenter = nxt->center();

   vgl_point_2d<double> loc = this->loc();

   pdist = vnl_math::hypot(loc.x() - pcenter.x(), loc.y() - pcenter.y());
   ndist = vnl_math::hypot(loc.x() - ncenter.x(), loc.y() - ncenter.y());

   if (vcl_fabs(pdist - (1.0/prv->curvature()) ) < 0.2) {
        // back
        theta = vcl_atan2(loc.y() - pcenter.y(), loc.x() - pcenter.x());
        theta = dbsol_geno_measures::rad2deg(theta);

        theta1 = vcl_atan2(prv->point_at(0).y() - pcenter.y(), 
                           prv->point_at(0).x() - pcenter.x());
        theta1 = dbsol_geno_measures::rad2deg(theta1);

        theta2 = vcl_atan2(prv->point_at(1).y() - pcenter.y(), 
                           prv->point_at(1).x() - pcenter.x());
        theta2 = dbsol_geno_measures::rad2deg(theta2);

        theta3 = patchspan(theta2 - theta1);

        if(patchspan(theta2 - theta) * patchspan(theta - theta1) > 0.0) {
           place_shock_at_endpoints(cur);
           return true;
        }

        if((theta3 * patchspan(theta1 - theta)) > 0.0) {
           place_shock_at_endpoints(cur);
           return true;
        }
    }

    if (vcl_fabs(ndist - (1/nxt->curvature())) < 0.2) {
         // forward
        theta = vcl_atan2(loc.y() - ncenter.y(), loc.x() - ncenter.x());
        theta = dbsol_geno_measures::rad2deg(theta);

        theta1 = vcl_atan2(nxt->point_at(0).y() - ncenter.y(), nxt->point_at(0).x() - ncenter.x());
        theta1 = dbsol_geno_measures::rad2deg(theta1);

        theta2 = vcl_atan2(nxt->point_at(1).y() - ncenter.y(), nxt->point_at(1).x() - ncenter.x());
        theta2 = dbsol_geno_measures::rad2deg(theta2);

        theta3 = patchspan(theta2 - theta1);

        if(patchspan(theta2 - theta) * patchspan(theta - theta1) > 0.0) {
           place_shock_at_endpoints(cur);
           return true;
        }

        if((theta3 * patchspan(theta - theta2)) > 0.0) {
           place_shock_at_endpoints(cur);
           return true;
        }
    }
    return false;
}
