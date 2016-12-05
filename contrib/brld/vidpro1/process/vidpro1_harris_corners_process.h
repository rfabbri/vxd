// This is brl/vidpro1/process/vidpro1_harris_corners_process.h
#ifndef vidpro1_harris_corners_process_h_
#define vidpro1_harris_corners_process_h_

//:
// \file
// \brief A process that computes Harris Corners
// \author Mark Johnson (mrj@lems.brown.edu)
// \date 7/21/03
//
// \verbatim
//  Modifications
//   Matt Leotta  1/26/04    Updated comments in Doxygen form
// \endverbatim


#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>

#include <bpro1/bpro1_parameters.h>


class vidpro1_harris_corners_process : public bpro1_process {

public:

  vidpro1_harris_corners_process();
  ~vidpro1_harris_corners_process();

  //: Clone the process
  virtual bpro1_process* clone() const;

  vcl_string name();

  int input_frames();
  int output_frames();
  
  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  bool execute();
  bool finish();

protected:

private:

};

#endif // vidpro1_harris_corners_process_h_
