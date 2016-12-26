// This is basic/bild/vis/bild_image_intensity_inspector.h
#ifndef  bild_image_intensity_inspector_h_
#define  bild_image_intensity_inspector_h_
//:
// \file
// \brief tool to inspect the RGB,IHS values of images
// \author Based on original code by  unknown
// \date 03/01/2005
//
// \verbatim
//  Modifications
//    Amir Tamrakar    Ported from dbvis1 to bild_vis
// \endverbatim

#include "bild_image_tool.h"
#include <vil/vil_image_view.h>

//: tool to inspect the RGB,IHS values of images
class  bild_image_intensity_inspector : public bild_image_tool
{
 public:
  //: Constructor
  bild_image_intensity_inspector();
  //: Destructor
  virtual ~bild_image_intensity_inspector(){}

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

#endif //bild_image_intensity_inspector_h_
