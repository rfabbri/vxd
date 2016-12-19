// This is breye1/bvis1/mapper/bvis1_mapper.cxx

#include <bvis1/bvis1_mapper.h>
#include <vgui/vgui_style.h>
#include <vcl_cmath.h>

//: A simple generic mapping function using green
vgui_style_sptr
bvis1_mapper::style(const double attr) const
{
  if(attr<mina_)
    return vgui_style::new_style(0,0,0, 3.0, 3.0);
  if(attr>maxa_)
    return vgui_style::new_style(0,1.0,0, 3.0, 3.0);
  //else scan between min and max values
  double range = maxa_-mina_;
  if(range == 0)
    return vgui_style::new_style(0,1.0,0, 3.0, 3.0);
  double v = (attr-mina_)/range;
  if(gamma_>0&&gamma_!=1)
    v = vcl_pow(v, 1/gamma_);
  return vgui_style::new_style(0,static_cast<float>(v),0, 3.0, 3.0);
}
