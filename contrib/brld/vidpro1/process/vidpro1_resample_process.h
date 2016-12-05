// This is breye1/vidpro1/process/vidpro1_resample_process.h
#ifndef vidpro1_resample_process_h_
#define vidpro1_resample_process_h_

//:
// \file
// \brief A process that resamples an image
// \author Matt Leotta
// \date 8/26/05
//
// \verbatim
// \endverbatim


#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>


class vidpro1_resample_process : public bpro1_process {

public:

  vidpro1_resample_process();
  virtual ~vidpro1_resample_process();

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

private:

};

#endif // vidpro1_resample_process_h_
