// This is basic/bprod/vis/bprod_run_tableau.h
#ifndef bprod_run_tableau_h_
#define bprod_run_tableau_h_
//:
// \file
// \brief  Tableau that runs a bprod process graph in its idle handler
// \author Matthew Leotta (mleotta@brown.lems.edu)
// \date   July 14, 2006
//
// \verbatim
//  Modifications
// \endverbatim


#include <bprod/vis/bprod_run_tableau_sptr.h>
#include <bprod/bprod_executive.h>
#include <vgui/vgui_tableau.h>
#include <vul/vul_timer.h>


//: Tableau that runs a bprod process graph in its idle handler
class bprod_run_tableau: public vgui_tableau
{
  public:
    //: Constructor
    bprod_run_tableau(const bprod_executive& g);

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
    bprod_executive graph_;
    vul_timer time_;
    double fps_;
    unsigned int count_;
    bool display_;
    bool running_;
};


//: Creates a smart-pointer to a bprod_run_tableau tableau.
struct bprod_run_tableau_new : public bprod_run_tableau_sptr
{
  typedef bprod_run_tableau_sptr base;

  //: Constructor
  bprod_run_tableau_new(const bprod_executive& g)
    : base(new bprod_run_tableau(g)) {}
};

#endif // bprod_run_tableau_h_
