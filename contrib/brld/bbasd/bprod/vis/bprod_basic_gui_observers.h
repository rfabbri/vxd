// This is basic/bprod/vis/bprod_basic_gui_observers.h
#ifndef bprod_basic_gui_observers_h_
#define bprod_basic_gui_observers_h_
//:
// \file
// \brief  Commonly used bprod_observers that update vgui_tableau
// \author Matthew Leotta (mleotta@brown.lems.edu)
// \date   April 22, 2009
//
// \verbatim
//  Modifications
// \endverbatim

#include <bprod/bprod_observer.h>
#include <vgui/vgui_image_tableau.h>
#include <vil/vil_image_resource.h>

//: update a vgui_image_tableau with a vil_image_resource_sptr
class bprod_image_observer: public bprod_observer
{
public:
  bprod_image_observer(const vgui_image_tableau_sptr& itab)
  : image_tab(itab) {}
  
  //: Called by the process when the data is ready
  virtual bool notify(const bprod_storage_sptr& data, unsigned long timestamp)
  {
    assert(image_tab);
    assert(data);
    if(data->info() != BPROD_VALID)
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


#endif // bprod_basic_gui_observers_h_
