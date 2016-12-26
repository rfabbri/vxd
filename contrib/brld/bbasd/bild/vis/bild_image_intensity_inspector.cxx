// This is basic/dbil/vis/dbil_image_intensity_inspector.cxx
//:
// \file

#include "dbil_image_intensity_inspector.h"

#include <vgui/vgui_projection_inspector.h>
#include <vgui/vgui.h>
#include <vil/vil_image_view.h>
#include <vil/vil_image_resource.h>
#include <bvis1/bvis1_manager.h>
#include <vil/vil_rgb.h>
#include <vil/vil_convert.h>
#include <brip/brip_vil_float_ops.h>

dbil_image_intensity_inspector::dbil_image_intensity_inspector()
 : dimage_(NULL),
   rgb_(false),
   ihs_(false),
   raw_(true)
{
}


bool 
dbil_image_intensity_inspector::set_storage ( const bpro1_storage_sptr& storage)
{
  if(!dbil_image_tool::set_storage(storage))
    return false;
  
  dimage_ = vil_convert_cast(double(), storage_->get_image()->get_view()); 
  return true;
}


bool dbil_image_intensity_inspector::handle(const vgui_event & e,
                                            const bvis1_view_tableau_sptr& selector )
{
  if(e.type==vgui_MOTION)
  {
    float ix, iy;
    vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);
    unsigned int px = static_cast<unsigned int>(vcl_floor(ix+0.5));
    unsigned int py = static_cast<unsigned int>(vcl_floor(iy+0.5));
    vgui::out<<"("<<px<<","<<py<<")";

    if(px>=0 && px<dimage_.ni() && py>=0 && py<dimage_.nj())
    {
    
      if(rgb_)
      {
        if( dimage_.nplanes() == 3 ) 
        {
          vgui::out<<" RGB("<<dimage_(px,py,0)<<","
                            <<dimage_(px,py,1)<<","
                            <<dimage_(px,py,2)<<")";
        }
        else if ( dimage_.nplanes() == 1 ) 
        {
          vgui::out<<" RGB("<<dimage_(px,py)<<","
                            <<dimage_(px,py)<<","
                            <<dimage_(px,py)<<")";
        }
      }
      if(ihs_)
      {
        float i=0.0, h=0.0, s=0.0;
        if( dimage_.nplanes() == 3 ) 
        {
          vil_rgb<double> rgbpixel(dimage_(px,py,0),dimage_(px,py,1),dimage_(px,py,2));
          brip_vil_float_ops::rgb_to_ihs(rgbpixel,i,h,s);
        }
        else if ( dimage_.nplanes() == 1 ) 
        {
          vil_rgb<double> rgbpixel(dimage_(px,py),dimage_(px,py),dimage_(px,py));
          brip_vil_float_ops::rgb_to_ihs(rgbpixel,i,h,s);
        }
        vgui::out<<" HSV("<<i<<","<<h*360/255<<","<<s/255<<")";
      }
      if(raw_)
      {
        vgui::out<<" raw(";
        for(unsigned np = 0; np<dimage_.nplanes(); ++np){
          if(np>0)
            vgui::out<<",";
          vgui::out<<dimage_(px,py,np);
        }
        vgui::out<<")";
      }
    }
    vgui::out<<"\n";
    return true;
  }
  return false;
}


void dbil_image_intensity_inspector::get_popup( const vgui_popup_params& params, 
                                                 vgui_menu &menu )
{
  vcl_string on = "[x] ", off = "[ ] ";
  menu.add( ((rgb_)?on:off)+"RGB ", bvis1_tool_toggle, (void*)(&rgb_) );
  menu.add( ((ihs_)?on:off)+"IHS", bvis1_tool_toggle, (void*)(&ihs_) );
  menu.add( ((raw_)?on:off)+"Raw", bvis1_tool_toggle, (void*)(&raw_) );
}
