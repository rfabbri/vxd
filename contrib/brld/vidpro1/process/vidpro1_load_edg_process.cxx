//This is vidpro1/process/vidpro1_load_edg_process.cxx

#include <vcl_iostream.h>
#include <vcl_cassert.h>
#include <vcl_fstream.h>
#include <vcl_cmath.h>
#include <vcl_cstring.h>
#include <vcl_cstdio.h>

#include <vidpro1/process/vidpro1_load_edg_process.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_line_2d.h>
#include <vsol/vsol_line_2d_sptr.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_vector_2d.h>



vidpro1_load_edg_process::vidpro1_load_edg_process() : bpro1_process()
{
  if( !parameters()->add( "Input file <filename...>" , "-edginput" , bpro1_filepath("","*.edg") ) ||
      !parameters()->add( "SubPixel ?" ,               "-bP_SP" ,    false ) ||
      !parameters()->add( "Load as Lines" ,            "-blines" ,   true )  ||
      !parameters()->add( "Scale" ,                    "-scale" ,   1.0 ))
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Clone the process
bpro1_process*
vidpro1_load_edg_process::clone() const
{
  return new vidpro1_load_edg_process(*this);
}


bool vidpro1_load_edg_process::execute()
{
  double x, y;
  char lineBuffer[1024];
  int numGeometry;
  int ix, iy;
  bool bSubPixel=false, blines=false;
  double idir, iconf, dir, conf;
  bpro1_filepath input;
  double scale;
  
  parameters()->get_value( "-edginput" , input );
  parameters()->get_value( "-bP_SP" , bSubPixel );
  parameters()->get_value( "-blines" , blines );
  parameters()->get_value( "-scale" , scale );
  
  vcl_string input_file = input.path;

  //1)If file open fails, return.
  vcl_ifstream infp(input_file.c_str(), vcl_ios::in);

  if (!infp){
    vcl_cout << " Error opening file  " << input_file.c_str() << vcl_endl;
    return false;
  }

  // vector to store the egdels
  vcl_vector< vsol_spatial_object_2d_sptr > edgels;

  //2)Read in each line
  while (infp.getline(lineBuffer,1024)) {
    //ignore comment lines and empty lines
    if (vcl_strlen(lineBuffer)<2 || lineBuffer[0]=='#')
      continue;

    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, " EDGE_COUNT=", sizeof(" EDGE_COUNT=")-1)){
      vcl_sscanf(lineBuffer," EDGE_COUNT=%d",&(numGeometry));
      continue;
    }

    //the rest should have data
    //there are two variations of this file in existence
    if (!vcl_strncmp(lineBuffer, "EDGE : ", sizeof("EDGE : ")-1))
      sscanf(lineBuffer,"EDGE :  [%d, %d]    %lf %lf   [%lf, %lf]   %lf %lf",&(ix), &(iy),
        &(idir), &(iconf), &(x), &(y), &(dir), &(conf));
    else
      sscanf(lineBuffer," [%d, %d]   %lf %lf  [%lf, %lf]  %lf %lf",&(ix), &(iy),
        &(idir), &(iconf), &(x), &(y), &(dir), &(conf));

    //scale the edges
    x*=scale;
    y*=scale;

    if (bSubPixel){
      if (blines){
        vsol_line_2d_sptr newLine = new vsol_line_2d(vgl_vector_2d<double>(vcl_cos(dir)/2.0, vcl_sin(dir)/2.0), vgl_point_2d<double>(x,y));
        edgels.push_back(newLine->cast_to_spatial_object());
      }
      else {
        vsol_point_2d_sptr newPt = new vsol_point_2d(x,y);
        edgels.push_back(newPt->cast_to_spatial_object());
      }
    }
    else {
      if (blines){
        vsol_line_2d_sptr newLine = new vsol_line_2d(vgl_vector_2d<double>(vcl_cos(idir), vcl_sin(idir)), vgl_point_2d<double>((double)ix, (double)iy));
        edgels.push_back(newLine->cast_to_spatial_object());
      }
      else {
        vsol_point_2d_sptr newPt = new vsol_point_2d((double)ix, (double)iy);
        edgels.push_back(newPt->cast_to_spatial_object());
      }
    }
    
  }
  infp.close();

  // create the output storage class
  vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
  output_vsol->add_objects(edgels, input_file);
  output_data_[0].push_back(output_vsol);

  return true;
}
