#include "vcpld_bundle_adjust_sink.h"

void vcpld_bundle_adjust_sink::
setup_inputs(
      vcl_vector<bprod_process_sptr> &cam_src, 
      vcl_vector<bprod_process_sptr> &edg_src, 
      vcl_vector<bprod_process_sptr> &edg_dt
      )
{
  nviews_ = cam_src.size();
  assert(cam_src.size() == edg_src.size());
  assert(cam_src.size() == edg_dt.size());

  inputs_per_view_ = 4;

  for (unsigned i=0; i < cam_src.size(); ++i) {
    unsigned offset = i*inputs_per_view_;
    connect_input(offset + CAM_ID,  cam_src[i], 0);
    connect_input(offset + EDG_ID,  edg_src[i], 0);
    connect_input(offset + DT_ID,   edg_dt[i], 0);
    connect_input(offset + LBL_ID,  edg_dt[i], 1);
  }
}

void vcpld_bundle_adjust_sink::
get_cameras()
{
  cam_.resize(nviews());

  for (unsigned i=0; i < nviews(); ++i) {
    unsigned offset = i*inputs_per_view_;
    assert(input_type_id(offset + CAM_ID) 
        == typeid(vpgl_perspective_camera<double>));

    cam_[i] = input<vpgl_perspective_camera<double> >(offset + CAM_ID);
  }
}

void vcpld_bundle_adjust_sink::
get_edgemaps()
{
  em_.resize(nviews());
  
  for (unsigned i=0; i < nviews(); ++i) {
    unsigned offset = i*inputs_per_view_;
    assert( input_type_id(offset + EDG_ID) == typeid(sdet_edgemap_sptr) );

    em_[i] = input< sdet_edgemap_sptr >(offset + EDG_ID);
  }
}

void vcpld_bundle_adjust_sink::
get_dt_label()
{
  dt_.resize(nviews());
  label_.resize(nviews());

  // dts[0] == null;
  // labels [0] == null;
  for (unsigned i=0; i < nviews(); ++i) {
    unsigned offset = i*inputs_per_view_;
    assert(input_type_id(offset + DT_ID) == typeid( dt_t ));
    assert(input_type_id(offset + LBL_ID) == typeid( label_t ));

    dt_[i] = input< dt_t >(offset + DT_ID);
    label_[i] = input< label_t >(offset + LBL_ID);
  }
}
