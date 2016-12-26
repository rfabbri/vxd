// This is basic/dbpro/vis/dbpro_basic_gui_observers.h
#ifndef dbpro_basic_gui_observers_h_
#define dbpro_basic_gui_observers_h_
//:
// \file
// \brief  Commonly used dbpro_observers that update vgui_tableau
// \author Matthew Leotta (mleotta@brown.lems.edu)
// \date   April 22, 2009
//
// \verbatim
//  Modifications
// \endverbatim

#include <dbpro/dbpro_observer.h>
#include <vgui/vgui_image_tableau.h>
#include <vil/vil_image_resource.h>

//: update a vgui_image_tableau with a vil_image_resource_sptr
class dbpro_image_observer: public dbpro_observer
{
public:
  dbpro_image_observer(const vgui_image_tableau_sptr& itab)
  : image_tab(itab) {}
  
  //: Called by the process when the data is ready
  virtual bool notify(const dbpro_storage_sptr& data, unsigned long timestamp)
  {
    assert(image_tab);
    assert(data);
    if(data->info() != DBPRO_VALID)
      image_tab->set_image_resource(NULL);
    else{
      assert(data->type_id() == typeid(vil_image_resource_sptr));
      image_tab->set_image_resource(data->data<vil_image_resource_sptr>());
    }
    image_tab->post_redraw();
    return true;
  }
  vgui_image_tableau_sptr image_tab;
};


#endif // dbpro_basic_gui_observers_h_
