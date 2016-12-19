// This is brl/vidpro1/process/vidpro1_load_video_and_polys_process.cxx

//:
// \file

#include <vidpro1/process/vidpro1_load_video_and_polys_process.h>
#include <vcl_iostream.h>

#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>

#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_polygon_2d_sptr.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_point_2d_sptr.h>

#include <vil/vil_new.h>
#include <vil/vil_image_resource_sptr.h>
#include <vcl_cmath.h>
#include <vcl_algorithm.h>
#include <vul/vul_file_iterator.h>

//: Constructor
vidpro1_load_video_and_polys_process::vidpro1_load_video_and_polys_process() : bpro1_process(), num_frames_(0)
{
    if( !parameters()->add( "Video file <filename...>" , "-video_filename", bpro1_filepath("","*") )||
        !parameters()->add( "Poly file <filename...>" , "-poly_filename", bpro1_filepath("","*.txt") )||
        //: update vidl1 in vxl for this option to be effective
        !parameters()->add( "mina" , "-mina",float(0.0) )||
        !parameters()->add( "maxa: " , "-maxa",float(5.0) )||
        !parameters()->add( "gamma: " , "-gamma",float(0.5) )
        )
    {
        vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
    }
}


//: Destructor
vidpro1_load_video_and_polys_process::~vidpro1_load_video_and_polys_process()
{
}


//: Clone the process
bpro1_process*
vidpro1_load_video_and_polys_process::clone() const
{
    return new vidpro1_load_video_and_polys_process(*this);
}


//: Return the name of the process
vcl_string vidpro1_load_video_and_polys_process::name()
{
    return "Load Video And Polys";
}


//: Call the parent function and reset num_frames_
void
vidpro1_load_video_and_polys_process::clear_output(int resize)
{
  num_frames_ = 0;
  bpro1_process::clear_output(resize);  
}


//: Returns a vector of strings describing the input types to this process
vcl_vector< vcl_string >
vidpro1_load_video_and_polys_process::get_input_type()
{
    vcl_vector< vcl_string > to_return;
    // no input type required
    to_return.clear();

    return to_return;
}


//: Returns a vector of strings describing the output types of this process
vcl_vector< vcl_string >
vidpro1_load_video_and_polys_process::get_output_type()
{
    vcl_vector< vcl_string > to_return;
    to_return.push_back( "image" );
    to_return.push_back( "vsol2D" );
    return to_return;
}


//: Returns the number of input frames to this process
int
vidpro1_load_video_and_polys_process::input_frames()
{
    return 0;
}


//: Returns the number of output frames from this process
int
vidpro1_load_video_and_polys_process::output_frames()
{
    return num_frames_;
}


//: Run the process on the current frame
bool
vidpro1_load_video_and_polys_process::execute()
{
#if 0
    bpro1_filepath video_path;
    bpro1_filepath poly_path;
    float mina, maxa, gamma;
    parameters()->get_value( "-video_filename" , video_path );
    parameters()->get_value( "-poly_filename" , poly_path );
    parameters()->get_value( "-mina" , mina );  
    parameters()->get_value( "-maxa" , maxa );
    parameters()->get_value( "-gamma" , gamma );

    //create a new mapper
    /*bvis1_mapper_sptr mapper = new bvis1_mapper(mina, maxa, gamma);

    //set the mapper on the displayer
    bvis1_displayer_sptr displayer = bvis1_manager::instance()->displayer("vsol2D");
    displayer->set_mapper(mapper);
    */
    vcl_string poly_filename = poly_path.path;
    vcl_ifstream fp(poly_filename.c_str());
    if (!fp) {
      vcl_cout<<" Unable to Open "<< poly_filename <<vcl_endl;
      return false;
    }

    vcl_vector<vcl_vector<vcl_pair<vsol_polygon_2d_sptr, float> > > frame_polygons;
    int frame_no;
    fp >> frame_no;
    for (int fi = 0; fi<frame_no; fi++) {
      vcl_vector<vcl_pair<vsol_polygon_2d_sptr, float> > polygons;
      int poly_cnt;
      fp >> poly_cnt;
      for (int pi = 0; pi < poly_cnt; pi++) {
        int size; float attr;
        vcl_vector<vsol_point_2d_sptr> tmp;
        fp >> size; fp >> attr;
        for (int s = 0; s < size; s++) {
          float x;
          fp >> x;
          
          vsol_point_2d_sptr point = new vsol_point_2d(x, 0);
          tmp.push_back(point);
        }
        vcl_cout << "\n";
        for (int s = 0; s < size; s++) {
          float y;
          fp >> y;
          
          tmp[s]->set_y(y);
        }
        for (int s = 0; s < size; s++)
          vcl_cout << (*tmp[s]) << " ";
        vcl_cout << "\n\n";
        vsol_polygon_2d_sptr poly = new vsol_polygon_2d(tmp);
        vcl_pair<vsol_polygon_2d_sptr, float> pair(poly, attr);
        polygons.push_back(pair);
      }
      frame_polygons.push_back(polygons);
    }
    
    vcl_cout << "read: " << frame_polygons.size() << " frames of polygons\n";
    fp.close();

    vcl_vector<vcl_string> video_files;
    for(vul_file_iterator fn = video_path.path; fn; ++fn)
      video_files.push_back(fn());
//    while(!video_files.empty())
 //   {
  //    vidl1_movie_sptr my_movie = 0;
      //vidl1_io::load_movie(video_files.back().c_str());
  
//      if (!my_movie) {
          vcl_cerr << "Failed to load movie file: "<< video_files.back() << vcl_endl;
          return false;
//      }
  
/* FIXME vidl1 Bug
      vidl1_movie::frame_iterator pframe = my_movie->last();
      for (; pframe >= my_movie->first(); --pframe){
          vidpro1_image_storage_sptr image_storage = vidpro1_image_storage_new();
          vil_image_resource_sptr img=pframe->get_resource();
          
          image_storage->set_image( img );
          output_data_.push_back(vcl_vector< bpro1_storage_sptr > (1,image_storage));
      }
      
      num_frames_ += my_movie->length();
      video_files.pop_back();
*/
    }

    vcl_cout << "number of frames " << num_frames_ << "\n";

    vcl_vector<vcl_string> cats;
    cats.push_back("car");
    cats.push_back("utility");
    cats.push_back("pickup");
    cats.push_back("none");
    cats.push_back("van");

    // push vsol2D storage to each frame loaded
    int output_size = output_data_.size() < frame_polygons.size() ? output_data_.size() : frame_polygons.size();
    vcl_cout << "output_size is " << output_size << " \n";
    for (int oi = 0; oi < output_size; oi++) {
      vcl_vector< vsol_spatial_object_2d_sptr > contours;
      vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
      for (unsigned int ii = 0; ii<frame_polygons[oi].size(); ii++) {
        vcl_string cat = cats[int(frame_polygons[oi][ii].second)];
        output_vsol->add_object(frame_polygons[oi][ii].first->cast_to_spatial_object(), cat);
      }
      output_data_[oi].push_back(output_vsol);
    }
    for (unsigned int oi = output_size; oi < output_data_.size(); oi++) {
      vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
      output_data_[oi].push_back(output_vsol);
    }
      
#endif
    return true;
}


//: Finish
bool
vidpro1_load_video_and_polys_process::finish() 
{
    return true;
}



