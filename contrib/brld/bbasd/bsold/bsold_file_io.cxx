// This is bsold/bsold_file_io.cxx
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma implementation
#endif

//:
// \file


#include "bsold_file_io.h"

#include <vcl_iostream.h>
#include <vcl_cstring.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_line_2d.h>
#include <vsol/vsol_line_2d_sptr.h>

#include <vnl/vnl_math.h>
#include <vul/vul_psfile.h>
#include <vul/vul_file.h>
#include <vil/vil_image_view.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_image_resource_sptr.h>
#include <vil/vil_image_view_base.h>

#include <bsold/bsold_config.h>

#ifdef HAS_BOOST
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#endif


// -----------------------------------------------------------------------------
//: Load a .CON file and save results to a vector of points and whether the 
// contour is closed. Return false if loading fails
bool bsold_load_con_file(char const* filename, 
                         vcl_vector<vsol_point_2d_sptr >& points, 
                         bool & is_closed)
{
  // open file for reading
  vcl_ifstream infp(filename, vcl_ios_in);
  if (!infp) 
  {
    vcl_cerr << " Error opening file  " << filename << vcl_endl;
    return false;
  }

  // check header
  char line_buffer[2000]; //200
  infp.getline(line_buffer,2000);
  if (vcl_strncmp(line_buffer,"CONTOUR",7)) 
  {
    vcl_cerr << "Invalid File " << filename << vcl_endl
             << "Should be CONTOUR " << line_buffer << vcl_endl;
    return false;
  }

  // check whether contour is open or closed
  char open_flag[2000];
  infp.getline(open_flag,2000);
  if (!vcl_strncmp(open_flag,"OPEN",4))
    is_closed = false;
  else if (!vcl_strncmp(open_flag,"CLOSE",5))
    is_closed = true;
  else
  {
    vcl_cerr << "Invalid File " << filename << vcl_endl
             << "Should be OPEN/CLOSE " << open_flag << vcl_endl;
    return false;
  }


  // read coordinates of the points
  int num_pts;
  infp >> num_pts;

  // vector to store the points
  points.clear();
  points.reserve(num_pts);
  for (int i=0;i<num_pts;i++) 
  {
    float x,y;
    infp >> x >> y;
    vsol_point_2d_sptr new_pt = new vsol_point_2d (x,y);
    points.push_back(new_pt);
  }

  //close file
  infp.close();

  return true;
}


 
// ----------------------------------------------------------------------------
//: Load a .CON file and return a smart pointer to vsol_spatial_object_2d
// This can be either a polyline or a polygon. Use cast function to get the
// exact type
vsol_spatial_object_2d_sptr bsold_load_con_file(char const* filename)
{  
  vcl_vector<vsol_point_2d_sptr > points;
  bool is_closed;
  if (!bsold_load_con_file(filename, points, is_closed))
    return 0;

  
  // create a polyline or a polygon depending on whether the contour
  // file is labelled OPEN or CLOSE
  if (is_closed) 
  {
    return new vsol_polygon_2d (points);
  }
  else 
  {
    return new vsol_polyline_2d (points);
  }
}


// ----------------------------------------------------------------------------
//: Save a polyline to a .CON file. Return false if saving fails
bool bsold_save_con_file(char const* filename, vsol_polyline_2d_sptr polyline)
{
  vcl_vector<vsol_point_2d_sptr > pts;
  pts.reserve(polyline->size());
  for (unsigned int i=0; i<polyline->size(); ++i)
  {
    pts.push_back(polyline->vertex(i));
  }
  return bsold_save_con_file(filename, pts, false);
}


// ----------------------------------------------------------------------------
//: Save a polygon to a .CON file. Return false if saving fails
bool bsold_save_con_file(char const* filename, vsol_polygon_2d_sptr polygon)
{
  vcl_vector<vsol_point_2d_sptr > pts;
  pts.reserve(polygon->size());
  for (unsigned int i=0; i<polygon->size(); ++i)
  {
    pts.push_back(polygon->vertex(i));
  }
  return bsold_save_con_file(filename, pts, true);
}


// ----------------------------------------------------------------------------
//: Save a set of ordered points to a .CON file. Return false if saving fails
bool bsold_save_con_file(char const* filename, 
                         const vcl_vector<vsol_point_2d_sptr >& pts,
                         bool is_closed)
{
  // 1) create the file
  //vcl_ifstream infp(filename, vcl_ios::in);
  vcl_ofstream outfp(filename, vcl_ios_out);
  if (!outfp)
  {
    vcl_cout << " Error writing file  " << filename << vcl_endl;
    return false;
  }

  // 2) start writing out the contour to the file
  
  // header
  outfp << "CONTOUR" << vcl_endl;

  // The contour can either be a polyline producing an open contour 
  // or a polygon producing a close contour          
  if (is_closed)
  {
    outfp << "CLOSE" << vcl_endl;
  }
  else
  {
    outfp << "OPEN" << vcl_endl;
  }

  // Number of points
  outfp << pts.size() << vcl_endl;

  // Write coordinates of the points
  for (unsigned int i=0; i<pts.size(); ++i)
  {
    vsol_point_2d_sptr pt = pts[i];
    outfp <<pt->x() << " " << pt->y() << vcl_endl;
  }

  // 3) close the file
  outfp.close();

  return true;
}


#ifdef HAS_BOOST
static bool bsold_load_cem_gzip(vcl_vector< vsol_spatial_object_2d_sptr >& contours, vcl_string filename);
static bool bsold_save_cem_gzip(vcl_vector< vsol_spatial_object_2d_sptr >& vsol_list, vcl_string filename);
#endif

static bool bsold_load_cem_ascii(vcl_vector< vsol_spatial_object_2d_sptr >& contours, vcl_string filename);
static bool bsold_save_cem_ascii(vcl_vector< vsol_spatial_object_2d_sptr >& vsol_list, vcl_string filename);

bool bsold_load_cem(vcl_vector< vsol_spatial_object_2d_sptr >& contours, vcl_string filename)
{
  vcl_string ext = vul_file::extension(filename);

  if (ext == ".gz") {
#ifdef HAS_BOOST
    return bsold_load_cem_gzip(contours, filename);
#else
    vcl_cerr << "Error: .gz compressed file was provided, but boost wasn't found\n";
#endif
  } else
    return bsold_load_cem_ascii(contours, filename);
}

bool bsold_save_cem(vcl_vector< vsol_spatial_object_2d_sptr >& vsol_list, vcl_string filename)
{
  vcl_string ext = vul_file::extension(filename);

  if (ext == ".gz") {
#ifdef HAS_BOOST
    return bsold_save_cem_gzip(vsol_list, filename);
#else
    vcl_cerr << "Error: .gz compressed filename was provided, but boost wasn't found\n";
#endif
  } else
    return bsold_save_cem_ascii(vsol_list, filename);
}


// -----------------------------------------------------------------------------
// Load .CEM file
bool bsold_load_cem_ascii(vcl_vector< vsol_spatial_object_2d_sptr >& contours, vcl_string filename)
{
  float x, y;
  char lineBuffer[1024];
  int numContours, numTotalEdges, numEdges;
  int ix, iy;
  double idir, iconf, dir, conf;

  //1)If file open fails, return.
  vcl_ifstream infp(filename.c_str(), vcl_ios::in);

  if (!infp){
    vcl_cout << " Error opening file  " << filename.c_str() << vcl_endl;
    return false;
  }

  //2)Read in each line
  while (infp.getline(lineBuffer,1024)) {

    //ignore comment lines and empty lines
    if (strlen(lineBuffer)<2 || lineBuffer[0]=='#')
      continue;

    //read the line with the contour count info
    if (!strncmp(lineBuffer, "CONTOUR_COUNT=", sizeof("CONTOUR_COUNT=")-1)){
      sscanf(lineBuffer,"CONTOUR_COUNT=%d",&(numContours));
      //vcl_cout << numContours << vcl_endl;
      continue;
    }

    //read the line with the edge count info
    if (!strncmp(lineBuffer, "TOTAL_EDGE_COUNT=", sizeof("TOTAL_EDGE_COUNT=")-1)){
      sscanf(lineBuffer,"TOTAL_EDGE_COUNT=%d",&(numTotalEdges));
      //vcl_cout << numTotalEdges << vcl_endl;
      continue;
    }

    //read the beginning of a contour block
    if (!strncmp(lineBuffer, "[BEGIN CONTOUR]", sizeof("[BEGIN CONTOUR]")-1)){

      //discarding other information for now...should really be outputting edgels
      vcl_vector< vsol_point_2d_sptr > points;

      infp.getline(lineBuffer,1024);
      sscanf(lineBuffer,"EDGE_COUNT=%d",&(numEdges));
      //vcl_cout << numEdges << vcl_endl;

      for (int j=0; j< numEdges; j++){
        //the rest should have data that goes into the current contour
        infp.getline(lineBuffer,1024);
        sscanf(lineBuffer," [%d, %d]\t%f\t%f\t[%f, %f]\t%f\t%f",&(ix), &(iy),
              &(idir), &(iconf), &(x), &(y), &(dir), &(conf));

        //VJ's current CEM is in degrees rather than radians so need to convert
        dir += 90;
        dir *= vnl_math::pi/180;

        vsol_point_2d_sptr newPt = new vsol_point_2d (x,y);
        points.push_back(newPt);
      }

      infp.getline(lineBuffer,1024);

    while (strncmp(lineBuffer, "[END CONTOUR]", sizeof(" [END CONTOUR]")-1)) {

      infp.getline(lineBuffer,1024);

    }
      //if (strncmp(lineBuffer, "[END CONTOUR]", sizeof(" [END CONTOUR]")-1))
        //assert(false);

      vsol_polyline_2d_sptr newContour = new vsol_polyline_2d (points);
      contours.push_back(newContour->cast_to_spatial_object());
    }
  }
  infp.close();

  vcl_cout << "Loaded: " << filename.c_str() << ".\n";
  return true;
}

#ifdef HAS_BOOST
// -----------------------------------------------------------------------------
// Load .CEM file compressed with zlib, gzip style.
// TODO: use templating and/or istream inheritance to avoid duplicating almost
// identical code to bsold_load_cem_ascii
bool bsold_load_cem_gzip(vcl_vector< vsol_spatial_object_2d_sptr >& contours, vcl_string filename)
{
  float x, y;
  char lineBuffer[1024];
  int numContours, numTotalEdges, numEdges;
  int ix, iy;
  double idir, iconf, dir, conf;

  //1)If file open fails, return.
  vcl_ifstream infp_orig(filename.c_str(), vcl_ios::in | vcl_ios::binary);

  if (!infp_orig){
    vcl_cout << " Error opening file  " << filename.c_str() << vcl_endl;
    return false;
  }

  boost::iostreams::filtering_istream infp;
  infp.push(boost::iostreams::gzip_decompressor());
  infp.push(infp_orig);

  //2)Read in each line
  while (infp.getline(lineBuffer,1024)) {

    //ignore comment lines and empty lines
    if (strlen(lineBuffer)<2 || lineBuffer[0]=='#')
      continue;

    //read the line with the contour count info
    if (!strncmp(lineBuffer, "CONTOUR_COUNT=", sizeof("CONTOUR_COUNT=")-1)){
      sscanf(lineBuffer,"CONTOUR_COUNT=%d",&(numContours));
      //vcl_cout << numContours << vcl_endl;
      continue;
    }

    //read the line with the edge count info
    if (!strncmp(lineBuffer, "TOTAL_EDGE_COUNT=", sizeof("TOTAL_EDGE_COUNT=")-1)){
      sscanf(lineBuffer,"TOTAL_EDGE_COUNT=%d",&(numTotalEdges));
      //vcl_cout << numTotalEdges << vcl_endl;
      continue;
    }

    //read the beginning of a contour block
    if (!strncmp(lineBuffer, "[BEGIN CONTOUR]", sizeof("[BEGIN CONTOUR]")-1)){

      //discarding other information for now...should really be outputting edgels
      vcl_vector< vsol_point_2d_sptr > points;

      infp.getline(lineBuffer,1024);
      sscanf(lineBuffer,"EDGE_COUNT=%d",&(numEdges));
      //vcl_cout << numEdges << vcl_endl;

      for (int j=0; j< numEdges; j++){
        //the rest should have data that goes into the current contour
        infp.getline(lineBuffer,1024);
        sscanf(lineBuffer," [%d, %d]\t%f\t%f\t[%f, %f]\t%f\t%f",&(ix), &(iy),
              &(idir), &(iconf), &(x), &(y), &(dir), &(conf));

        //VJ's current CEM is in degrees rather than radians so need to convert
        dir += 90;
        dir *= vnl_math::pi/180;

        vsol_point_2d_sptr newPt = new vsol_point_2d (x,y);
        points.push_back(newPt);
      }

      infp.getline(lineBuffer,1024);

    while (strncmp(lineBuffer, "[END CONTOUR]", sizeof(" [END CONTOUR]")-1)) {

      infp.getline(lineBuffer,1024);

    }
      //if (strncmp(lineBuffer, "[END CONTOUR]", sizeof(" [END CONTOUR]")-1))
        //assert(false);

      vsol_polyline_2d_sptr newContour = new vsol_polyline_2d (points);
      contours.push_back(newContour->cast_to_spatial_object());
    }
  }

  vcl_cout << "Loaded: " << filename.c_str() << ".\n";
  return true;
}
#endif //! HAS_BOOST

// -----------------------------------------------------------------------------
//: Save .CEM file
bool bsold_save_cem_ascii(vcl_vector< vsol_spatial_object_2d_sptr >& vsol_list, vcl_string filename)
{
  //1)If file open fails, return.
  vcl_ofstream outfp(filename.c_str(), vcl_ios::out);

  if (!outfp){
    vcl_cout << " Error opening file  " << filename.c_str() << vcl_endl;
    return false;
  }

  // output header information
   outfp <<"# CONTOUR_EDGE_MAP : Logical-Linear + Shock_Grouping"<<vcl_endl;
   outfp <<"# .cem files"<<vcl_endl;
   outfp <<"#"<<vcl_endl;
   outfp <<"# Format :"<<vcl_endl;
   outfp <<"# Each contour block will consist of the following"<<vcl_endl;
   outfp <<"# [BEGIN CONTOUR]"<<vcl_endl;
   outfp <<"# EDGE_COUNT=num_of_edges"<<vcl_endl;
   outfp <<"# [Pixel_Pos]  Pixel_Dir Pixel_Conf  [Sub_Pixel_Pos] Sub_Pixel_Dir Sub_Pixel_Conf "<<vcl_endl;
   outfp <<"# ..."<<vcl_endl;
   outfp <<"# ..."<<vcl_endl;
   outfp <<"# [END CONTOUR]"<<vcl_endl;

  outfp<<vcl_endl;

  // Note: this count is currently missing
   outfp <<"CONTOUR_COUNT="<< vcl_endl;
   outfp <<"TOTAL_EDGE_COUNT="<<vcl_endl;

  // parse through all the vsol classes and save curve objects only
   
  
  for (unsigned int b = 0 ; b < vsol_list.size() ; b++ )
  {
    if( vsol_list[b]->cast_to_curve())
    {
      if( vsol_list[b]->cast_to_curve()->cast_to_polyline() )
      {
        outfp <<"[BEGIN CONTOUR]"<<vcl_endl;
        outfp <<"EDGE_COUNT="<< vsol_list[b]->cast_to_curve()->cast_to_polyline()->size() <<vcl_endl;
        
        for (unsigned int i=0; i<vsol_list[b]->cast_to_curve()->cast_to_polyline()->size();i++)
        {
          vsol_point_2d_sptr pt = vsol_list[b]->cast_to_curve()->cast_to_polyline()->vertex(i);
          //output as subpixel contours
          // [%d, %d]\t%lf\t%lf\t[%lf, %lf]\t%lf\t%lf
          outfp <<" [0, 0]  0.0  0.0  [" << pt->x() << ", " << pt->y() << "]  0.0  0.0"  << vcl_endl;
        }
        outfp <<"[END CONTOUR]"<<vcl_endl<<vcl_endl;
      }
    } else if (vsol_list[b]->cast_to_region()) {
      if (vsol_list[b]->cast_to_region()->cast_to_polygon() )
      {
        outfp <<"[BEGIN CONTOUR]"<<vcl_endl;
        outfp <<"EDGE_COUNT="<< vsol_list[b]->cast_to_region()->cast_to_polygon()->size()+1 <<vcl_endl;
        
        for (unsigned int i=0; i<vsol_list[b]->cast_to_region()->cast_to_polygon()->size();i++)
        {
          vsol_point_2d_sptr pt = vsol_list[b]->cast_to_region()->cast_to_polygon()->vertex(i);
          //output as subpixel contours
          // [%d, %d]\t%lf\t%lf\t[%lf, %lf]\t%lf\t%lf
          outfp <<" [0, 0]  0.0  0.0  [" << pt->x() << ", " << pt->y() << "]  0.0  0.0"  << vcl_endl;
        }
        vsol_point_2d_sptr pt = vsol_list[b]->cast_to_region()->cast_to_polygon()->vertex(0);  // print the first point once more to include last edge
        outfp <<" [0, 0]  0.0  0.0  [" << pt->x() << ", " << pt->y() << "]  0.0  0.0"  << vcl_endl;

        outfp <<"[END CONTOUR]"<<vcl_endl<<vcl_endl;
      }
    }
  }
  
  return true;
}

#ifdef HAS_BOOST
// -----------------------------------------------------------------------------
//: Save .CEM file
bool bsold_save_cem_gzip(vcl_vector< vsol_spatial_object_2d_sptr >& vsol_list, vcl_string filename)
{
  //1)If file open fails, return.
  vcl_ofstream outfp_orig(filename.c_str(), vcl_ios::out | vcl_ios::binary);

  if (!outfp_orig){
    vcl_cout << " Error opening file  " << filename.c_str() << vcl_endl;
    return false;
  }

  boost::iostreams::filtering_ostream outfp;
  outfp.push(boost::iostreams::gzip_compressor());
  outfp.push(outfp_orig);

  // output header information
   outfp <<"# CONTOUR_EDGE_MAP : Logical-Linear + Shock_Grouping"<<vcl_endl;
   outfp <<"# .cem files"<<vcl_endl;
   outfp <<"#"<<vcl_endl;
   outfp <<"# Format :"<<vcl_endl;
   outfp <<"# Each contour block will consist of the following"<<vcl_endl;
   outfp <<"# [BEGIN CONTOUR]"<<vcl_endl;
   outfp <<"# EDGE_COUNT=num_of_edges"<<vcl_endl;
   outfp <<"# [Pixel_Pos]  Pixel_Dir Pixel_Conf  [Sub_Pixel_Pos] Sub_Pixel_Dir Sub_Pixel_Conf "<<vcl_endl;
   outfp <<"# ..."<<vcl_endl;
   outfp <<"# ..."<<vcl_endl;
   outfp <<"# [END CONTOUR]"<<vcl_endl;

  outfp<<vcl_endl;

  // Note: this count is currently missing
   outfp <<"CONTOUR_COUNT="<< vcl_endl;
   outfp <<"TOTAL_EDGE_COUNT="<<vcl_endl;

  // parse through all the vsol classes and save curve objects only
   
  
  for (unsigned int b = 0 ; b < vsol_list.size() ; b++ )
  {
    if( vsol_list[b]->cast_to_curve())
    {
      if( vsol_list[b]->cast_to_curve()->cast_to_polyline() )
      {
        outfp <<"[BEGIN CONTOUR]"<<vcl_endl;
        outfp <<"EDGE_COUNT="<< vsol_list[b]->cast_to_curve()->cast_to_polyline()->size() <<vcl_endl;
        
        for (unsigned int i=0; i<vsol_list[b]->cast_to_curve()->cast_to_polyline()->size();i++)
        {
          vsol_point_2d_sptr pt = vsol_list[b]->cast_to_curve()->cast_to_polyline()->vertex(i);
          //output as subpixel contours
          // [%d, %d]\t%lf\t%lf\t[%lf, %lf]\t%lf\t%lf
          outfp <<" [0, 0]  0.0  0.0  [" << pt->x() << ", " << pt->y() << "]  0.0  0.0"  << vcl_endl;
        }
        outfp <<"[END CONTOUR]"<<vcl_endl<<vcl_endl;
      }
    } else if (vsol_list[b]->cast_to_region()) {
      if (vsol_list[b]->cast_to_region()->cast_to_polygon() )
      {
        outfp <<"[BEGIN CONTOUR]"<<vcl_endl;
        outfp <<"EDGE_COUNT="<< vsol_list[b]->cast_to_region()->cast_to_polygon()->size()+1 <<vcl_endl;
        
        for (unsigned int i=0; i<vsol_list[b]->cast_to_region()->cast_to_polygon()->size();i++)
        {
          vsol_point_2d_sptr pt = vsol_list[b]->cast_to_region()->cast_to_polygon()->vertex(i);
          //output as subpixel contours
          // [%d, %d]\t%lf\t%lf\t[%lf, %lf]\t%lf\t%lf
          outfp <<" [0, 0]  0.0  0.0  [" << pt->x() << ", " << pt->y() << "]  0.0  0.0"  << vcl_endl;
        }
        vsol_point_2d_sptr pt = vsol_list[b]->cast_to_region()->cast_to_polygon()->vertex(0);  // print the first point once more to include last edge
        outfp <<" [0, 0]  0.0  0.0  [" << pt->x() << ", " << pt->y() << "]  0.0  0.0"  << vcl_endl;

        outfp <<"[END CONTOUR]"<<vcl_endl<<vcl_endl;
      }
    }
  }
  return true;
}
#endif //!HAS_BOOST
