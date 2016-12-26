// This is basic/dbil/vis/dbil_image_intensity_inspector.h
#ifndef  dbil_image_intensity_inspector_h_
#define  dbil_image_intensity_inspector_h_
//:
// \file
// \brief tool to inspect the RGB,IHS values of images
// \author unknown
// \date 03/01/2005
//
// \verbatim
//  Modifications
//    Amir Tamrakar    Ported from dbvis1 to dbil_vis
// \endverbatim

#include "dbil_image_tool.h"
#include <vil/vil_image_view.h>

//: tool to inspect the RGB,IHS values of images
class  dbil_image_intensity_inspector : public dbil_image_tool
{
 public:
  //: Constructor
  dbil_image_intensity_inspector();
  //: Destructor
  virtual ~dbil_image_intensity_inspector(){}

  //: Returns the string name of this tool
  virtual vcl_string name() const{return "Image Inspector";}

  virtual bool set_storage ( const bpro1_storage_sptr& storage);
  
  //: handle events
  virtual bool handle( const vgui_event & e, 
                       const bvis1_view_tableau_sptr& selector );
  
  void get_popup( const vgui_popup_params& params, vgui_menu &menu );
  
protected:

  vil_image_view<double> dimage_;
  bool rgb_;
  bool ihs_;
  bool raw_;
};

#endif //dbil_image_intensity_inspector_h_
