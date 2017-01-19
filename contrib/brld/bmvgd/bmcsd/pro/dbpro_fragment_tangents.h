// This is bprod_fragment_tangents.h
#ifndef bprod_fragment_tangents_h
#define bprod_fragment_tangents_h
//:
//\file
//\brief Process to compute tangents at each point of a curve fragment
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 08/31/2009 10:38:47 PM PDT
//

#include <bprod/bprod_process.h>
#include <bmcsd/algo/bmcsd_algo_util.h>

class bprod_fragment_tangents : public bprod_filter {
public:
  //: Assumes input pin 0 has a a vcl_vector< vsol_polyline_2d_sptr > curves
  bprod_signal execute() {
    vcl_cout << "Started tgt computation.\n";

    vcl_vector<vcl_vector<double> > tangents;

    assert(input_type_id(0) == typeid(vcl_vector<vsol_polyline_2d_sptr>));
    
    const vcl_vector<vsol_polyline_2d_sptr> &crvs =
      input<vcl_vector<vsol_polyline_2d_sptr> >(0);

    tangents.resize(crvs.size());
    for (unsigned c=0; c < crvs.size(); ++c) {
      dbdet_edgel_chain ec;
      bmcsd_algo_util::extract_edgel_chain(*crvs[c], &ec);

      tangents[c].resize(ec.edgels.size());
      for (unsigned i=0; i < ec.edgels.size(); ++i) {
        tangents[c][i] = ec.edgels[i]->tangent;
        delete ec.edgels[i];
      }
    }

    output(0, tangents);
    vcl_cout << "Finished tgt computation.\n";
    return BPROD_VALID;
  }
};

#endif // bprod_fragment_tangents_h

