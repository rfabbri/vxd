#include "mw_curve_stereo.h"
#include <vnl/vnl_math.h>

#include <vgl/vgl_line_2d.h>
#include <vgl/vgl_box_2d.h>
#include <vgl/vgl_distance.h>
#include <vgl/vgl_intersection.h>

#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_digital_curve_2d.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_box_2d.h>

#include <vpgl/algo/vpgl_ray_intersect.h>
#include <brct/brct_algos.h>
#include <dvpgl/algo/dvpgl_triangulation.h>

#include <dbecl/dbecl_episeg_sptr.h>
#include <dbecl/dbecl_episeg.h>
#include <dbecl/dbecl_epipole.h>
#include <dbecl/dbecl_episeg_from_curve_converter.h>

#include <mw/mw_epi_interceptor.h>


mw_curve_stereo::
mw_curve_stereo()
  :
    isets_(),
    tau_min_samples_per_curve_frag_(0),
    tau_min_length_per_curve_frag_(0.0),
    tau_min_epipolar_overlap_(5),
    prune_by_length_(false),
    cached_curve_id_from_sptr_(false)
{
}

bool mw_curve_stereo::
set_nviews(unsigned nv)
{
  if (nv < 3) {
    vcl_cerr << "Error: need at least " << 3 << " views for this tool" << vcl_endl;
    return false;
  }
  nviews_ = nv;
  cam_.resize(nviews_);
  vsols_.resize(nviews_);
  ep_ini_.resize(nviews_-1);
  ep_end_.resize(nviews_-1);
  ep_.resize(nviews_-1);
  selected_crv_.resize(nviews_);
  selected_crv_id_.resize(nviews_);
  ep_ini_v1_.resize(nviews_-2);
  ep_end_v1_.resize(nviews_-2);
  ep_v1_.resize(nviews_-2);

  fm_.resize(nviews_);

  for (unsigned i=0; i<nviews_; ++i)
    fm_[i].resize(nviews_);

  return true;
}


void mw_curve_stereo::
reconstruct_multiview()
{
  //: Pick triplets
  //    - p0
  //    - p1 = ep0-1 intersect with selected curve in view[v0()]
  //    - p2 = ep0-2 intersect with selected curve in view[v1()]
  //    - If multiple intersections of type p2, disambiguate using ep1-2 intersect with selected
  //    curve in view[2]

  for (unsigned v=0; v < nviews_; ++v)
    if (!selected_crv_[v]) {
      vcl_cerr << "Error: a curve must be selected in all views.\n";
      return;
    }

  unsigned ini_id, end_id;
  get_increasing_endpoints(&ini_id, &end_id); 

  vcl_ofstream 
    fcrv_3d_2v, 
    fcrv_3d_3v, 
    fcrv_3d_linear, 
    fcrv_3d, 
    fcrv_2d;

  vcl_string prefix("dat/reconstr-tracer-multi"); 
  vcl_string prefix2("dat/curve2d-view0-tracer-multi");
  vcl_string ext(".dat");

  vcl_string cmd;
  cmd = vcl_string("rm -f ") + prefix + vcl_string("*dat  ") + prefix2 + vcl_string("*dat");

  if (system(cmd.c_str()) == -1)
    vcl_cout << "Error removing old reconstructions\n";

  //: notice string to distinguish this and the files from 'r' option
  vcl_string fname = prefix + ext;

  fcrv_3d.open(fname.c_str(),vcl_ios::out | vcl_ios::binary);
  vcl_cout << "Writing 3d curve: " << fname << vcl_endl;

  fname = prefix + vcl_string("-2v") + ext;
  fcrv_3d_2v.open(fname.c_str(),vcl_ios::out | vcl_ios::binary);
  vcl_cout << "Writing 3d curve: " << fname << vcl_endl;

  fname = prefix + vcl_string("-3v") + ext;
  fcrv_3d_3v.open(fname.c_str(),vcl_ios::out | vcl_ios::binary);
  vcl_cout << "Writing 3d curve: " << fname << vcl_endl;

  fname = prefix + vcl_string("-linear") + ext;
  fcrv_3d_linear.open(fname.c_str(),vcl_ios::out | vcl_ios::binary);
  vcl_cout << "Writing 3d curve: " << fname << vcl_endl;

  fname = prefix2 + ext;

  // write corresp curve in view 2
  fcrv_2d.open(fname.c_str(),vcl_ios::out | vcl_ios::binary);
  vcl_cout << "Writing 2d curve of 2nd view: " << fname << vcl_endl;

  for (unsigned k=0; k < selected_crv_[v1()]->size(); ++k) {
    const vsol_point_2d_sptr pt = selected_crv_[v1()]->vertex(k);
    double px = pt->x();
    double py = pt->y();
    fcrv_2d.write((char *)( &px ), sizeof(double));
    fcrv_2d.write((char *)( &py ), sizeof(double));
  }
  fcrv_2d.close();


  // Now traverse selected curve in view[v0()]

  for (unsigned di0=0; di0 + ini_id <= end_id; ++di0) {
    mw_vector_3d pt_3D, pt_3D_linear;

    { // reconstruct from all views
      vcl_cout << "reconstruct from all views\n";

      vcl_vector<unsigned> views; // views we want to use
      views.reserve(nviews_-1); 

      for (unsigned v=1; v < nviews_; ++v)
        views.push_back(v);

      get_reconstructions(views, ini_id, di0, &pt_3D, &pt_3D_linear);
    }
    fcrv_3d_linear.write((char *)(pt_3D_linear.data_block()),3*sizeof(double));
    fcrv_3d.write((char *)(pt_3D.data_block()),3*sizeof(double));

    { // for comparison, reconstruct from 2 views: first + last
      vcl_cout << "reconstruct from 2 views\n";
      vcl_vector<unsigned> views; // views we want to use
      views.push_back(nviews_-1);

      get_reconstructions(views, ini_id, di0, &pt_3D, &pt_3D_linear);
    }
    fcrv_3d_2v.write((char *)(pt_3D.data_block()),3*sizeof(double));

    { // for comparison, reconstruct from 3 views: first + last + mid
      vcl_cout << "reconstruct from 3 views\n";
      vcl_vector<unsigned> views; // views we want to use
      views.push_back((nviews_-1)/2);
      views.push_back(nviews_-1);

      get_reconstructions(views, ini_id, di0, &pt_3D, &pt_3D_linear);
    }
    fcrv_3d_3v.write((char *)(pt_3D.data_block()),3*sizeof(double));
  }

  fcrv_3d_linear.close();
  fcrv_3d.close();
  fcrv_3d_2v.close();
  fcrv_3d_3v.close();
}

void mw_curve_stereo::
linearly_reconstruct_pts(
    const vcl_vector<vsol_point_2d_sptr> &pt_img,
    const vcl_vector<unsigned> &other_views,
    vgl_point_3d<double> *pt_3D
    ) const 
{
  vcl_cout << "  Reconstructing linearly.\n";
  assert(pt_img.size() > 1);
  assert(other_views.size() > 0);
  vcl_vector<vnl_double_2> pts;
  vcl_vector<vnl_double_3x4> projs;

  pts.push_back(vnl_double_2(pt_img[0]->x(), pt_img[0]->y()) );
  projs.push_back(cam_[v0()].Pr_.get_matrix());

  for (unsigned v=0; v < other_views.size(); ++v) {
    pts.push_back(vnl_double_2(pt_img[v+1]->x(), pt_img[v+1]->y()));
    projs.push_back(cam_[other_views[v]].Pr_.get_matrix());
  }
  *pt_3D = brct_algos::bundle_reconstruct_3d_point(pts, projs);
}

void mw_curve_stereo::
nonlinearly_optimize_reconstruction(
    const vcl_vector<vsol_point_2d_sptr> &pt_img,
    const vcl_vector<unsigned> &other_views,
    const vgl_point_3d<double> &pt_3D_initial,
    vgl_point_3d<double> *pt_3D
    ) const
{
  vcl_cout << "  Reconstructing optimized.\n";
  assert(pt_img.size() > 1);
  assert(other_views.size() > 0);
  vpgl_ray_intersect isect(other_views.size()+1);

  vcl_vector<vgl_point_2d<double> > pts;
  vcl_vector<vpgl_camera<double> * > projs;

  pts.push_back(pt_img[0]->get_p());
  projs.push_back(new vpgl_perspective_camera <double> (cam_[v0()].Pr_) );

  for (unsigned v=0; v < other_views.size(); ++v) {
    pts.push_back(pt_img[v+1]->get_p());
    projs.push_back(new vpgl_perspective_camera <double> (cam_[other_views[v]].Pr_) );
  }
  isect.intersect(projs, pts, pt_3D_initial, *pt_3D);

  for (unsigned v=0; v < projs.size(); ++v)
    delete projs[v];
}

void mw_curve_stereo::
get_reconstructions(
    const vcl_vector<unsigned> &other_views, 
    unsigned ini_id, 
    unsigned di0, 
    mw_vector_3d *pt_3D, 
    mw_vector_3d *pt_3D_linear
    ) const 
{
  // Corresponding points
  vcl_vector<vsol_point_2d_sptr> pt_img(other_views.size()+1);
  pt_img[0] = selected_crv_[v0()]->vertex(ini_id + di0); 
  for (unsigned v=0; v < other_views.size(); ++v) {
    assert(v+1 < selected_crv_.size());
    assert(v+1 < pt_img.size());
    assert (other_views[v] != v0());
    assert (other_views[v] < nviews_);

    vgl_point_2d<double> p;
    mw_epi_interceptor::curve_line_intersection_simple(
        *selected_crv_[other_views[v]], ep(other_views[v]-1)[di0], &p);
    pt_img[v+1] = new vsol_point_2d(p);
  }

  // Reconstructions

  vgl_point_3d<double> pt_3D_vgl, pt_3D_linear_vgl;

  linearly_reconstruct_pts(pt_img, other_views, &pt_3D_linear_vgl);
  *pt_3D_linear = mw_util::vgl_to_vnl(pt_3D_linear_vgl);

  nonlinearly_optimize_reconstruction(
      pt_img, other_views, pt_3D_linear_vgl, &pt_3D_vgl);

  *pt_3D = mw_util::vgl_to_vnl(pt_3D_vgl);
}

void mw_curve_stereo::
reconstruct_subcurve(
    unsigned ini_id_sub, 
    unsigned end_id_sub, 
    vcl_vector<mw_vector_3d> *curve_3d) const
{
  curve_3d->reserve(end_id_sub - ini_id_sub + 1);
  for (unsigned di0=0; di0 + ini_id_sub <= end_id_sub; ++di0) {
    mw_vector_3d pt_3D;
    static const unsigned second_view = 1;
    reconstruct_curve_point_kanatani(second_view, ini_id_sub, di0, &pt_3D);
    curve_3d->push_back(pt_3D);
  }
}

void mw_curve_stereo::
reconstruct_curve_point_kanatani(
    unsigned v,
    unsigned ini_id,
    unsigned di0, 
    mw_vector_3d *pt_3D) const
{
  vgl_point_2d<double> p_0 = selected_crv_[v0()]->vertex(ini_id + di0)->get_p();
  // Corresponding points
  vgl_point_2d<double> p_v;
  mw_epi_interceptor::curve_line_intersection_simple(*(selected_crv_[v]), ep(v-1)[di0], &p_v);

  // Reconstructions
  vgl_point_3d<double> pt_3D_vgl;

  // Kanatani
  pt_3D_vgl = triangulate_3d_point_optimal_kanatani_fast(
      p_0, p_v, cam_[v0()].Pr_, cam_[v].Pr_);

  *pt_3D = mw_util::vgl_to_vnl(pt_3D_vgl);
}

void mw_curve_stereo::
reconstruct_and_reproject(
    unsigned crv2_id, 
    unsigned view, 
    vcl_vector<vsol_point_2d_sptr> &reproj, 

    vcl_vector<mw_vector_3d> &crv3d, 
    vcl_vector<unsigned> &crv1_pt_id,
    vcl_vector<unsigned> &crv2_pt_id,
    dbdif_rig &rig) const
{
  define_match_for_reconstruction(crv2_id, crv1_pt_id, crv2_pt_id, rig);
  reconstruct_one_candidate(crv2_id, crv3d, crv1_pt_id, crv2_pt_id, rig);
  project_curve(view, crv3d, &reproj); 
}

void mw_curve_stereo::
project_curve(
    unsigned view, 
    const vcl_vector<mw_vector_3d> &crv3d,
    vcl_vector<vsol_point_2d_sptr> *proj_ptr
    ) const
{
  vcl_vector<vsol_point_2d_sptr> &proj = *proj_ptr;

  // Reproject into 3rd view
  proj.resize(crv3d.size());
  for (unsigned i=0; i<crv3d.size(); ++i) {
    // - get image coordinates
    mw_vector_2d p_aux;
    p_aux = cam_[view].project_to_image(crv3d[i]);
    proj[i] = new vsol_point_2d(p_aux[0], p_aux[1]);
  }
}

void mw_curve_stereo::
define_match_for_reconstruction(
    unsigned crv2_id,
    vcl_vector<unsigned> &crv1_pt_id,
    vcl_vector<unsigned> &crv2_pt_id,
    dbdif_rig &/*rig*/
    ) const
{
  unsigned ini_id, 
           end_id,
           j;

  get_increasing_endpoints(&ini_id, &end_id);

  crv1_pt_id.clear();
  crv2_pt_id.clear();

  j = crv_candidates_id_[crv2_id];

  // traverse L_[j] 
  vcl_list<mw_intersection_sets::intersection_nhood_>::const_iterator ptr;
  for (ptr=isets_.L_[j].intercepts.begin(); ptr != isets_.L_[j].intercepts.end(); ++ptr) {
    unsigned k = ptr->ep_number;
    crv1_pt_id.push_back(ini_id + k);
    //    vsol_point_2d_sptr pt_img1 = crv_->vertex(ini_id + k); 

    unsigned lmin=0;

    { // determine point of this iset minimizing epipolar distance (assume accurate calib)
      double cost_min = vcl_numeric_limits<double>::infinity(); 
      double cost;

      assert(ptr->index.size() > 0);
      for (unsigned l=0; l<ptr->index.size(); ++l) {
        const vsol_point_2d_sptr pt2 = crv_candidates_ptrs_[crv2_id]->vertex(ptr->index[l]);

        vgl_homg_point_2d<double> homg_pt(pt2->x(),pt2->y());
        cost = vgl_distance(ep(0)[k],homg_pt);
        if ( cost < cost_min ) {
          cost_min = cost;
          lmin = l;
        }
      }
    }
    crv2_pt_id.push_back(ptr->index[lmin]);
  }
}

void mw_curve_stereo::
reconstruct_one_candidate(
    unsigned crv2_id, 
    vcl_vector<mw_vector_3d> &crv3d, 
    const vcl_vector<unsigned> &crv1_pt_id,
    const vcl_vector<unsigned> &crv2_pt_id,
    dbdif_rig &rig) const
{
  crv3d.resize(crv1_pt_id.size());
  for (unsigned i=0; i<crv1_pt_id.size(); ++i) {
    vsol_point_2d_sptr pt_img1 = selected_crv_[v0()]->vertex(crv1_pt_id[i]); 

    vsol_point_2d_sptr pt_img2 = crv_candidates_ptrs_[crv2_id]->vertex(crv2_pt_id[i]);

    mw_vector_3d pt_3D;

    // ---- Reconstruct ---
    vgl_point_3d<double> pt_3D_vgl;

    pt_3D_vgl = triangulate_3d_point_optimal_kanatani_fast(
        pt_img1->get_p(), pt_img2->get_p(), rig.cam[v0()].Pr_, rig.cam[v1()].Pr_);

    crv3d[i] = mw_util::vgl_to_vnl(pt_3D_vgl);
    // --------------------
  }
}

void mw_curve_stereo::
break_curves_into_episegs_pairwise(
    vcl_vector<vcl_vector< vsol_polyline_2d_sptr > > *broken_vsols,
    vcl_vector<bbld_subsequence_set> *ss_ptr
    ) const
{
#ifndef NDEBUG
  vcl_cout << "mw_curve_stereo::break_curves_into_episegs_pairwise" << vcl_endl;
#endif
  vcl_vector<bbld_subsequence_set> &ss = *ss_ptr;
  broken_vsols->resize(nviews_);
  ss.resize(nviews_);
  vgl_homg_point_2d<double> e, e_prime;
  fm_[v0()][v1()].get_epipoles(e, e_prime);

  break_curves_into_episegs(vsols_[v0()], &(*broken_vsols)[v0()], e, &ss[v0()]);
  break_curves_into_episegs(vsols_[v1()], &(*broken_vsols)[v1()], e_prime, &ss[v1()]);

  for (unsigned v=0; v < nviews_; v++) {
    if (vsols_[v].empty() || v == v0() || v == v1())
      continue;
    fm_[v0()][v].get_epipoles(e, e_prime);
    break_curves_into_episegs(vsols_[v], &(*broken_vsols)[v], e_prime, &ss[v]);
  }
}

void mw_curve_stereo::
break_curves_into_episegs(
  const vcl_vector< vsol_polyline_2d_sptr >  &vsols,
  vcl_vector<vsol_polyline_2d_sptr> *vsols2,
  const vgl_homg_point_2d<double> &e,
  bbld_subsequence_set *ss_ptr
  )
{
  bbld_subsequence_set &ss = *ss_ptr;
  // ----------------------------------------------------------------------
  // Break curve

  dbecl_epipole_sptr epipole = new dbecl_epipole(e.x()/e.w(), e.y()/e.w());
  dbecl_episeg_from_curve_converter factory(epipole);

  // A) For each vsol, do:
  
  vcl_vector<dbecl_episeg_sptr> all_episegs;
  all_episegs.reserve(2*vsols.size());
  ss.reserve(2*vsols.size());
  for (unsigned i=0; i < vsols.size(); ++i) {
    // A1 - convert to digital curve
    vcl_vector<vsol_point_2d_sptr>  samples;
    samples.reserve(vsols[i]->size());
    for (unsigned k=0; k < vsols[i]->size(); ++k)
      samples.push_back(vsols[i]->vertex(k));

    
    // A2 - apply episeg
    vcl_vector<dbecl_episeg_sptr> eps = 
      factory.convert_curve(new vsol_digital_curve_2d(samples));

    for(vcl_vector<dbecl_episeg_sptr>::iterator itr = eps.begin(); 
        itr != eps.end();  ++itr) {
      all_episegs.push_back(*itr);

      bbld_subsequence sub((unsigned) vcl_ceil((*itr)->min_index()),
                         (unsigned)((*itr)->max_index()) + 1);
      sub.set_orig_id(i);
      ss.push_back(sub);
    }
  }

  // B) Extract vsol from episegs

  vsols2->reserve(all_episegs.size());

  for(unsigned k=0; k < all_episegs.size(); ++k) {
    //if ( all_episegs[k]->max_index() 
    //   - all_episegs[k]->min_index()  < threshold_number_of_points)
    //  continue;

    vsols2->push_back(new vsol_polyline_2d);
    // B1 - traverse episegs
    for (unsigned i = ss[k].ini(); i < ss[k].end(); ++i)
      vsols2->back()->add_vertex( all_episegs[k]->curve()->point(i) );
  }
}

const vsol_point_2d_sptr mw_curve_stereo::
advance_endpoint() 
{
  if (pn_id_ > p0_id_ && (pn_id_+1) < selected_crv_[v0()]->size()) {
    pn_id_++;
    vsol_point_2d_sptr pt = selected_crv_[v0()]->vertex(pn_id_);
  } else { 
    if (pn_id_ < p0_id_ && pn_id_ != 0)
      pn_id_--;
    else
      return NULL;
  }
  update_endpoint(pn_id_);
  return selected_crv_[v0()]->vertex(pn_id_);
}

const vsol_point_2d_sptr mw_curve_stereo::
recede_endpoint() 
{
  if (pn_id_ >= p0_id_ && (pn_id_-1) != 0) {
    pn_id_--;
  } else  {
    if (pn_id_ < p0_id_ && (pn_id_+1) < selected_crv_[v0()]->size()) 
      pn_id_++;
    else
      return NULL;
  }
  update_endpoint(pn_id_);
  return selected_crv_[v0()]->vertex(pn_id_);
}

void mw_curve_stereo::
set_subcurve(unsigned ini_id, unsigned end_id)
{
  initialize_subcurve(ini_id);
  update_endpoint(end_id);
}

void mw_curve_stereo::
initialize_subcurve(unsigned pt_id)
{
  p0_id_ = pn_id_ = pt_id;

  vsol_point_2d_sptr pt = selected_crv(v0())->vertex(pt_id);
  vgl_homg_point_2d<double> homg_pt(pt->x(), pt->y());
  for (unsigned v=0; v+1 < nviews(); ++v) {
    ep_ini_[v] = fm_[v0()][v+1].l_epipolar_line(homg_pt);
    ep_end_[v] = ep_ini(v);
    ep_[v].push_back(ep_ini(v));
  }

  // Polyline joining p0 to pn
  subcurve_ = new vsol_polyline_2d();
  subcurve_->add_vertex(pt);
}

void mw_curve_stereo::
update_endpoint(unsigned end_id)
{
  pn_id_ = end_id;
  subcurve_ = mw_util::get_subcurve(selected_crv(v0()), p0_id_, pn_id_);

  const vsol_point_2d &pt = *selected_crv(v0())->vertex(pn_id_);

  // update epipolar line of pn
  vgl_homg_point_2d<double> homg_pt(pt.x(), pt.y());

  for (unsigned v=0; v+1 < nviews(); ++v) {
    ep_end_[v] = fm_[v0()][v+1].l_epipolar_line(homg_pt);
    ep_[v].push_back(ep_end(v));
  }
}

void mw_curve_stereo::
swap_endpoints() 
{
  unsigned id_aux;
  id_aux = p0_id_;
  p0_id_ = pn_id_;
  pn_id_ = id_aux;
  subcurve_ = mw_util::reverse_order(subcurve_);

  for (unsigned i=0; i+1 < nviews(); ++i) {
    vgl_homg_line_2d<double> ep_tmp;
    ep_tmp = ep_ini(i);
    ep_ini_[i] = ep_end(i);
    ep_end_[i] = ep_tmp;
    vcl_reverse(ep_[i].begin(), ep_[i].end());
  }
}

void mw_curve_stereo::
compute_epipolar_beam_candidates()
{
  //: index into s->vsols_[1] of candidate (whole) curves
  vcl_list<unsigned> crv_prelim_candidates; 
  vcl_vector<vsol_polyline_2d_sptr> crv_prelim_candidates_ptrs;

  // clear previous crv_candidates (we're not incrementally updating)

  crv_candidates_.clear();
  crv_candidates_ptrs_.clear();
  crv_candidates_id_.clear();

  vcl_vector<bool> is_candidate(vsols_[v1()].size(), false);

  unsigned ini_id, end_id;
  get_increasing_endpoints(&ini_id, &end_id);

  for (unsigned i=0; i+1 < nviews(); ++i) {
    ep_[i].clear();
    ep_[i].reserve(end_id - ini_id + 1);
  }

  ep_left_.clear();
  ep_left_.reserve(end_id - ini_id + 1);

  // update epipolar lines for the subcurve
  // TODO this might not be necessary. Plus, instead of using selected_crv(0)
  // all the time, we can just use subcurve_.

  for (unsigned v=0; v+1 < nviews(); ++v) {
    for (unsigned i=ini_id; i <= end_id; ++i) {
      const vsol_point_2d &pt = *selected_crv(v0())->vertex(i);
      vgl_homg_point_2d<double> homg_pt(pt.x(), pt.y());

      vgl_homg_line_2d<double>  epi = fm_[v0()][v+1].l_epipolar_line(homg_pt);
      ep_[v].push_back(epi);

      if (v == v0()) {
        vgl_homg_line_2d<double>  epi_left = fm_[v0()][v1()].r_epipolar_line(ep(0).back());
        ep_left_.push_back(epi_left);
      }
    }
    ep_ini_[v] = ep(v).front();
    ep_end_[v] = ep(v).back();
  }
  assert (subcurve_->size() == ep_[0].size());
  assert (subcurve_->size() == end_id-ini_id+1);

  // intersect bounding box of each curve in view[1] with the epipolar beams. If
  // at least one epipolar line intersects the bounding box, mark the curve as a
  // preliminary candidate for further inspection.
  for (unsigned k=0; k < vsols_[v1()].size(); ++k) {
    vsol_box_2d_sptr vsol_bbox = vsols_[v1()][k]->get_bounding_box();

    vgl_box_2d<double> bbox(vsol_bbox->get_min_x(), vsol_bbox->get_max_x(),
                            vsol_bbox->get_min_y(), vsol_bbox->get_max_y());

    vgl_point_2d<double> pt_meet1, pt_meet2;
    // if we increment by min_intercepts, we guarantee that no curves greater
    // than or equal to min_intercepts will be missed.
    for (unsigned i = 0; i < subcurve_->size(); i += tau_min_epipolar_overlap_) {
      if (vgl_intersection<double>(bbox,
            vgl_line_2d<double>(ep(0)[i]), pt_meet1, pt_meet2)) {
        crv_prelim_candidates.push_back(k);
        crv_prelim_candidates_ptrs.push_back(vsols_[v1()][k]);
        break;
      }
    }
  }

  // Any curve fragment that touches at least one epipolar line of the beam is a
  // candidate. By "touches" we mean having a point in the vicinity of an
  // epipolar line. Last time I checked, the threshold was 1.4px
  isets_.compute(crv_prelim_candidates_ptrs, ep(0));
  crv_candidates_.reserve(isets_.ncurves());
  crv_candidates_ptrs_.reserve(isets_.ncurves());
  crv_candidates_id_.reserve(isets_.ncurves());

  vcl_list<unsigned>::const_iterator itr = crv_prelim_candidates.begin(); 
  for (unsigned j=0;  j < isets_.ncurves();  ++j, ++itr) {
    if ( isets_.L_[j].intercepts.size() >= tau_min_epipolar_overlap_) {
      crv_candidates_.push_back( *itr );
      crv_candidates_ptrs_.push_back(crv_prelim_candidates_ptrs[j]);
      crv_candidates_id_.push_back(j);
    }
  }
  // \todo perhaps trim vectors here using the swap trick
}
  
void mw_curve_stereo::
set_cams(const vcl_vector<dbdif_camera> &cams)
{
  assert(cam_.size() == nviews());
  cam_ = cams;

  // Cache fmatrices
  for (unsigned i=0; i < nviews(); ++i) {
    for (unsigned k=0; k < nviews(); ++k) {
      if (k == i)
        continue;
      fm_[i][k] = vpgl_fundamental_matrix <double> (cam_[i].Pr_,cam_[k].Pr_);
    }
  }
}

void mw_curve_stereo::
set_curves(const vcl_vector<vcl_vector< vsol_polyline_2d_sptr > > &curves)
{
  assert (curves.size() == nviews());
  vsols_ = curves;
}

void mw_curve_stereo::
break_into_episegs_and_replace_curve(vcl_vector<bbld_subsequence_set> *pcurves_ss)
{
  vcl_vector<vcl_vector< vsol_polyline_2d_sptr > > curves;
  vcl_vector<bbld_subsequence_set> ss_break;

  vcl_cout << "Orig   #curves in view 0: " << num_curves(0) << vcl_endl;
  vcl_cout << "Orig   #curves in view 1: " << num_curves(1) << vcl_endl;

  break_curves_into_episegs_pairwise(&curves, &ss_break);

  vcl_cout << "Broken #curves in view 0: " << curves[0].size() << vcl_endl;
  vcl_cout << "Broken #curves in view 1: " << curves[1].size() << vcl_endl;

  const unsigned num_curve_sets = curves.size();
  assert(ss_break.size() == num_curve_sets);

  vcl_vector<bbld_subsequence_set> &curves_ss = *pcurves_ss;
  curves_ss.resize(num_curve_sets);

  for (unsigned i=0; i < num_curve_sets; ++i) {
    assert(ss_break[i].num_subsequences() == curves[i].size());
    if (prune_by_length_)
      mw_util::prune_curves_by_length(tau_min_length_per_curve_frag_, 
          &curves[i], &curves_ss[i]);
    else
      mw_util::prune_curves(tau_min_samples_per_curve_frag_, 
          &curves[i], &curves_ss[i]);
    compose_subsequences(ss_break[i], &curves_ss[i]);
  }
  vcl_cout << "Pruned #curves in view 0: " << curves[0].size() << vcl_endl;
  vcl_cout << "Pruned #curves in view 1: " << curves[1].size() << vcl_endl;
  set_curves(curves);
  vcl_cout << "Final  #curves in view 0: " << num_curves(0) << vcl_endl;
  vcl_cout << "Final  #curves in view 1: " << num_curves(1) << vcl_endl;
}

bool mw_curve_stereo::
get_index_of_candidate_curve(const vsol_polyline_2d_sptr & select_crv, unsigned *crv2_id) const
{
  for (unsigned i=0; i<crv_candidates_ptrs_.size(); ++i) {
    if (crv_candidates_ptrs_[i] == select_crv) {  // Pointer comparison
      *crv2_id = i;
      return true;
    }
  } 
  return false;
}

bool mw_curve_stereo::
get_index_of_curve(
    const vsol_polyline_2d_sptr & select_crv, unsigned view, unsigned *crv2_id) const
{
  if (view == v0()) {
    if (cached_curve_id_from_sptr_) {
      vcl_map<vsol_polyline_2d_sptr, unsigned>::iterator itr
       = curve_v0_id_from_sptr_.find(select_crv);
      if (itr == curve_v0_id_from_sptr_.end())
        return false;

      *crv2_id = (*itr).second;
      return true;
    } else {
      for (unsigned i=0; i < vsols_[view].size(); ++i)
        curve_v0_id_from_sptr_[vsols_[view][i]] = i;
      cached_curve_id_from_sptr_ = true;
      return get_index_of_curve(select_crv, view, crv2_id);
    }
  } else {
    for (unsigned i=0; i < vsols_[view].size(); ++i) {
      if (vsols_[view][i] == select_crv) {  // Pointer comparison
        *crv2_id = i;
        return true;
      }
    }
    return false;
  }

  return false;
}

void mw_curve_stereo::
reproject_in_all_views(unsigned crv2_id, vcl_vector< vcl_vector<vsol_point_2d_sptr> > *preproj)
{
  vcl_vector< vcl_vector<vsol_point_2d_sptr> > &reproj = *preproj;
  reproj.resize(nviews());

  dbdif_rig rig(cams(v0()).Pr_, cams(v1()).Pr_);

  for (unsigned v=2; v < nviews_; ++v) {
    vcl_vector<mw_vector_3d> crv3d; 

    //: These indicate an 1-1 alignment between the selected curve in view[v0()] and the
    // selected curve in view[v1()]. Both these vectors have the same size, and
    // crv1_pt_id[i] corresponds to crv2_pt_id[i].
    vcl_vector<unsigned> crv1_pt_id, crv2_pt_id;

    // TODO crvi_pt_id and crv3d should be computed only once outside this loop.
    reconstruct_and_reproject(crv2_id, v /*view*/, reproj[v], crv3d, 
        crv1_pt_id, crv2_pt_id, rig);

    // epilines for reprojection: endpts
    vsol_point_2d_sptr pt = crv_candidates_ptrs_[crv2_id]->vertex(crv2_pt_id.front());
    ep_ini_v1_[v-2] = fm_[v1()][v].l_epipolar_line(
        vgl_homg_point_2d<double>(pt->x(), pt->y()));

    pt = crv_candidates_ptrs_[crv2_id]->vertex(crv2_pt_id.back());
    ep_end_v1_[v-2] = fm_[v1()][v].l_epipolar_line(
        vgl_homg_point_2d<double>(pt->x(), pt->y()));

    // epilines for reprojection: all other points
    ep_v1_[v-2].resize(crv2_pt_id.size());
    for (unsigned i=0; i < crv2_pt_id.size(); ++i) {
      vsol_point_2d_sptr pt = crv_candidates_ptrs_[crv2_id]->vertex(crv2_pt_id[i]);
      ep_v1_[v-2][i] = fm_[v1()][v].l_epipolar_line(vgl_homg_point_2d<double>(pt->x(), pt->y()));
    }
  }
}

void mw_curve_stereo::
get_matching_subcurve(
    unsigned candidate_index,
    unsigned ini_idx,
    unsigned end_idx,
    unsigned *ini_idx_sub,
    unsigned *end_idx_sub
    ) const
{
  // compute a map 
  // no_intersects[i] == true if epipolar i does not intersect the curve candidate.

  assert(selected_crv(v0())->size() >= ep(0).size());
  unsigned const selected_subcurve_size = end_idx-ini_idx+1;
  assert(ep(0).size() == selected_subcurve_size);

  vcl_vector<bool> no_intersections(selected_subcurve_size, true);

  {
    vcl_list<mw_intersection_sets::intersection_nhood_>::const_iterator  itr;

    assert(crv_candidates_id(candidate_index) < isets().L_.size());
    const vcl_list<mw_intersection_sets::intersection_nhood_> &ilist 
      = isets().L_[ crv_candidates_id(candidate_index) ].intercepts;  

    for (itr = ilist.begin(); itr != ilist.end(); ++itr) {
      assert(itr->ep_number < no_intersections.size());
      no_intersections[itr->ep_number] = false;
    }
  }

  assert (ini_idx <= end_idx);

  unsigned origin = ini_idx;

  while (no_intersections[ini_idx-origin]) {
    assert (ini_idx >= origin);
    assert (ini_idx - origin < no_intersections.size());
    ++ini_idx;
  }

  assert (ini_idx <= end_idx);

  while (no_intersections[end_idx-origin]) {
    assert (end_idx >= origin);
    assert (end_idx - origin < no_intersections.size());
    --end_idx;
  }

  assert (ini_idx <= end_idx);

  *ini_idx_sub = ini_idx;
  *end_idx_sub = end_idx;
}

void mw_curve_stereo::
get_candidate_intercepts(vcl_vector<vcl_vector<unsigned> > *cand_pt_id_ptr)
{
  vcl_vector<vcl_vector<unsigned> > &pts_id_cand = *cand_pt_id_ptr;

  if (!num_candidates())
    return;

  vcl_vector<vcl_vector<unsigned> > pts_id;
  isets().all_points(pts_id);

  pts_id_cand.resize(num_candidates());

  for (unsigned j=0; j < num_candidates(); ++j) {
    unsigned icrv = crv_candidates_id(j);
    unsigned const npts = pts_id[icrv].size();

    pts_id_cand[j].resize(npts);

    for (unsigned k=0; k < npts; ++k)
      pts_id_cand[j][k] = pts_id[icrv][k];
  }
}

void mw_curve_stereo::
set_selected_crv(unsigned iview, const vsol_polyline_2d_sptr &scrv)
{ 
  selected_crv_[iview] = scrv; 
  get_index_of_curve(scrv, iview, &selected_crv_id_[iview]);
}
