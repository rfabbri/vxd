// This is brl/bbas/bgui/bsold2D_tableau.h
#ifndef bsold2D_tableau_h_
#define bsold2D_tableau_h_
//-----------------------------------------------------------------------------
//:
// \file
// \brief A child tableau of bgui_vsol2D_tableau to display a circular arc vsol object.
// \author Based on original code by 
//  Isa Restrepo
//
//   Default styles are defined for each geometry object soview.
//   Users can change the default style by using the set_*_style commands,
//
// \verbatim
//  Modifications:
// \endverbatim
//-----------------------------------------------------------------------------
#include <vcl_vector.h>

#include <vsol/vsol_spatial_object_2d_sptr.h>
#include <vgui/vgui_style_sptr.h>
#include <bgui/bgui_vsol2D_tableau.h>
#include <bsold/bsold_circ_arc_2d_sptr.h>
#include <bsold/vis/bsold_soview2D_circ_arc.h>
#include <bsold/vis/bsold2D_tableau_sptr.h>


class bsold2D_tableau : public bgui_vsol2D_tableau
{
 public:  
  bsold2D_tableau(const char* n="unnamed");

  bsold2D_tableau(vgui_image_tableau_sptr const& it,
                             const char* n="unnamed");

  bsold2D_tableau(vgui_tableau_sptr const& t,
                             const char* n="unnamed");

  ~bsold2D_tableau(){}

  //: Returns the type of this tableau ('bsold2D_tableau').
  vcl_string type_name() const { return "bsold2D_tableau";}

 
  ////: display for bsold_circ_arc_2d
  bsold_soview2D_circ_arc*
    add_bsold_circ_arc_2d(bsold_circ_arc_2d_sptr const& circ_arc,
                      const vgui_style_sptr& style = NULL);

  void add_spatial_object(vsol_spatial_object_2d_sptr const& sos,
                          const vgui_style_sptr& style = NULL);

  //: display methods for vectors of vsol classes (not grouped)
  void add_spatial_objects(vcl_vector<vsol_spatial_object_2d_sptr> const& sos,
                           const vgui_style_sptr& style = NULL);


  //: Methods for changing the default style of displayable objects
  void set_vsol_spatial_object_2d_style(vsol_spatial_object_2d_sptr sos,
                                        const vgui_style_sptr& style);

  void set_bsold_circ_arc_2d_style(const vgui_style_sptr& style);
 

 protected:

  //: Default styles
  vgui_style_sptr circ_arc_style_;

  void init();

};

////this stuff is needed to establish inheritance between tableau  smart pointers
////cloned from xcv_image_tableau
//
struct bsold2D_tableau_new : public bsold2D_tableau_sptr
{
  typedef bsold2D_tableau_sptr base;

  bsold2D_tableau_new(const char* n="unnamed") :
    base(new bsold2D_tableau(n)) { }
  bsold2D_tableau_new(vgui_image_tableau_sptr const& it,
                                 const char* n="unnamed") :
    base(new bsold2D_tableau(it,n)) { }

  bsold2D_tableau_new(vgui_tableau_sptr const& t, const char* n="unnamed")
    :  base(new bsold2D_tableau(t, n)) { }

  operator vgui_easy2D_tableau_sptr () const { vgui_easy2D_tableau_sptr tt; tt.vertical_cast(*this); return tt; }
};

#endif // bsold2D_tableau_h_
