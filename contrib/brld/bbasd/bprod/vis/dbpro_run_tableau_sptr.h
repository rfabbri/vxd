// This is basic/dbpro/vis/dbpro_run_tableau_sptr.h
#ifndef dbpro_run_tableau_sptr_h_
#define dbpro_run_tableau_sptr_h_
//:
// \file
// \author  Matthew Leotta (mleotta@lems.brown.edu)
// \brief   Smart-pointer to a dbpro_run_tableau tableau.

#include <vgui/vgui_tableau_sptr.h>

class dbpro_run_tableau;
typedef vgui_tableau_sptr_t<dbpro_run_tableau> dbpro_run_tableau_sptr;

#endif // dbpro_run_tableau_sptr_h_
