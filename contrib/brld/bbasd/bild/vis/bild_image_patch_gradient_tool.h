// This is basic/dbil/vis/dbil_image_patch_gradient_tool.h
#ifndef dbil_image_patch_gradient_tool_h_
#define dbil_image_patch_gradient_tool_h_
//:
// \file
// \brief A tool for looking at image gradients in a patch
// \author Amir Tamrakar
// \date 03/01/2005
//
// \verbatim
//  Modifications
// \endverbatim

#include "dbil_image_tool.h"

//: A tool for looking at image gradients in a patch
class dbil_image_patch_gradient_tool : public dbil_image_tool 
{
public:

  dbil_image_patch_gradient_tool();
  virtual ~dbil_image_patch_gradient_tool();

  bool handle( const vgui_event & e, 
               const bvis1_view_tableau_sptr& view );

  vcl_string name() const;
  
protected:

  vgui_event_condition left_click;
  
private:

};

#endif //dbil_image_patch_gradient_tool_h_
