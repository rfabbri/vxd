// This is vidpro1/process/vidpro1_save_ps_process.h

#include <vcl_iostream.h>
#include <vcl_fstream.h>

#include <vidpro1/process/vidpro1_save_ps_process.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_point_2d.h>
#include <vnl/vnl_math.h>

#include <vul/vul_psfile.h>

vidpro1_save_ps_process::vidpro1_save_ps_process() : bpro1_process()
{
  if( !parameters()->add( "Output file <filename...>" , "-psoutput" , bpro1_filepath("","*.ps") ))
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Clone the process
bpro1_process*
vidpro1_save_ps_process::clone() const
{
  return new vidpro1_save_ps_process(*this);
}


vcl_vector< vcl_string > vidpro1_save_ps_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;

  to_return.push_back( "vsol2D" );

  return to_return;
}

vcl_vector< vcl_string > vidpro1_save_ps_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;

  to_return.clear();

  return to_return;
}

bool vidpro1_save_ps_process::execute()
{
  bpro1_filepath output;
  parameters()->get_value( "-psoutput" , output );
  return savePS(output.path);
}

bool vidpro1_save_ps_process::savePS (vcl_string filename)
{
  // get input storage class
  vidpro1_vsol2D_storage_sptr input_vsol;
  input_vsol.vertical_cast(input_data_[0][0]);

  //1)If file open fails, return.
  vul_psfile psfile1(filename.c_str(), false);

  if (!psfile1){
    vcl_cout << " Error opening file  " << filename.c_str() << vcl_endl;
    return false;
  }

  unsigned char * data= new unsigned char[1024*768];
  for( int i=0;i<1024*768;i++)
    data[i]=255;

  psfile1.set_scale_x(50);
  psfile1.set_scale_y(50);
  psfile1.print_greyscale_image(data,1024,768);
  psfile1.set_line_width(1.0);
  psfile1.set_fg_color(0,0,1);

  // parse through all the vsol classes and save curve objects only
  vcl_vector< vsol_spatial_object_2d_sptr > vsol_list = input_vsol->all_data();
  for (unsigned int b = 0 ; b < vsol_list.size() ; b++ )
  {
    if( vsol_list[b]->cast_to_curve())
    {
      if( vsol_list[b]->cast_to_curve()->cast_to_polyline() )
      {
        for (unsigned int i=1; i<vsol_list[b]->cast_to_curve()->cast_to_polyline()->size();i++)
        {
          vsol_point_2d_sptr p1 = vsol_list[b]->cast_to_curve()->cast_to_polyline()->vertex(i-1);
          vsol_point_2d_sptr p2 = vsol_list[b]->cast_to_curve()->cast_to_polyline()->vertex(i);
          psfile1.line((float)p1->x(), (float)p1->y(), (float)p2->x(), (float)p2->y());
        }
      }
    }
  }
  
  //close file
  psfile1.close();

  return true;
}
