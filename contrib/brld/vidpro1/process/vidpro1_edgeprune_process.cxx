#include <vcl_iostream.h>
#include <vcl_fstream.h>
#include <vidpro1/process/vidpro1_edgeprune_process.h>

#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_point_2d.h>
#include <vtol/vtol_edge.h>
#include <vtol/vtol_edge_2d.h>
#include <vdgl/vdgl_digital_curve.h>
#include <vdgl/vdgl_edgel_chain.h>
#include <vdgl/vdgl_edgel.h>
#include <vdgl/vdgl_interpolator.h>

#include <vsol/vsol_line_2d.h>
#include <vsol/vsol_conic_2d.h>
#include <vsol/vsol_region_2d.h>
#include <vsol/vsol_polygon_2d.h>


vidpro1_edgeprune_process::vidpro1_edgeprune_process() : bpro1_process()
{
  if(!parameters()->add( "Prune edges (if not, just copy to output)?" ,                               "-prune" ,  true ) ||
     !parameters()->add( "by length? (otherwise by number of points)" , "-prunel" , true ) ||
     !parameters()->add( "Prune threshold" ,                            "-thres" ,  (int)40 )||
     !parameters()->add( "Input VSOL?" ,                            "-ivsol" ,  true )
     )
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Clone the process
bpro1_process*
vidpro1_edgeprune_process::clone() const
{
  return new vidpro1_edgeprune_process(*this);
}


vcl_vector< vcl_string > vidpro1_edgeprune_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;

  bool i_vsol=false;
  parameters()->get_value( "-ivsol" , i_vsol);

  if (i_vsol)
    to_return.push_back( "vsol2D" );
  else
    to_return.push_back( "vtol" );

  return to_return;
}

vcl_vector< vcl_string > vidpro1_edgeprune_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;

  //is this input or output
  to_return.push_back( "vsol2D" );
  return to_return;
}

bool vidpro1_edgeprune_process::execute()
{
  bool prune=false; //I = true O = false
  parameters()->get_value( "-prune" , prune );
  bool prunel=false; //I = true O = false
  parameters()->get_value( "-prunel" , prunel );
  int thres=0;
  parameters()->get_value( "-thres" , thres );
  bool i_vsol=false;
  parameters()->get_value( "-ivsol" , i_vsol);

  // new vvector to store the points
  vcl_vector< vsol_spatial_object_2d_sptr > contours;


  if (!i_vsol) {
    // create the input storage class
    vidpro1_vtol_storage_sptr input_vtol;
    input_vtol.vertical_cast(input_data_[0][0]);

    for ( vcl_set<vtol_topology_object_sptr>::const_iterator itr = input_vtol->begin();
          itr != input_vtol->end();  ++itr ) {

      //: Adding only curves from the vtol objects
      vtol_edge *edge = (*itr)->cast_to_edge();
      if (edge){
        vtol_edge_2d *edge_2d = edge->cast_to_edge_2d();
        if (edge_2d){
          vsol_curve_2d *curve_2d = edge_2d->curve().ptr();
          if (curve_2d) {
            vdgl_digital_curve_sptr dc = curve_2d->cast_to_vdgl_digital_curve();
            if (dc.ptr()){
              vdgl_edgel_chain_sptr chain = dc->get_interpolator()->get_edgel_chain();
                     
              if ((prune && 
                    ((!prunel && int(chain->size()) > thres) || 
                     (prunel && dc->get_interpolator()->get_length() > thres)
                    )
                   ) || !prune) {
                vcl_vector< vsol_point_2d_sptr > points; 
                for (unsigned int i = 0; i<chain->size(); i++) {
                  vsol_point_2d_sptr newPt = new vsol_point_2d (chain->edgel(i).x(),chain->edgel(i).y());
                  points.push_back(newPt);
                }
                vsol_polyline_2d_sptr newContour = new vsol_polyline_2d (points);
                contours.push_back(newContour->cast_to_spatial_object());
              }

            }
          } else vcl_cout << "failed curve_2d" << vcl_endl;
        } else vcl_cout << "failed edge_2d" << vcl_endl;
      } else vcl_cout << "failed edge" << vcl_endl;
    }

    vcl_cout << "Pruned " << input_vtol->size()-contours.size() << " edges out of " << input_vtol->size() << " edges\n"; 

  } else { // Input Vsol2D

    // create the input storage class
    vidpro1_vsol2D_storage_sptr input_vsol;
    input_vsol.vertical_cast(input_data_[0][0]);

    vcl_vector< vsol_spatial_object_2d_sptr > vsol_list = input_vsol->all_data();

    for (unsigned int b = 0 ; b < vsol_list.size() ; b++ ) 
    {
      vcl_vector<vsol_point_2d_sptr> pts;
      //POINT
      if( vsol_list[b]->cast_to_point() ) {
        if (prune)
          continue;   // remove the points!!
      }
      else if( vsol_list[b]->cast_to_curve())
      {
        //LINE
        if( vsol_list[b]->cast_to_curve()->cast_to_line() )
        {
          vsol_point_2d_sptr p0 = vsol_list[b]->cast_to_curve()->cast_to_line()->p0();
          vsol_point_2d_sptr p1 = vsol_list[b]->cast_to_curve()->cast_to_line()->p1();
          pts.push_back(p0);
          pts.push_back(p1);
        }
        //POLYLINE
        else if( vsol_list[b]->cast_to_curve()->cast_to_polyline() ) {
          vsol_polyline_2d* pline = vsol_list[b]->cast_to_curve()->cast_to_polyline();
          for (unsigned int i = 0; i<pline->size(); i++) 
            pts.push_back(pline->vertex(i));
        }
        // CIRCULAR ARC
        else if (vsol_list[b]->cast_to_curve()->cast_to_conic())
        {
          vcl_cout << "CAUTION: This vsol member is a circular ARC and this process is NOT HANDLING circular arcs!!! Skipping it!\n";
          continue;
        }
      }
      else if( vsol_list[b]->cast_to_region())
      {
        //POLYGON
        if( vsol_list[b]->cast_to_region()->cast_to_polygon() ) {
          vsol_polygon_2d* poly = vsol_list[b]->cast_to_region()->cast_to_polygon();
          for (unsigned int i = 0; i<poly->size(); i++)
            pts.push_back(poly->vertex(i));
          pts.push_back(poly->vertex(0));
        }
      }

      // Now that we got the points, convert it to polyline (whatever the source was), then prune.


      bool include_contour=!prune;
      if(prune) {
        if (!prunel) {
          include_contour = pts.size() > (unsigned)thres;
        } else {
          vsol_polyline_2d pl(pts);
          include_contour = pl.length() > (unsigned)thres;
        }
      }

      if(include_contour)
        contours.push_back(vsol_list[b]);
    }

    vcl_cout << "Pruned " << input_vsol->all_data().size()-contours.size() << " curves out of " << input_vsol->all_data().size() << " curves\n"; 
  }


  // create the output storage class
  vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
  output_vsol->add_objects(contours, "pruned");
  output_data_[0].push_back(output_vsol);

  return true;
}
