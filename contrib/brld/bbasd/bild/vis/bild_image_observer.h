// This is basic/bild/vis/bild_image_observer.h
#ifndef bild_image_observer_h_
#define bild_image_observer_h_
//:
// \file
// \brief A bprod image observer for vgui 
// \author Based on original code by  Matt Leotta
// \date 01/21/2008
//
// \verbatim
//  Modifications
// \endverbatim


#include <bprod/bprod_observer.h>

#include <vil/vil_image_resource.h>
#include <vgui/vgui_image_tableau.h>


class bild_image_observer: public bprod_observer
{
  public:
    bild_image_observer(const vgui_image_tableau_sptr& itab)
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

#endif //bild_image_observer
