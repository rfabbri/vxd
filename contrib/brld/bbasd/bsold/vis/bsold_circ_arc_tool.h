/**************************************************************/
/*  Name: Isabel restrepo
//  File: dbsol_circ_arc_tool.h
//  Asgn: bvis
//  Date: Thu Aug 14 15:24:53 EDT 2003
***************************************************************/

#ifndef dbsol_circ_arc_tool_h
#define dbsol_circ_arc_tool_h

#include <bvis1/tool/bvis1_vsol2D_tools.h>
#include "dbsol2D_tableau_sptr.h"



// ***************************************************
// Various Geometry tools
// ***************************************************



class dbsol_circ_arc_tool : public bvis1_vsol2D_tool {

public:

  dbsol_circ_arc_tool();
  virtual ~dbsol_circ_arc_tool();

  bool handle( const vgui_event & e, 
               const bvis1_view_tableau_sptr& view );

  vcl_string name() const;
  void deactivate();

protected:

  vgui_event_condition gesture0;
  vgui_event_condition gesture1;
  vgui_event_condition gesture2;

  dbsol2D_tableau_sptr active_tableau_;
  vidpro1_vsol2D_storage_sptr active_storage_;
  
private:

  bool active;
  bool out_of_region;
  int event_count;
  int n;


  vsol_point_2d p0; 
  vsol_point_2d p1; 
  vsol_point_2d p2;
  vsol_point_2d p3;
  vsol_point_2d p4;
  vsol_point_2d center;
  vgl_vector_2d<double> v1; 
  vgl_vector_2d<double> v2;
  vgl_vector_2d<double> v3;
  vgl_vector_2d<double> v1_ortho;
  vgl_vector_2d<double> v2_v1;
  vgl_vector_2d<double> v3_v2;
  vcl_vector< vsol_point_2d_sptr > arc_point_list;
  double last_x;
  double last_y;
  double temp_x;
  double temp_y;
  double dir;
  double radius;      
  double theta;
  double angle_v1;

 
};




#endif

