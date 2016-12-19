// This is vidpro1/process/vidpro1_save_con_process.h

#include <vcl_iostream.h>
#include <vcl_fstream.h>
#include <vcl_cstdio.h>

#include <vidpro1/process/vidpro1_save_con_process.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_polygon_2d_sptr.h>
#include <vsol/vsol_point_2d.h>
#include <vnl/vnl_math.h>

vidpro1_save_con_process::vidpro1_save_con_process() : bpro1_process(), first_frame_(true)
{
  if( !parameters()->add( "Output file <filename...>" , 
                          "-conoutput" , 
                          bpro1_filepath("","*.con")) ||
      !parameters()->add( "Output poly file?", "-b_output_poly", (bool) false) ||
      !parameters()->add( "Video File ID:", "-video_file_id", (int) 1007) ||
      !parameters()->add( "# of frames:", "-num_frames", (int) 1) ||
      !parameters()->add( "Output Poly file <filename...>", "-poly_filename", bpro1_filepath("","*.txt")) )
  {
    vcl_cerr << "ERROR: Adding parameters in vidpro1_save_con_process::vidpro1_save_con_process()" << vcl_endl;
  }
}

//: Clone the process
bpro1_process*
vidpro1_save_con_process::clone() const
{
  return new vidpro1_save_con_process(*this);
}

vcl_vector< vcl_string > vidpro1_save_con_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}

vcl_vector< vcl_string > vidpro1_save_con_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.clear();
  return to_return;
}

bool vidpro1_save_con_process::execute()
{
  bpro1_filepath con_output, poly_output;
  bool b_output_poly=false;
  int video_id=0, num_frames=0;
  parameters()->get_value( "-conoutput" , con_output);
  parameters()->get_value( "-b_output_poly" , b_output_poly);
  parameters()->get_value( "-video_file_id" , video_id);
  parameters()->get_value( "-num_frames" , num_frames);
  parameters()->get_value( "-poly_filename" , poly_output);

  vcl_string con_file_root = con_output.path;
  vcl_string poly_file = poly_output.path;
  
  return saveCON(con_file_root, b_output_poly, video_id, num_frames, poly_file );
}

bool 
vidpro1_save_con_process::saveCON (vcl_string con_file_rootname, 
                                  bool output_poly_file, int video_id, 
                                  int num_frames, vcl_string poly_filename)
{
  // get input storage class
  vidpro1_vsol2D_storage_sptr input_vsol;
  input_vsol.vertical_cast(input_data_[0][0]);

  // parse through all the vsol classes and save curve objects only
  vcl_vector< vsol_spatial_object_2d_sptr > vsol_list = input_vsol->all_data();

  vcl_ofstream polyf;
  if (output_poly_file){
    //create a polyfile
    if (first_frame_)
    {
      polyf.open(poly_filename.c_str(), vcl_ios_out);
      polyf << "VIDEOFILEID: " << video_id << vcl_endl;
      polyf << "NFRAMES: " << num_frames << vcl_endl;
      polyf << "NOBJECTS: " << vsol_list.size() << vcl_endl;
      polyf << "NPOLYS: " << vsol_list.size() << vcl_endl;
      first_frame_ = false;
    }
    else {
      polyf.open(poly_filename.c_str(), vcl_ios_app);
      polyf << "NOBJECTS: " << vsol_list.size() << vcl_endl;
      polyf << "NPOLYS: " << vsol_list.size() << vcl_endl;
    }

    if (!polyf){
      vcl_cout << " Error writing file  " << poly_filename << vcl_endl;
      return false;
    }
  }

  //unused bool first = true;
  for (unsigned int b = 0 ; b < vsol_list.size() ; b++ )
  {
    vcl_ofstream outfp;

    if (!output_poly_file)//if we want to save .cons
    {
      //create a unique name for this contour based on the root filename
      char filename[200];
      vcl_sprintf(filename, "%s_%d.con", con_file_rootname.c_str(), b);

      //create the file
      outfp.open(filename, vcl_ios_out);

      if (!outfp){
        vcl_cout << " Error writing file  " << filename << vcl_endl;
        return false;
      }

      //2) start writing out the contour to the file
      outfp << "CONTOUR" << vcl_endl;
    }

    // The contour can either be a polyline producing an open contour 
    // or a polygon producing a close contour
    if( vsol_list[b]->cast_to_curve())
    {
      if( vsol_list[b]->cast_to_curve()->cast_to_polyline() )
      {
        if (!output_poly_file){
          outfp << "OPEN" << vcl_endl;
          outfp << vsol_list[b]->cast_to_curve()->cast_to_polyline()->size() <<vcl_endl;
        
          for (unsigned int i=0; i<vsol_list[b]->cast_to_curve()->cast_to_polyline()->size();i++)
          {
            vsol_point_2d_sptr pt = vsol_list[b]->cast_to_curve()->cast_to_polyline()->vertex(i);
            outfp <<pt->x() << " " << pt->y() << vcl_endl;
          }
        }
      }
    }

    if( vsol_list[b]->cast_to_region())
    {
      if( vsol_list[b]->cast_to_region()->cast_to_polygon() )
      {
        if (!output_poly_file){
          outfp << "CLOSE" << vcl_endl;
          outfp << vsol_list[b]->cast_to_region()->cast_to_polygon()->size() << vcl_endl;

          for (unsigned int i=0; i<vsol_list[b]->cast_to_region()->cast_to_polygon()->size();i++)
          {
            vsol_point_2d_sptr pt = vsol_list[b]->cast_to_region()->cast_to_polygon()->vertex(i);
            outfp << pt->x() << " " << pt->y() << vcl_endl;
          }
        }
        else {
          polyf << "NVERTS: " << vsol_list[b]->cast_to_region()->cast_to_polygon()->size() << vcl_endl;
        
          polyf << "X: " << vcl_endl;
          for (unsigned int i=0; i<vsol_list[b]->cast_to_region()->cast_to_polygon()->size();i++)
            polyf << vsol_list[b]->cast_to_region()->cast_to_polygon()->vertex(i)->x() << " ";
          polyf << vcl_endl;
          
          polyf << "Y: " << vcl_endl;
          for (unsigned int i=0; i<vsol_list[b]->cast_to_region()->cast_to_polygon()->size();i++)
            polyf << vsol_list[b]->cast_to_region()->cast_to_polygon()->vertex(i)->y() << " ";
          polyf << vcl_endl;
        }
      }
    }
    
    if (!output_poly_file) //close file
      outfp.close();
  }

  if (output_poly_file)
    polyf.close();

  return true;
}

bool vidpro1_save_con_process::finish() 
{
    return true;
}
