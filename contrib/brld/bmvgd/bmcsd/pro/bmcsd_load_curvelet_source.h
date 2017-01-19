// This is bmcsd_load_curvelet_source.h
#ifndef bmcsd_load_curvelet_source_h
#define bmcsd_load_curvelet_source_h
//:
//\file
//\brief 
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 07/24/10 15:53:48 EDT
//

#include <bprod/bprod_process.h>
#include <dbdet/pro/dbdet_sel_storage.h>
#include <dbdet/pro/dbdet_sel_storage_sptr.h>
#include <dbdet/edge/dbdet_edgemap.h>
#include <dbdet/edge/dbdet_edgemap_sptr.h>
#include <dbdet/sel/dbdet_curvelet_map.h>
#include <dbdet/algo/dbdet_cvlet_map_io.h>


//: Reads .cvlet files into curvelet maps
class bmcsd_load_curvelet_source : public bprod_source {
public:
  bmcsd_load_curvelet_source(vcl_string fname)
    : fname_(fname)
      {}

  bprod_signal execute() {
#ifndef NDEBUG
    vcl_cout << "Started curvelet loading.\n";
#endif
    dbdet_edgemap_sptr edge_map;

    dbdet_sel_storage_sptr output_sel = dbdet_sel_storage_new();

    bool retval = dbdet_load_cvlet_map(fname_, edge_map, output_sel->CM());
    if (!retval)
      return BPROD_INVALID;

    output_sel->set_EM(edge_map);

    output(0, output_sel);
#ifndef NDEBUG
    vcl_cout << "Finished curvelet loading.\n";
#endif
    return BPROD_VALID;
  }

private:
  vcl_string fname_;
};

#endif // bmcsd_load_curvelet_source_h
