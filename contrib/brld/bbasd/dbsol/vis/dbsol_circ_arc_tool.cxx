/*************************************************************************
 *    NAME: Isabel Restrepo
 *    USER: mrj
 *    FILE: dbsol_circ_arc_tool.cxx
 *    DATE: Aug 25  2006
 *************************************************************************/
#include "dbsol_circ_arc_tool.h"
#include <dbgl/algo/dbgl_arc_center.h> 
#include <bvis1/tool/bvis1_vsol2D_tools.h>
#include <bvis1/bvis1_manager.h>
#include <vgui/vgui_style.h>
#include <vgui/vgui_command.h>
#include <vgui/vgui_dialog.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_conic_2d.h>
#include <dbsol/dbsol_circ_arc_2d.h>
#include "dbsol2D_tableau.h"

#include <vdgl/vdgl_digital_curve_sptr.h>
#include <vdgl/vdgl_edgel_chain_sptr.h>
#include <vdgl/vdgl_digital_curve.h>
#include <vdgl/vdgl_edgel_chain.h>
#include <vdgl/vdgl_interpolator_linear.h>

#include <bdgl/bdgl_curve_algs.h>

#include <vgl/algo/vgl_h_matrix_2d.h>
#include <vgl/algo/vgl_homg_operators_2d.h>
#include <vnl/vnl_math.h>

#include <vsol/vsol_box_2d.h>


/*************************************************************************
 * ***********************************************************************
 * ***********************************************************************
 * ***********************************************************************
 * ***********************************************************************/


/*************************************************************************
 * Function Name: dbsol_circ_arc_tool::dbsol_circ_arc_tool
 * Parameters:
 * Effects:
 *************************************************************************/

dbsol_circ_arc_tool::dbsol_circ_arc_tool()
{
  active = false;
  event_count=0;
  out_of_region =0;
  n=0;
  gesture0 = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true);
  arc_point_list.clear();
}

/*************************************************************************
 * Function Name: dbsol_circ_arc_tool::~dbsol_circ_arc_tool
 * Parameters:
 * Effects:
 *************************************************************************/

dbsol_circ_arc_tool::~dbsol_circ_arc_tool()
{
 
  arc_point_list.clear();
}


/*************************************************************************
 * Function Name: dbsol_circ_arc_tool::handle
 * Parameters:  const vgui_event & e
 * Returns: bool
 * Effects:
 *************************************************************************/
bool
dbsol_circ_arc_tool::handle( const vgui_event & e, 
                                   const bvis1_view_tableau_sptr& view )
{
  float ix, iy;
  unsigned j=0;

  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);

      
  if( e.type == vgui_MOTION ) {
    last_x = ix;
    last_y = iy;
    if( active ) {
      
      
      if (event_count==2){
        p2.set_x(ix);
        p2.set_y(iy);
        v2_v1=p0.get_p() - p1.get_p();
        v3_v2=p2.get_p() - p1.get_p();
        if (dot_product(v3_v2, v2_v1)< 0){
          out_of_region =1;
        }
        else
          out_of_region =0;

        center=getArcCenterFromThreePoints(p0.x(),p0.y(),p1.x(),p1.y(),p2.x(),p2.y());
        v1= p0.get_p() - center.get_p();
        v2= p1.get_p() - center.get_p();
        v3= p2.get_p() - center.get_p();
        angle_v1= atan2(v1.y(),v1.x());
        radius= length(v1);
        theta=angle(v1,v2);
        v1_ortho.set(-v1.y(), v1.x());

        if  ((dot_product(v2,v1_ortho)* dot_product(v3,v1_ortho))>0){
          if ( dot_product(v2,v1_ortho)>0) // + orientation of v1 respect arc center
            dir=1;
          else
            dir=-1;
        } else{
          theta= 2*vnl_math::pi - theta;
          if ( dot_product(v2,v1_ortho)>0) // + orientation of v1 respect arc center
            dir=-1;
          else
            dir=1;
      
      }
      
      }
      tableau()->post_overlay_redraw();
    }
  }
  

  if( !active && gesture0(e) ) {
    active = true;
    active_tableau_.vertical_cast(tableau());
    active_storage_ = storage();
    event_count++;
    p0.set_x(ix);
    p0.set_y(iy);
    return true;
  } 
  else if(active) {

    if( gesture0(e) ) {
      
      event_count++;
      if (event_count==2){
      p1.set_x(ix);
      p1.set_y(iy);
      return true;
      }
      

    }

    if (e.type == vgui_OVERLAY_DRAW){
      if (event_count<2){// Displays the radius
        glLineWidth(1);
        glColor3f(0.7f,0.7f,0.7f);
        glBegin(GL_LINES);
        glVertex2f(p0.x(), p0.y() );
        glVertex2f(last_x,last_y);
        glEnd();
        return true;
      }
      else{  
        
        arc_point_list.clear();
        glLineWidth(1);
        glColor3f(0.7f,0.7f,0.7f);
        

        if (out_of_region){
          glBegin(GL_LINES);
          glVertex2f(p0.x(),p0.y());
          glVertex2f(p1.x(),p1.y());

        }else{
        glBegin(GL_LINE_STRIP);
        for (float temp_theta=0; temp_theta<theta; temp_theta+=0.1f){
            n++;
            temp_x= center.x() + radius * cos(angle_v1+dir*temp_theta);
            temp_y= center.y() + radius * sin(angle_v1+dir*temp_theta);
            arc_point_list.push_back( vsol_point_2d_sptr( new vsol_point_2d( temp_x , temp_y ) ) );
            glVertex2f(temp_x,temp_y);
          
          }
          glVertex2f(p1.x(),p1.y());
        }
          glEnd();  
            return true;

      } 
    }
    if(event_count>2) {

      // The following points were defined such that theta is split in half, and the tool
      // allows angles greater than pi to be drawn.
      p4=vsol_point_2d((center.x() + radius * cos(angle_v1+dir*theta/2)),(center.y() + radius * sin(angle_v1+dir*theta/2)));
      p2=vsol_point_2d((center.x() + radius * cos(angle_v1+dir*theta/4)),(center.y() + radius * sin(angle_v1+dir*theta/4)));
      p3=vsol_point_2d((center.x() + radius * cos(angle_v1+dir*3*theta/4)),(center.y() + radius * sin(angle_v1+dir*3*theta/4)));
      
      dbsol_circ_arc_2d_sptr newarc= new dbsol_circ_arc_2d(p0,p2,p4);
      dbsol_circ_arc_2d_sptr newarc1= new dbsol_circ_arc_2d(p4,p3,p1);
      
      vsol_point_2d_sptr tp0 = new vsol_point_2d(p0);
      vsol_point_2d_sptr tp2 = new vsol_point_2d(p2);
      vsol_point_2d_sptr tp4 = new vsol_point_2d(p4);
      vsol_point_2d_sptr tp1 = new vsol_point_2d(p1);
      
      newarc->set_p0(tp0);
      newarc->set_p1(tp4);
      newarc1->set_p0(tp4);
      newarc1->set_p1(tp1);
        

      //add to storage
      active_storage_->add_object( newarc->cast_to_spatial_object() , active_group_ );
      active_storage_->add_object( newarc1->cast_to_spatial_object() , active_group_ );
      
      //add to the tableau
      active_tableau_->set_current_grouping( active_group_ );
      active_tableau_->add_dbsol_circ_arc_2d( newarc );
      active_tableau_->add_dbsol_circ_arc_2d( newarc1 );
      active_tableau_->set_current_grouping( "default" );
      
      active_tableau_->post_redraw();
      active = false;
      out_of_region=false;
            event_count=0;

      active_tableau_ = NULL;
      active_storage_ = NULL;
      
      arc_point_list.clear();
      return true;
    }
  }
  return false;

}


/*************************************************************************
 * Function Name: dbsol_circ_arc_tool::name
 * Parameters:
 * Returns: vcl_string
 * Effects:
 *************************************************************************/
vcl_string
dbsol_circ_arc_tool::name() const
{
  return "arc";
}


/*************************************************************************
 * Function Name: dbsol_circ_arc_tool::reset
 * Parameters:
 * Returns: void
 * Effects:
 *************************************************************************/
void
dbsol_circ_arc_tool::deactivate()
{
  active = false;

}
