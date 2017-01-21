// This is bmcsd/pro/bmcsd_load_discrete_corresp_process_h_
#ifndef bmcsd_load_discrete_corresp_process_h_
#define bmcsd_load_discrete_corresp_process_h_

//:
//\file
//\brief Process to load correspondence files into storage
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri@gmail.com)
//\date Mon Apr 24 15:04:41 EDT 2006
//

#include <bpro1/bpro1_process.h>
#include <bpro1/bpro1_parameters.h>
#include <vcl_vector.h>


//: This process loads bmcsd_discrete_corresp binary file into a storage class
class bmcsd_load_discrete_corresp_process : public bpro1_process
{
public:
  bmcsd_load_discrete_corresp_process();
  ~bmcsd_load_discrete_corresp_process() {}

  //: Clone the process
  virtual bpro1_process* clone() const;
  
  vcl_string name() {
    return "Load correspondences";
  }
  
  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();
  
  int input_frames() {
    return 1;
  }
  int output_frames() {
    return 1;
  }
  
  bool execute();
  bool finish() {
    return true;
  }
};

#endif // bmcsd_load_discrete_corresp_process_h_
