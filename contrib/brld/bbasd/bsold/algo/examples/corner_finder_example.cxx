//: Corner finder example

#include <dbsol/algo/dbsol_curve_algs.h>
#include <dbsol/algo/dbsol_corner_finder.h>
#include <vul/vul_file_iterator.h>
#include <bdgl/bdgl_curve_algs.h>
#include <dbgl/algo/dbgl_curve_smoothing.h>
#include <vcl_cstring.h>
#include <vcl_cstdlib.h>

// Can says: In the future, it would be better to write an I/O class 
// for dbsol curve class. I am re-using these functions for the time-being.
void loadCON(vcl_string fileName, vcl_vector<vsol_point_2d_sptr> &points)
{
  vcl_ifstream infp(fileName.c_str());
  char magicNum[200];

  infp.getline(magicNum,200);
  if (vcl_strncmp(magicNum,"CONTOUR",7))
  {
    vcl_cerr << "Invalid File " << fileName.c_str() << vcl_endl;
    vcl_cerr << "Should be CONTOUR " << magicNum << vcl_endl;
    vcl_exit(1);
  }

  char openFlag[200];
  infp.getline(openFlag,200);
  if (!vcl_strncmp(openFlag,"OPEN",4))
    vcl_cout << "Open Curve" << vcl_endl;
  else if (!vcl_strncmp(openFlag,"CLOSE",5))
    vcl_cout << "Closed Curve" << vcl_endl;
  else
  {
    vcl_cerr << "Invalid File " << fileName.c_str() << vcl_endl;
    vcl_cerr << "Should be OPEN/CLOSE " << openFlag << vcl_endl;
    vcl_exit(1);
  }

  int i,numOfPoints;
  infp >> numOfPoints;

  vcl_cout << numOfPoints << vcl_endl;

  double x,y;
  for (i=0;i<numOfPoints;i++)
  {
    infp >> x >> y;
    points.push_back(new vsol_point_2d(x, y));
  }
  infp.close();
}

void writeCON(vcl_string fileName, dbsol_interp_curve_2d &c, int numpoints)
{
  vcl_ofstream outfp(fileName.c_str());
  assert(outfp != NULL);
  outfp << "CONTOUR" << vcl_endl;
  outfp << "OPEN" << vcl_endl;
  outfp << numpoints << vcl_endl;
  double ds = c.length()/(numpoints-1);
  for(int i=0; i<numpoints; i++)
  {
//    vsol_point_2d_sptr p = c.point_at(i*ds);
    vsol_point_2d_sptr p = c.point_at_sample(i);
    outfp << p->x() << " " << p->y() << " " << vcl_endl;
  }
  outfp.close();
}

int main()
{
  /************************************************************************************************************/
  /* TESTING RICARDO'S SMOOTHING, this was written to see the smoothing result step-by-step */
  /*
  vcl_string in = "D:/MyDocs/projects/PuzzleSolving/meetings/demo/tile b con/b07-points.con";
  vcl_vector<vsol_point_2d_sptr> points;
  loadCON(in, points);
  vcl_vector<vgl_point_2d<double > > pts;
  for(unsigned i=0; i<points.size();i++)
    pts.push_back(vgl_point_2d<double> (points[i]->x(), points[i]->y()));
  for(int i=0; i<33; i++)
  {
    vcl_string out = in;
    char buffer [33];
    _itoa(i,buffer,10);
    out.append(buffer);
    float psi = 1;
    unsigned int num_times = 5;
    dbgl_csm(pts, psi, num_times);
    vcl_vector<vsol_point_2d_sptr> smoothed;
    for(unsigned i=0; i<pts.size();i++)
      smoothed.push_back(new vsol_point_2d(pts[i].x(), pts[i].y()));
    dbsol_interp_curve_2d curve;
    vnl_vector<double> samples;
    dbsol_curve_algs::interpolate_eno(&curve, smoothed, samples);

    writeCON(out, curve, curve.size()+1);
  }
  /************************************************************************************************************/

  // DESCRIPTION
  // Given a folder, this program reads all cons under this folder and executed the corner finding algorithm,
  // writing the results to the same folder as corner files
  //
  // Corner file format is simple: The first line is an integer stating the number of corners
  // Every following line gives the index of one of these corners
  //
  // If you want to do re-sample the points evenly, there is also code available below
  // Just uncomment it
  // If you want different sampling rate, change i++ in for(int i=0; i<curve_temp.length(); i++)
  // with the sampling rate in terms of arclength ,e.g. use i = i + 2 for less number of points
  //
  // If you want to apply smoothing to the given set of points, there are three different algorithms
  // below: averaging, Gaussian smoothing, discrete curvature-based smoothing (Ricardo's idea).
  // Uncomment one of these smoothing techniques below and change the line 
  // dbsol_curve_algs::interpolate_eno(&curve, points, samples);
  // with
  // dbsol_curve_algs::interpolate_eno(&curve, smoothed, samples);
  // The smoothing parameters of the algorithms, if applicable, are straightforward to find in the code.
  //
  // Contact can@lems.brown.edu for questions...
  vcl_string folder = "S:/tile c con/";
  vcl_string ftype = folder;
  ftype.append("*.con");
  vul_file_iterator f(ftype);
  while(f.filename() != NULL)
  {
    vcl_cout << f.filename() << vcl_endl;
    vcl_string in = folder;
    in.append(f.filename());

    vcl_string out_corners = in;
    out_corners.append(".cor");

    vcl_string out_con = in;
    out_con.append(".con");

    vcl_vector<vsol_point_2d_sptr> points;
    loadCON(in, points);

    /************************************************************************************************************/
    /*RESAMPLE EVENLY*/
    
    vcl_vector<vsol_point_2d_sptr> evenly_sampled_points;
    dbsol_interp_curve_2d curve_temp;
    vnl_vector<double> samples_temp;
    dbsol_curve_algs::interpolate_eno(&curve_temp, points, samples_temp);
    for(int i=0; i<curve_temp.length(); i++)
    {
      vsol_point_2d_sptr p = curve_temp.point_at(i);
      evenly_sampled_points.push_back(new vsol_point_2d(p->x(), p->y()));
    }

    points.clear();
    for(unsigned i=0; i<evenly_sampled_points.size(); i++)
      points.push_back(new vsol_point_2d(evenly_sampled_points[i]->x(), evenly_sampled_points[i]->y()));
    /************************************************************************************************************/

    /************************************************************************************************************/
    /*RICARDO SMOOTHING*/
    
    vcl_vector<vgl_point_2d<double > > pts;
    for(unsigned i=0; i<points.size();i++)
      pts.push_back(vgl_point_2d<double> (points[i]->x(), points[i]->y()));
    float psi = 1;
    unsigned int num_times = 30;
    dbgl_csm(pts, psi, num_times);
    vcl_vector<vsol_point_2d_sptr> smoothed;
    for(unsigned i=0; i<pts.size();i++)
      smoothed.push_back(new vsol_point_2d(pts[i].x(), pts[i].y()));
    /************************************************************************************************************/
    
    /************************************************************************************************************/
    /*CAN STEP SMOOTHING*//*
    // SMOOTHING THE SET OF POINTS USING STEP KERNEL
    // The implementation is specific for closed curves
    vcl_vector<vsol_point_2d_sptr> smoothed;
    // Make the curve closed
    points.push_back(points[0]);
    int p = int(points.size());
    for(int i=0; i < p-1; i++)
    {
      double x = (points[(i-6+p)%p]->x() +
                  points[(i-5+p)%p]->x() +
                  points[(i-4+p)%p]->x() + 
                  points[(i-3+p)%p]->x() + 
                  points[(i-2+p)%p]->x() + 
                  points[(i-1+p)%p]->x() + 
                  points[(i+p)%p]->x() + 
                  points[(i+1+p)%p]->x() + 
                  points[(i+2+p)%p]->x() + 
                  points[(i+3+p)%p]->x() + 
                  points[(i+4+p)%p]->x() + 
                  points[(i+5+p)%p]->x() + 
                  points[(i+6+p)%p]->x()) / 13;
      double y = (points[(i-6+p)%p]->y() +
                  points[(i-5+p)%p]->y() +
                  points[(i-4+p)%p]->y() +
                  points[(i-3+p)%p]->y() +
                  points[(i-2+p)%p]->y() + 
                  points[(i-1+p)%p]->y() + 
                  points[(i+p)%p]->y() + 
                  points[(i+1+p)%p]->y() + 
                  points[(i+2+p)%p]->y() +
                  points[(i+3+p)%p]->y() +
                  points[(i+4+p)%p]->y() +
                  points[(i+5+p)%p]->y() +
                  points[(i+6+p)%p]->y()) / 13;

      smoothed.push_back(new vsol_point_2d(x, y));
    }
    /************************************************************************************************************/

    /************************************************************************************************************/
    /*CAN GAUSSIAN SMOOTHING*/
    // SMOOTHING THE SET OF POINTS USING GAUSSIAN KERNEL
    // The implementation is specific for closed curves
/*    double sigma = 3.0;
    double range = sigma * 3; // 3 standard deviations away
    vcl_vector<vsol_point_2d_sptr> smoothed;
    // Make the curve closed
    points.push_back(points[0]);
    int p = int(points.size());
    for(int i=0; i < p-1; i++)
    {
      vsol_point_2d_sptr cur_point = points[i];
      double total_weight = 1;
      double total_sum_x = cur_point->x();
      double total_sum_y = cur_point->y();
      // go left
      int index = 1;
      while(1)
      {
        vsol_point_2d_sptr neigh_point = points[(i - index + p) % p];
        double dist = cur_point->distance(neigh_point);
        if(dist > range)
          break;
        else
        {
          double weight = vcl_exp(-vcl_pow(dist, 2.0) / (2 * vcl_pow(sigma, 2.0)));
          total_weight += weight;
          total_sum_x += weight * neigh_point->x();
          total_sum_y += weight * neigh_point->y();
        }
        index++;
      }
      // go right
      index = 1;
      while(1)
      {
        vsol_point_2d_sptr neigh_point = points[(i + index + p) % p];
        double dist = cur_point->distance(neigh_point);
        if(dist > range)
          break;
        else
        {
          double weight = vcl_exp(-vcl_pow(dist, 2.0) / (2 * vcl_pow(sigma, 2.0)));
          total_weight += weight;
          total_sum_x += weight * neigh_point->x();
          total_sum_y += weight * neigh_point->y();
        }
        index++;
      }
      double x = total_sum_x / total_weight;
      double y = total_sum_y / total_weight;
      smoothed.push_back(new vsol_point_2d(x, y));
    }
    /************************************************************************************************************/

    dbsol_interp_curve_2d curve;
    vnl_vector<double> samples;
    dbsol_curve_algs::interpolate_eno(&curve, smoothed, samples);

    dbsol_corner_finder cf;
    cf.find_corners(&curve, true);
    vcl_vector<int> *indices;
    indices = cf.get_corner_indices();

    vcl_ofstream outfp(out_corners.c_str());
    assert(outfp != NULL);

    outfp << indices->size() << vcl_endl;

    for(unsigned i=0; i < indices->size(); i++)
      outfp << indices->at(i) << vcl_endl;

    outfp.close();

    writeCON(out_con, curve, curve.size()+1);

    ++f;
  }

  return 0;
}
