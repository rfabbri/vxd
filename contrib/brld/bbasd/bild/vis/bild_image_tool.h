// This is basic/dbil/vis/dbil_image_tool.h
#ifndef dbil_image_tool_h_
#define dbil_image_tool_h_
//:
// \file
// \brief Tools that work on images 
// \author Amir Tamrakar
// \date 03/01/2005
//
// \verbatim
//  Modifications
// \endverbatim

#include <bvis1/bvis1_tool.h>
#include <vgui/vgui_event_condition.h>
#include <vgui/vgui_projection_inspector.h>

#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>

#include <vgui/vgui_image_tableau.h>
#include <vgui/vgui_image_tableau_sptr.h>

class dbil_image_tool : public bvis1_tool
{
public:
  dbil_image_tool();
  virtual ~dbil_image_tool();

  virtual bool set_tableau ( const vgui_tableau_sptr& tableau );
  virtual bool set_storage ( const bpro1_storage_sptr& storage);

  virtual vcl_string name() const = 0;

protected:

  vgui_image_tableau_sptr tableau();
  vidpro1_image_storage_sptr storage();

  vgui_image_tableau_sptr tableau_;
  vidpro1_image_storage_sptr storage_;

};

#endif //dbil_image_tool_h_
