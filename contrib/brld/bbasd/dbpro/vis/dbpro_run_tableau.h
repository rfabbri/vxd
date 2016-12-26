// This is basic/dbpro/vis/dbpro_run_tableau.h
#ifndef dbpro_run_tableau_h_
#define dbpro_run_tableau_h_
//:
// \file
// \brief  Tableau that runs a dbpro process graph in its idle handler
// \author Matthew Leotta (mleotta@brown.lems.edu)
// \date   July 14, 2006
//
// \verbatim
//  Modifications
// \endverbatim


#include <dbpro/vis/dbpro_run_tableau_sptr.h>
#include <dbpro/dbpro_executive.h>
#include <vgui/vgui_tableau.h>
#include <vul/vul_timer.h>


//: Tableau that runs a dbpro process graph in its idle handler
class dbpro_run_tableau: public vgui_tableau
{
  public:
    //: Constructor
    dbpro_run_tableau(const dbpro_executive& g);

    //: Enable idle events (start the processing)
    void enable_idle();

    //: Enable idle events for one iteration
    void enable_idle_once();

    //: Run one step of the processing
    bool run_step();

    //: Toggle the text display
    void toggle_display();

    //: Builds a popup menu
    void get_popup(const vgui_popup_params& params,
                   vgui_menu &menu);

    //: Handle events
    bool handle(const vgui_event& e);

    //: Handle keypress
    bool key_press (int x, int y, vgui_key, vgui_modifier);

    //: Handle idle events
    bool idle();

  private:
    dbpro_executive graph_;
    vul_timer time_;
    double fps_;
    unsigned int count_;
    bool display_;
    bool running_;
};


//: Creates a smart-pointer to a dbpro_run_tableau tableau.
struct dbpro_run_tableau_new : public dbpro_run_tableau_sptr
{
  typedef dbpro_run_tableau_sptr base;

  //: Constructor
  dbpro_run_tableau_new(const dbpro_executive& g)
    : base(new dbpro_run_tableau(g)) {}
};

#endif // dbpro_run_tableau_h_
