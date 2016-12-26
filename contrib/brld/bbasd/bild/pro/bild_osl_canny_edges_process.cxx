// This is basic/dbil/pro/dbil_osl_canny_edges_process.cxx

//:
// \file

#include "dbil_osl_canny_edges_process.h"

#include <vil/vil_image_view.h>
#include <vil/vil_convert.h>
#include <vil/vil_new.h>
#include <vil1/vil1_memory_image_of.h>
#include <vil1/vil1_vil.h>

#include <osl/osl_canny_ox_params.h>
#include <osl/osl_canny_ox.h>
#include<brip/brip_vil_float_ops.h>

#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>
#include <bpro1/bpro1_parameters.h>

// ------------------------------------------------------------------
//: Constructor
dbil_osl_canny_edges_process::
dbil_osl_canny_edges_process() : bpro1_process()
{
    if( !parameters()->add( "Sigma" ,"-sigma" ,(float)1.0 ) ||
        !parameters()->add( "Low threshold" ,  "-lowthresh" ,  float(2.0) ) ||
        !parameters()->add( "High threshold" , "-howthresh" ,  float(12.0) ) ||
        !parameters()->add( "Border Size" , "-bordersize" ,  int(2) ) ||
        !parameters()->add( "Border Intesnity Value" , "-borderint" ,  float(0.0) ) ||
        !parameters()->add( "Scale" , "-scale" ,  float(5.0) ) ||
        !parameters()->add( "follow strategy" , "-follow strategy" ,  int(2) ) || 
        !parameters()->add( "Min length of curves" ,      "-lenofcurves" ,       (int)10 )||
        !parameters()->add( "Min Edgel Intensity" ,      "-minintensity" ,       (int)60 ) 
        )
    {
        vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
    }
    
}



// ------------------------------------------------------------------
//: Destructor
dbil_osl_canny_edges_process::
~dbil_osl_canny_edges_process()
{
   
}

// ------------------------------------------------------------------
//: Clone the process
bpro1_process* dbil_osl_canny_edges_process::
clone() const
{
    return new dbil_osl_canny_edges_process(*this);
}


// ------------------------------------------------------------------
vcl_string dbil_osl_canny_edges_process::
name()
{
    return "Binary Osl Canny edges";
}


// ------------------------------------------------------------------
//: Return input types
vcl_vector< vcl_string > dbil_osl_canny_edges_process::
get_input_type()
{
    vcl_vector< vcl_string > to_return;
    to_return.push_back( "image" );
    return to_return;
}

// ------------------------------------------------------------------
//: Return output types
vcl_vector< vcl_string > dbil_osl_canny_edges_process::
get_output_type()
{
    vcl_vector< vcl_string > to_return;
    to_return.push_back( "image" );
    to_return.push_back( "image" );
    return to_return;
}

// ------------------------------------------------------------------
//: Returns the number of input frames to this process
int dbil_osl_canny_edges_process::
input_frames()
{
    return 1;
}

// ------------------------------------------------------------------
//: Returns the number of output frames from this process
int dbil_osl_canny_edges_process::
output_frames()
{
    return 1;
}


// ------------------------------------------------------------------
//: Main function of the process 
bool dbil_osl_canny_edges_process::
execute()
{
    if ( input_data_.size() != 1 )
    {
        vcl_cout << "In dbil_osl_canny_edges_process::execute() - not exactly one"
            << " input image \n";
        return false;
    }
    clear_output();

    // get input image from the storage class
    vidpro1_image_storage_sptr frame_image;
    frame_image.vertical_cast(input_data_[0][0]);
    vil_image_resource_sptr image_sptr = frame_image->get_image();
    vil_image_view<float> float_curr_view=brip_vil_float_ops::convert_to_float(*image_sptr);
    vil_image_view< unsigned char > image_view = brip_vil_float_ops::convert_to_byte(float_curr_view);
    vil_image_view< unsigned char > greyscale_view;
    if( image_view.nplanes() == 3 ) {
        vil_convert_planes_to_grey( image_view , greyscale_view );
    } 
    else if ( image_view.nplanes() == 1 ) {
        greyscale_view = image_view;
    }
    else
    {
        vcl_cerr << "Returning false. nplanes(): " << image_view.nplanes() << vcl_endl;
        return false;
    }
    
    

    // prepare to-return images
    vil_image_view< unsigned char > binary_edge_img(greyscale_view.ni(),greyscale_view.nj(),1);
    vil_image_view< unsigned char > dir_img(greyscale_view.ni(),greyscale_view.nj(),1);
    binary_edge_img.fill(1);
    dir_img.fill(255);
 
    vil1_memory_image_of< unsigned char > img = vil1_from_vil_image_view( greyscale_view );

    // get parameters of Canny edge detector
    osl_canny_ox_params canny_params;

    parameters()->get_value( "-sigma" ,  canny_params.sigma);
    parameters()->get_value( "-lowthresh" , canny_params.low );
    parameters()->get_value( "-howthresh" , canny_params.high );
    parameters()->get_value( "-lenofcurves" ,canny_params.min_length );
    parameters()->get_value( "-minintensity" ,canny_params.edge_min );

    // Canny edge detector execution
    osl_canny_ox detector(canny_params);
    vcl_list<osl_edge*>  edges;

    // =========================================
    detector.detect_edges(img, &edges);
    // =========================================

    // put edge information (location and direction) on to-return images
    // `binary_edge_img' and `dir_img'
    while (edges.size() > 0)
    {
        osl_edgel_chain * ec=(osl_edgel_chain *)edges.front();
        for(unsigned i=0;i<ec->size();i++)
        {
            unsigned int x = (unsigned int)vcl_floor(ec->GetY(i));
            unsigned int y = (unsigned int)vcl_floor(ec->GetX(i));

            // location of edge
            binary_edge_img(x,y)=255;
            
            // tangent varies form 0 to 2pi but it is encoded to 0-180
            dir_img(x,y)=(unsigned int)vcl_floor((ec->GetTheta(i)+180)/2);
        }
        edges.pop_front();
    }
     
    // create output storage classes
    // binary edge
    vidpro1_image_storage_sptr output_img_storage = vidpro1_image_storage_new();
    output_img_storage->set_image(
      vil_new_image_resource_of_view(binary_edge_img));
    output_data_[0].push_back(output_img_storage);
    output_img_storage->set_name("Binary OSL Canny Edges");
      
    // edge with direction encoded
    vidpro1_image_storage_sptr output_img_storage2 = 
      vidpro1_image_storage_new();
    output_img_storage2->set_image(vil_new_image_resource_of_view(dir_img ) );
    output_data_[0].push_back(output_img_storage2);
    output_img_storage2->set_name("Dir OSL Canny Edges");

    clear_input();
    return true;
}


// ------------------------------------------------------------------
//: 
bool dbil_osl_canny_edges_process::
finish()
{
    return true;
}



