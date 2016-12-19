// This is bvis1/bvis1_view_tableau.h
#ifndef bvis1_view_tableau_h_
#define bvis1_view_tableau_h_
//:
// \file
// \brief  Tableau at the root of each view of the data in bvis
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 10/13/04
//
//  Contains classes: bvis1_view_tableau  bvis1_view_tableau_new


#include "bvis1_view_tableau_sptr.h"
#include <vgui/vgui_tableau.h>
#include <vgui/vgui_parent_child_link.h>
#include <vgui/vgui_selector_tableau_sptr.h>


class bvis1_view_tableau : public vgui_tableau
{
 public:
  //: Constructor - don't use this, use bvis1_view_tableau_new.
  bvis1_view_tableau();

  //: Constructor - don't use this, use bvis1_view_tableau_new.
  bvis1_view_tableau(const vgui_selector_tableau_sptr selector,
                    int offset = 0, bool absolute = false);

  //: Destructor
  ~bvis1_view_tableau();

  //: Returns the type of tableau ('bvis1_view_tableau').
  vcl_string type_name() const;

  //: Handle all events sent to this tableau.
  bool handle(vgui_event const &);

  //: Generate the popup menu for this tableau
  void get_popup(const vgui_popup_params& params, vgui_menu &menu);

  //: Access the selector tableau
  vgui_selector_tableau_sptr selector() const;

  //: Access the offset amount
  int offset() const { return offset_; }

  //: Return true if this view is absolute
  bool is_absolute() const { return absolute_; }

  //: Access the current frame to draw
  int frame() const;

 protected:
  //: The child of this tableau is guaranteed to be a bgui_selector_tableau
  vgui_parent_child_link child_;
  //: The frame offset about (from the global frame value)
  int offset_;
  //: is the offset absolute (always relative to frame 0)
  bool absolute_;
};

//: Create a smart-pointer to a bvis1_view_tableau.
struct bvis1_view_tableau_new : public bvis1_view_tableau_sptr {
  typedef bvis1_view_tableau_sptr base;

  //: Constructor - creates a default bvis1_view_tableau.
  bvis1_view_tableau_new() : base(new bvis1_view_tableau()) { }

  //: Constructor - creates a bvis1_view_tableau.
  bvis1_view_tableau_new(const vgui_selector_tableau_sptr selector,
                        int offset = 0, bool absolute = false) 
   : base(new bvis1_view_tableau(selector, offset, absolute)) { }
};

#endif // bvis1_view_tableau_h_
