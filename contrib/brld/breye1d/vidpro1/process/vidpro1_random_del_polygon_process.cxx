// This is vidpro1/process/vidpro1_random_del_polygon_process.h

#include <vcl_iostream.h>
#include <vcl_fstream.h>
#include <vcl_cstdio.h>

#include <vidpro1/process/vidpro1_random_del_polygon_process.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_polygon_2d_sptr.h>
#include <vsol/vsol_point_2d.h>
#include <vnl/vnl_math.h>
//#include <dbsol/dbsol_algos.h>
#include <dbsol/algo/dbsol_curve_algs.h>

vidpro1_random_del_polygon_process::vidpro1_random_del_polygon_process() : bpro1_process(), first_frame_(true)
{
  if( !parameters()->add( "percentage of length to delete:", "-perc_d", (float) 20) ||
      !parameters()->add( "deletion sigma:", "-sigma", (float) 2) ||
      !parameters()->add( "divident to find number of deleted segments:", "-divident", (float) 8.0f) ||
      !parameters()->add( "delete segments? (if both true, both adds spurious lines and deletes segments)", "-delete", false) ||
      !parameters()->add( "add spurious lines?", "-add", false) ||
      !parameters()->add( "percentage of length to add:", "-perc_add", (float) 20) ||
      !parameters()->add( "addition sigma:", "-sigma_add", (float) 5) ||
      !parameters()->add( "divident to find number of added line segments:", "-divident_add", (float) 0.01f) 
      )
  {
    vcl_cerr << "ERROR: Adding parameters in vidpro1_random_del_polygon_process::vidpro1_random_del_polygon_process()" << vcl_endl;
  }
}

//: Clone the process
bpro1_process*
vidpro1_random_del_polygon_process::clone() const
{
  return new vidpro1_random_del_polygon_process(*this);
}

vcl_vector< vcl_string > vidpro1_random_del_polygon_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}

vcl_vector< vcl_string > vidpro1_random_del_polygon_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  to_return.push_back( "vsol2D" );
  return to_return;
}

bool vidpro1_random_del_polygon_process::execute()
{
  bpro1_filepath con_output, poly_output;
  float perc_d, sigma, divident, perc_add, sigma_add, divident_add;
  parameters()->get_value( "-perc_d" , perc_d);
  parameters()->get_value( "-sigma" , sigma);
  parameters()->get_value( "-divident" , divident);
  parameters()->get_value( "-perc_add" , perc_add);
  parameters()->get_value( "-sigma_add" , sigma_add);
  parameters()->get_value( "-divident_add" , divident_add);
  bool delete_segments, add_lines;
  parameters()->get_value( "-delete", delete_segments);
  parameters()->get_value( "-add", add_lines);

  // get input storage class
  vidpro1_vsol2D_storage_sptr input_vsol;
  input_vsol.vertical_cast(input_data_[0][0]);

  // parse through all the vsol classes and save curve objects only
  vcl_vector< vsol_spatial_object_2d_sptr > vsol_list = input_vsol->all_data();
  
  if (!vsol_list.size())
    return false;

  if (!vsol_list[0]->cast_to_region() || !vsol_list[0]->cast_to_region()->cast_to_polygon())
    return false;

  vsol_polygon_2d_sptr poly = vsol_list[0]->cast_to_region()->cast_to_polygon();

  if (delete_segments) {
    if (add_lines) {  // both delete and add

      vcl_vector<vsol_spatial_object_2d_sptr> pieces;
      vcl_vector<vsol_polyline_2d_sptr> del_pieces;
      if (!dbsol_curve_algs::segment_addition_and_deletion(poly, pieces, del_pieces, perc_add, sigma_add, divident_add, perc_d, sigma, divident))
        return false;

      vcl_cout << "there are " << pieces.size() << " remaining pieces and " << del_pieces.size() << " deleted pieces\n";

      if (!pieces.size() || !del_pieces.size()) {
        vcl_cout << "vidpro1_random_del_polygon_process::execute() - both deleted and non deleted pieces are empty!\n";
        return false;
      }

      //: add them to the output storage
      vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
      for (unsigned i = 0; i<pieces.size(); i++) {
        //vcl_cout << "remaining piece: " << i << " size: " << pieces[i]->size() << vcl_endl;
        output_vsol->add_object(pieces[i], "remains");
      }
      if (!output_data_.size())
        output_data_.push_back(vcl_vector< bpro1_storage_sptr > (1,output_vsol));
      else
        output_data_[0].push_back(output_vsol);

      vidpro1_vsol2D_storage_sptr output_vsol2 = vidpro1_vsol2D_storage_new();
      for (unsigned i = 0; i<del_pieces.size(); i++) {
        vcl_cout << "deleted piece: " << i << " size: " << del_pieces[i]->size() << vcl_endl;
        output_vsol2->add_object(del_pieces[i]->cast_to_spatial_object(), "deleted");
      }

      output_data_[0].push_back(output_vsol2);
    
    } else {  // just delete

      vcl_vector<vsol_polyline_2d_sptr> pieces;
      vcl_vector<vsol_polyline_2d_sptr> del_pieces;
      if (!dbsol_curve_algs::segment_wise_deletion(poly, pieces, del_pieces, perc_d, sigma, divident))
        return false;

      vcl_cout << "there are " << pieces.size() << " remaining pieces and " << del_pieces.size() << " deleted pieces\n";

      if (!pieces.size() || !del_pieces.size()) {
        vcl_cout << "vidpro1_random_del_polygon_process::execute() - both deleted and non deleted pieces are empty!\n";
        return false;
      }

      //: add them to the output storage
      vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
      for (unsigned i = 0; i<pieces.size(); i++) {
        vcl_cout << "remaining piece: " << i << " size: " << pieces[i]->size() << vcl_endl;
        output_vsol->add_object(pieces[i]->cast_to_spatial_object(), "remains");
      }
      if (!output_data_.size())
        output_data_.push_back(vcl_vector< bpro1_storage_sptr > (1,output_vsol));
      else
        output_data_[0].push_back(output_vsol);

      vidpro1_vsol2D_storage_sptr output_vsol2 = vidpro1_vsol2D_storage_new();
      for (unsigned i = 0; i<del_pieces.size(); i++) {
        vcl_cout << "deleted piece: " << i << " size: " << del_pieces[i]->size() << vcl_endl;
        output_vsol2->add_object(del_pieces[i]->cast_to_spatial_object(), "deleted");
      }

      output_data_[0].push_back(output_vsol2);

    }
  
  } else {
    
    if (add_lines) {
    
      vcl_vector<vsol_spatial_object_2d_sptr> pieces;
      if (!dbsol_curve_algs::segment_addition(poly, pieces, perc_add, sigma_add, divident_add))
        return false;

      vcl_cout << "there are " << pieces.size() << " pieces\n";

      //: add them to the output storage
      vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
      for (unsigned i = 0; i<pieces.size(); i++) {
        //vcl_cout << "remaining piece: " << i << " size: " << pieces[i]->size() << vcl_endl;
        output_vsol->add_object(pieces[i], "pieces");
      }
      if (!output_data_.size())
        output_data_.push_back(vcl_vector< bpro1_storage_sptr > (1,output_vsol));
      else
        output_data_[0].push_back(output_vsol);
   
    } else 
      return false;
  }

  
  return true;
}

bool vidpro1_random_del_polygon_process::finish() 
{
    return true;
}
