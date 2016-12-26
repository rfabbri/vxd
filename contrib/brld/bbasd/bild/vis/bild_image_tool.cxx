// This is basic/bild/vis/bild_image_tool.cxx
//:
// \file

#include "bild_image_tool.h"

#include <bvis1/bvis1_manager.h>
#include <vgui/vgui_style.h>

bild_image_tool::bild_image_tool()
{
}

bild_image_tool::~bild_image_tool()
{
}

bool bild_image_tool::set_tableau( const vgui_tableau_sptr& tableau )
{
  vgui_image_tableau_sptr temp_tab;
  temp_tab.vertical_cast(tableau);
  if (!temp_tab.ptr())
    return false;
    
  if (this->set_storage(bvis1_manager::instance()->storage_from_tableau(tableau))){
    tableau_ = temp_tab;
    return true;
  }
  return false;
}

bool bild_image_tool::set_storage ( const bpro1_storage_sptr& storage)
{
  if (!storage.ptr())
    return false;
  //make sure its an image class
  if (storage->type() == "image"){
    storage_.vertical_cast(storage);
    return true;
  }
  return false;
}

vgui_image_tableau_sptr bild_image_tool::tableau()
{
  return tableau_;
}

vidpro1_image_storage_sptr bild_image_tool::storage()
{
  return storage_;
}
