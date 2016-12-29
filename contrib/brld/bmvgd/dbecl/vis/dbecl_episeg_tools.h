// This is brcv/mvg/dbecl/vis/dbecl_episeg_tools.h
#ifndef dbecl_episeg_tools_h_
#define dbecl_episeg_tools_h_
//:
// \file
// \brief Tools that work on dbecl_episegs
// \author Matt Leotta, (mleotta@lems.brown.edu)
// \date 11/10/04
//
// \verbatim
//  Modifications
// \endverbatim

#include <bvis1/bvis1_tool.h>
#include <vgui/vgui_displaylist2D_tableau_sptr.h>
#include <dbecl/pro/dbecl_episeg_storage_sptr.h>
#include <dbecl/dbecl_epipole_sptr.h>
#include <dbecl/dbecl_episeg_sptr.h>

//: forward declaration
class dbecl_episeg_soview2D;


//: An abstract base class for tools in this file
class dbecl_episeg_tool : public bvis1_tool
{
public:
  //: Set the tableau to work with
  virtual bool set_tableau ( const vgui_tableau_sptr& tableau );

  //: Set the storage class for the active tableau
  virtual bool set_storage ( const bpro1_storage_sptr& storage);
  
protected:
  dbecl_episeg_tool();
  
  vgui_displaylist2D_tableau_sptr tableau_;
  dbecl_episeg_storage_sptr storage_;
};

  
//: A tool for inspecting episegs
class dbecl_episeg_inspector_tool : public dbecl_episeg_tool
{
public:
  dbecl_episeg_inspector_tool();
  virtual ~dbecl_episeg_inspector_tool();

  //: Return the name of this tool
  virtual vcl_string name() const;

  //: Handle events
  virtual bool handle( const vgui_event & e, 
                       const bvis1_view_tableau_sptr& view );

  //: Add popup menu items
  virtual void get_popup(const vgui_popup_params& params, vgui_menu &menu);
  
protected:
  //: Print stats about the episeg
  void print_stats(const dbecl_episeg_sptr episeg) const;

  bool draw_bounds_;
  bool draw_neighbors_;
  bool draw_epipolar_line_;
  bool print_stats_;
  dbecl_epipole_sptr epipole_;
  
private:
  dbecl_episeg_soview2D* object_;
  float last_x_, last_y_;

};


#endif // dbecl_episeg_tools_h_
