// This is basic/dbil/vis/dbil_image_observer.h
#ifndef dbil_image_observer_h_
#define dbil_image_observer_h_
//:
// \file
// \brief A dbpro image observer for vgui 
// \author Matt Leotta
// \date 01/21/2008
//
// \verbatim
//  Modifications
// \endverbatim


#include <dbpro/dbpro_observer.h>

#include <vil/vil_image_resource.h>
#include <vgui/vgui_image_tableau.h>


class dbil_image_observer: public dbpro_observer
{
  public:
    dbil_image_observer(const vgui_image_tableau_sptr& itab)
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

#endif //dbil_image_observer
