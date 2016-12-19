// This is breye1/bvis1/displayer/bvis1_vtol_displayer.cxx

#include <bvis1/displayer/bvis1_vtol_displayer.h>
#include <vidpro1/storage/vidpro1_vtol_storage.h>
#include <bgui/bgui_vsol2D_tableau.h>

#include <vsol/vsol_spatial_object_2d.h>
#include <vtol/vtol_topology_object.h>
#include <vtol/vtol_edge_2d.h>

//: Create a tableau if the storage object is of type image
vgui_tableau_sptr
bvis1_vtol_displayer::make_tableau( bpro1_storage_sptr storage) const
{
  // Return a NULL tableau if the types don't match
  if( storage->type() != this->type() )
    return NULL;

  // Cast the storage object into an image storage object
  vidpro1_vtol_storage_sptr vtol_storage;
  vtol_storage.vertical_cast(storage);

  // Create a new vsol2D tableau
  bgui_vsol2D_tableau_sptr vsol2D_tab;
  vsol2D_tab = bgui_vsol2D_tableau_new();

  for ( vcl_set<vtol_topology_object_sptr>::const_iterator itr = vtol_storage->begin();
        itr != vtol_storage->end();  ++itr ) {

    // Matt Leotta:
    //  For now I am assuming that the vtol objects are all vtol_edge_2d
    //  (from the VD edge detector).  This should be made more generic in
    //  the future.  Maybe a vtol tableau?     
    vtol_edge *edge = (*itr)->cast_to_edge();
    if (edge){
      vtol_edge_2d *edge_2d = edge->cast_to_edge_2d();
      if (edge_2d){
        vsol_curve_2d *curve_2d = edge_2d->curve().ptr();
        if (curve_2d){
          vdgl_digital_curve_sptr dc = curve_2d->cast_to_vdgl_digital_curve();
          if (dc.ptr()){
            vsol2D_tab->add_edgel_curve(dc);
          } else vcl_cout << "failed digital curve" << vcl_endl;
        } else vcl_cout << "failed curve_2d" << vcl_endl;
      } else vcl_cout << "failed edge_2d" << vcl_endl;
    } else vcl_cout << "failed edge" << vcl_endl;
        
  }

  return vsol2D_tab;
}
