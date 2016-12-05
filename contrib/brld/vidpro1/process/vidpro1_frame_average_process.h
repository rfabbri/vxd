// This is breye1/vidpro1/process/vidpro1_frame_average_process.h
#ifndef vidpro1_frame_average_process_h_
#define vidpro1_frame_average_process_h_

//:
// \file
// \brief A process that averages (co-addition) a sequence of frames
// \author J. L. Mundy 
// \date June 30, 2005
//
// \verbatim
//  Modifications
// \endverbatim


#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>
#include <vil/vil_image_view.h>

class vidpro1_frame_average_process : public bpro1_process {

public:

  vidpro1_frame_average_process();
  virtual ~vidpro1_frame_average_process();

  //: Clone the process
  virtual bpro1_process* clone() const;

  vcl_string name();

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  int input_frames();
  int output_frames();

  bool execute();
  bool finish();
  
protected:
  void store_output(vil_image_view<float> const& result);
  bool first_frame_;
  vil_image_view<float> average_;
  unsigned frame_count_;
private:

};

#endif // vidpro1_frame_average_process_h_
