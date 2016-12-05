// This is vidpro1_smoothcem_process_h_
#ifndef vidpro1_smoothcem_process_h_
#define vidpro1_smoothcem_process_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief Process that takes a vsol2D storage class as input and outputs a vsol2D  storage class.
//        Edges are smoothed by the specified sigma.
// \author  Ozge C Ozcanli (ozge@lems.brown.edu)
// \date 07/10/04
//
// \verbatim
//  Modifications
// \endverbatim
//--------------------------------------------------------------------------------
#include <bpro1/bpro1_process.h>
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>

#include <vcl_vector.h>

class vidpro1_smoothcem_process : public bpro1_process
{
public:
  vidpro1_smoothcem_process();
  ~vidpro1_smoothcem_process() {}

  vcl_string name() {
    return "Smooth Edges";
  }

  //: Clone the process
  virtual bpro1_process* clone() const;

  vcl_vector<vcl_string> get_input_type();
  vcl_vector<vcl_string> get_output_type();

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

protected:
};

#endif // vidpro1_smoothcem_process_h_
