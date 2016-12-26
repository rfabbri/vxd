#include "bsold2D_tableau.h"
//:
// \file
#include <bgui/bgui_vsol_soview2D.h>
#include <bsold/vis/bsold_soview2D_circ_arc.h>
#include <vgui/vgui.h>
#include <vgui/vgui_style.h>
#include <vsol/vsol_spatial_object_2d.h>
#include <bsold/bsold_circ_arc_2d.h>
#include <bsold/bsold_circ_arc_2d_sptr.h>
#include <vcl_cassert.h>
#include <vsol/vsol_curve_2d_sptr.h>

bsold2D_tableau::bsold2D_tableau(const char* n) :
  bgui_vsol2D_tableau(n) {this->init(); }

bsold2D_tableau::bsold2D_tableau(vgui_image_tableau_sptr const& it,
                                         const char* n) :
  bgui_vsol2D_tableau(it, n) {this->init(); }

bsold2D_tableau::bsold2D_tableau(vgui_tableau_sptr const& t,
                                         const char* n) :
  bgui_vsol2D_tableau(t, n) { this->init(); }


void bsold2D_tableau::init()
{
  //define default soview styles
  //these can be overridden by later set_*_syle commands prior to drawing.

  circ_arc_style_ = vgui_style::new_style(0.2f, 0.8f, 0.1f, 1.0f, 3.0f);
}


bsold_soview2D_circ_arc*
bsold2D_tableau::add_bsold_circ_arc_2d(bsold_circ_arc_2d_sptr const& circ_arc,
                                      const vgui_style_sptr& style)
{
  bsold_soview2D_circ_arc* obj =
      new bsold_soview2D_circ_arc(circ_arc);
  add(obj);
  if (style)
    obj->set_style( style );
  else
    obj->set_style( circ_arc_style_ );
  return obj;
}

void bsold2D_tableau::
add_spatial_objects(vcl_vector<vsol_spatial_object_2d_sptr> const& sos,
                    const vgui_style_sptr& style)
{
  for (vcl_vector<vsol_spatial_object_2d_sptr>::const_iterator sit = sos.begin();
       sit != sos.end(); sit++)
  {
    add_spatial_object( (*sit) , style );
  }
}

void bsold2D_tableau::
add_spatial_object(vsol_spatial_object_2d_sptr const& sos,
                   const vgui_style_sptr& style)
{
  if (sos->cast_to_curve())
  {
    if (sos->cast_to_curve()->cast_to_circ_arc()){
      this->add_bsold_circ_arc_2d(sos->cast_to_curve()->cast_to_circ_arc(), circ_arc_style_);
      return;
    }
  }
  
  bgui_vsol2D_tableau::add_spatial_object(sos,style);
  return;
}

void bsold2D_tableau::set_vsol_spatial_object_2d_style(vsol_spatial_object_2d_sptr sos,
                                                           const vgui_style_sptr& style)
{
  if (sos->cast_to_point()) {
    set_vsol_point_2d_style(style);
  } else if (sos->cast_to_curve()) {
    set_digital_curve_style(style);
  }
}

void bsold2D_tableau::set_bsold_circ_arc_2d_style(const vgui_style_sptr& style)
{
  circ_arc_style_->rgba[0] = style->rgba[0];
  circ_arc_style_->rgba[1] = style->rgba[1];
  circ_arc_style_->rgba[2] = style->rgba[2];
  circ_arc_style_->point_size = style->point_size;
  circ_arc_style_->line_width = style->line_width;
}
