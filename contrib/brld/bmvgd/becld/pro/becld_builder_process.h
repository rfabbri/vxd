// This is bmvgd/becld/pro/becld_builder_process.h
#ifndef becld_builder_process_h_
#define becld_builder_process_h_

//:
// \file
// \brief A process that converts vtol edges to becld episegs
// \author Based on original code by  Matt Leotta (@lems.brown.edu)
// \date 11/8/04
//
// \verbatim
//  Modifications
// \endverbatim


#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>

#include <becld/becld_episeg.h>


class becld_builder_process : public bpro1_process
{
public:

  becld_builder_process();
  ~becld_builder_process();

  becld_builder_process(const becld_builder_process& other);

  //: Clone the process
  virtual bpro1_process* clone() const;
  
  vcl_string name();

  int input_frames();
  int output_frames();
  
  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  bool execute();
  bool finish();

};

#endif // becld_builder_process_h_
