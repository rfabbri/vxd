// This is basic/bild/vis/bild_image_patch_gradient_tool.h
#ifndef bild_image_patch_gradient_tool_h_
#define bild_image_patch_gradient_tool_h_
//:
// \file
// \brief A tool for looking at image gradients in a patch
// \author Based on original code by  Amir Tamrakar
// \date 03/01/2005
//
// \verbatim
//  Modifications
// \endverbatim

#include "bild_image_tool.h"

//: A tool for looking at image gradients in a patch
class bild_image_patch_gradient_tool : public bild_image_tool 
{
public:

  bild_image_patch_gradient_tool();
  virtual ~bild_image_patch_gradient_tool();

  bool handle( const vgui_event & e, 
               const bvis1_view_tableau_sptr& view );

  vcl_string name() const;
  
protected:

  vgui_event_condition left_click;
  
private:

};

#endif //bild_image_patch_gradient_tool_h_
