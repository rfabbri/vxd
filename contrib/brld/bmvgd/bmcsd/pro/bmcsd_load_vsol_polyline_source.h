// This is bmcsd_load_vsol_polyline_source.h
#ifndef bmcsd_load_vsol_polyline_source_h
#define bmcsd_load_vsol_polyline_source_h
//:
//\file
//\brief Source process associated with binary vsol file
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 08/28/2009 04:12:36 PM PDT
//

#include <vul/vul_file.h>
#include <bsold/bsold_file_io.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>


//: bprod souce process to load vsol storage from a binary file.
//
class bmcsd_load_vsol_polyline_source : public bprod_source {
public:

  bmcsd_load_vsol_polyline_source( vcl_string fname ) 
    : fname_(fname),
      min_samples_(0),
      min_length_(0.0),
      use_length_(true)
  {
    vsl_add_to_binary_loader(vsol_polyline_2d());
  }

  //: Used to prune the curves by enforcing a minimum number of samples.
  void set_min_samples(unsigned m) { min_samples_ = m; use_length_ = false; }

  //: Used to prune the curves by enforcing a minimum number of samples.
  void set_min_length(double m) { min_length_ = m; use_length_ = true; }

  bprod_signal execute() {
    vcl_string ext = vul_file::extension(fname_);
    vcl_vector< vsol_spatial_object_2d_sptr > base;

    if (ext == ".vsl") {
      vsl_b_ifstream bp_in(fname_.c_str());
      if (!bp_in) {
        vcl_cout << " Error opening file  " << fname_ << vcl_endl;
        return BPROD_INVALID;
      }

      vcl_cout << "Opened vsl file " << fname_ <<  " for reading" << vcl_endl;

      vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
      output_vsol->b_read(bp_in);
      bp_in.close();
      base = output_vsol->all_data();
    } else {
      bool retval = bsold_load_cem(base, fname_);
      if (!retval) {
        return BPROD_INVALID;
      }
      vcl_cout << "Opened cemv file " << fname_ <<  " for reading" << vcl_endl;
    }

    vcl_vector< vsol_polyline_2d_sptr > curves;
    curves.reserve(base.size());

    // Cast everything to polyline

    for (unsigned i=0; i < base.size(); ++i) {
      vsol_polyline_2d_sptr
        p = dynamic_cast<vsol_polyline_2d *> (base[i].ptr());

      if (!p) {
        vcl_cerr << "Non-polyline found, but only POLYLINES supported!" << vcl_endl;
        return BPROD_INVALID;
      }

      bool include_curve = (use_length_)? (p->length() > min_length_) : (p->size() > min_samples_);

      if (include_curve)
        curves.push_back(p);
    }

    // The swap trick reduces the excess memory used by curves
    vcl_vector< vsol_polyline_2d_sptr >(curves).swap(curves);
    vcl_cout << "Curves: #curves =  " << curves.size() << vcl_endl;

    output(0, curves);
    return BPROD_VALID;
  }

private:
  vcl_string fname_;
  unsigned min_samples_;
  double min_length_;
  bool use_length_;
};

#endif // bmcsd_load_vsol_polyline_source_h

