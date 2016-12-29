#include <vcl_iostream.h>
#include <vcl_cassert.h>
#include <vcl_cmath.h>
#include <vgl/algo/vgl_homg_operators_2d.h>
#include <vnl/algo/vnl_svd.h>
#include <rrel/rrel_ran_sam_search.h>
#include <rrel/rrel_muset_obj.h>

#include <vpgl/vpgl_camera_compute.h>


//------------------------------------------
bool
dvpgl_camera_compute_ransac::
compute( const vcl_vector< vgl_homg_point_2d<double> >& image_pts,
         const vcl_vector< vgl_homg_point_3d<double> >& world_pts,
         vpgl_proj_camera<double>& camera )
{

  // Check that there are at least 6 points.
  if ( image_pts.size() < 6 || world_pts.size() < 6 ){
    vcl_cerr << "vpgl_camera_compute_ransac: Need at least 6 point pairs.\n"
             << "Number in each set: " << pr.size() << ", " << pl.size() << vcl_endl;
    return false;
  }

  // Check that the correspondence lists are the same size.
  if ( image_pts.size() != world_pts.size() ){
    vcl_cerr << "vpgl_camera_compute_ransac: Need correspondence lists of same size.\n";
    return false;
  }

  // ------- The following is based on code in rrel/examples -------
  
  rrel_proj_problem* estimator = new rrel_proj_problem( pr, pl );
  estimator->verbose = false;

  // The maximum fraction of the data that is expected to be gross outliers.
  double max_outlier_frac = 0.5;

  // The desired probability of finding the correct fit.
  double desired_prob_good = 0.99;

  // The number of different populations in the data set. For most
  // problems, the data is from one source (surface, etc.), so this
  // will be 1.
  int max_pops = 1;

  int trace_level = 0;


  // Using MUSET (Minimum Unbiased Scale Estimator with trimmed statistics)
  rrel_muset_obj* ransac = new rrel_muset_obj((int)vcl_floor(image_points.size()*.75));

  rrel_ran_sam_search* ransam = new rrel_ran_sam_search;
  ransam->set_trace_level(trace_level);
  ransam->set_sampling_params( max_outlier_frac, desired_prob_good, max_pops);
  bool ransac_succeeded = ransam->estimate( estimator, ransac );

  if ( ransac_succeeded )
  estimator->params_to_projective( ransam->params(), camera );

  // Get a list of the outliers.
  vcl_vector<double> residuals;
  estimator->compute_residuals( ransam->params(), residuals );

  outliers = vcl_vector<bool>();
  for ( unsigned i = 0; i < pr.size(); i++ ){
    if ( residuals[i] > 1 )
      outliers.push_back( true );
    else
      outliers.push_back( false );
  }

  delete ransac;
  delete ransam;
  delete estimator;

  return ransac_succeeded;
}


//------------------------------------------
rrel_proj_problem::rrel_proj_problem(
  const vcl_vector< vgl_homg_point_2d<double> >& pr,
  const vcl_vector< vgl_homg_point_2d<double> >& pl ) :
  image_pts_(&image_pts),
  world_pts_(&world_pts),
  rrel_estimation_problem(6,6)
{
  assert( image_pts.size() == world_pts.size() );

  verbose = false;
}


//------------------------------------------
bool
rrel_proj_problem::fit_from_minimal_set(
  const vcl_vector<int>& point_indices,
  vnl_vector<double>& params ) const
{
  if ( verbose ) vcl_cerr << "rrel_proj_problem::fit_from_minimal_set\n";
  assert( point_indices.size() == 6 );

  vcl_vector< vgl_homg_point_2d<double> > image_6pts(6), world_6pts(6);

  for ( int i = 0; i < 6; i++ ){
    int index = point_indices[i];
    image_6pts[i] = image_pts_[index];
    world_6pts[i] = world_pts_[index];
  }

  vpgl_proj_matrix<double> cam;
  vpgl_camera_compute cam6c(true);
  if ( !cam6c.compute( image_pts, world_pts, cam ) )
    return false;

  proj_to_params( cam , params );
  if ( verbose ) vcl_cerr << "params: " << params << '\n';
  return true;
}


//------------------------------------------
void
rrel_proj_problem::compute_residuals(
  const vnl_vector<double>& params,
  vcl_vector<double>& residuals ) const
{
  if ( verbose ) vcl_cerr << "rrel_proj_problem::compute_residuals\n";

  vpgl_proj_matrix<double> cam;
  params_to_proj(params, cam);

  if ( residuals.size() != image_pts_.size() )
    residuals.resize( image_pts_.size() );

  // The residual for each correspondence is the sum of the squared distances from
  // the measured image points to the projected 3D points
  for ( unsigned i = 0; i < image_pts_.size(); i++ )
  {
    vgl_homg_point_2d<double> p_proj(cam->project(vgl_homt_point_3d<double>(world_pts_)));
    residuals[i] = vgl_distance(p_proj,image_pts_[i]);
  }
}


//-------------------------------------------
void
rrel_proj_problem::proj_to_params(
  const vpgl_proj_matrix<double>& camera,
  vnl_vector<double>& p ) const
{
  p.set_size(12);
  vnl_matrix_fixed<double,3,4> camera_vnl = camera.get_matrix();
  unsigned i=0;
  for ( int r = 0; r < 3; r++ )
    for ( int c = 0; c < 4; c++ ) {
      p(i++) = camera_vnl( r, c );
    }
}


//-------------------------------------------
void
rrel_proj_problem::params_to_proj(
  const vnl_vector<double>& p,
  vpgl_proj_matrix<double>& cam ) const
{
  vnl_matrix_fixed<double,3,3> cam_vnl;
  unsigned i=0;
  for ( int r = 0; r < 3; r++ )
    for ( int c = 0; c < 4; c++ )
      cam_vnl( r, c ) = p( i++ );

  cam.set_matrix( cam_vnl );
}


//--------------------------------------------
bool
rrel_proj_problem::weighted_least_squares_fit(
  vnl_vector<double>& /*params*/,
  vnl_matrix<double>& /*norm_covar*/,
  const vcl_vector<double>* /*weights*/ ) const
{
  vcl_cerr << "rrel_proj_problem::weighted_least_squares_fit was called, but is not implemented.\n";
  return false;
}


