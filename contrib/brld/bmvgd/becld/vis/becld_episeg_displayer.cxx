// This is bmvgd/becld/vis/becld_episeg_displayer.cxx

#include "becld_episeg_displayer.h"
#include <becld/pro/becld_episeg_storage.h>
#include <vgui/vgui_displaylist2D_tableau.h>
#include "becld_episeg_soview2D.h"


//: This defines the default style used to draw all epi_segments
vgui_style_sptr
becld_episeg_displayer::seg_style = vgui_style::new_style(0.0, 1.0, 0.0, 2.0, 2.0);

  
// Constructor
becld_episeg_displayer::becld_episeg_displayer()
{  
}

//: Create a tableau if the storage object is of type bmrf
vgui_tableau_sptr
becld_episeg_displayer::make_tableau( bpro1_storage_sptr storage) const
{
  // Return a NULL tableau if the types don't match
  if( storage->type() != this->type() )
    return NULL;

  // Cast the storage object into a episeg storage object
  becld_episeg_storage_sptr episeg_storage;
  episeg_storage.vertical_cast(storage);

  vcl_vector<becld_episeg_sptr> episegs = episeg_storage->episegs();

  // Create a new tableau
  vgui_displaylist2D_tableau_sptr episeg_tab = vgui_displaylist2D_tableau_new();
  
  for( vcl_vector<becld_episeg_sptr>::const_iterator itr = episegs.begin();
       itr != episegs.end(); ++itr){
      becld_episeg_soview2D* obj = new becld_episeg_soview2D(*itr);
      episeg_tab->add( obj );
      obj->set_style(seg_style);
  }
 

  return episeg_tab;
}
