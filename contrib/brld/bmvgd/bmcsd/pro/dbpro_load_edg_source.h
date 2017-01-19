// This is bprod_load_edg_source.h
#ifndef bprod_load_edg_source_h
#define bprod_load_edg_source_h
//:
//\file
//\brief bprod source for loading edgemaps
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri@gmail.com)
//\date 08/28/2009 03:54:41 PM PDT
//

#include <dbdet/algo/dbdet_load_edg.h>
#include <dbdet/algo/dbdet_convert_edgemap.h>
#include <bprod/bprod_process.h>
#include <bild/algo/bild_exact_distance_transform.h>


//: Reads .EDG files into edgemaps.
//
// \todo support for converting edges into vsol. Currently we only output
// dbdet_edgemaps.
class bprod_load_edg_source : public bprod_source {
public:
  bprod_load_edg_source(vcl_string fname, bool bSubPixel, double scale) 
    : fname_(fname),
      bSubPixel_(bSubPixel),
      scale_(scale) {};

  bprod_signal execute() {
    dbdet_edgemap_sptr edge_map;

    bool retval = dbdet_load_edg(fname_, bSubPixel_, scale_, edge_map);
    if (!retval)
      return BPROD_INVALID;

    output(0, edge_map);
#ifndef NDEBUG
    vcl_cout << "#edgels: " << edge_map->num_edgels() << "\n";
    vcl_cout << "Finished with edge map loading.\n";
#endif
    return BPROD_VALID;
  }

private:
  vcl_string fname_;
  bool bSubPixel_;
  double scale_;
};

//: This process binarizes subpixel edgemaps and also computes its
// distance transform and label map.
class bprod_edg_dt : public bprod_filter {
public:
  bprod_edg_dt () {}

  //: Assumes input pin 0 has a dbdet_edgemap.
  bprod_signal execute() {
    vil_image_view<vxl_byte > bw_image;

    assert(input_type_id(0) == typeid(dbdet_edgemap_sptr));
    //: Assumes the conversion maps edges to 255 and others to 0.
    bool retval = dbdet_convert_edgemap_to_image(*(input<dbdet_edgemap_sptr>(0)), bw_image);
    
    if (!retval)
      return BPROD_INVALID;

    vil_image_view<vxl_uint_32> dt(bw_image.ni(), bw_image.nj(), 1);

    for(unsigned i=0; i<dt.ni(); i++)
      for(unsigned j=0; j<dt.nj(); j++)
        dt(i,j) = static_cast<vxl_uint_32>(bw_image(i,j)<127);

    vil_image_view<unsigned> imlabel(dt.ni(), dt.nj(), 1);

    retval = bild_exact_distance_transform_maurer_label(dt, imlabel);
    if (!retval)
      return BPROD_INVALID;

    output(0, dt);
    output(1, imlabel);
    return BPROD_VALID;
  }
};

#endif // bprod_load_edg_source_h

