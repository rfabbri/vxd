// This is brcv/mvg/dbecl/pro/dbecl_builder_process.h
#ifndef dbecl_builder_process_h_
#define dbecl_builder_process_h_

//:
// \file
// \brief A process that converts vtol edges to dbecl episegs
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 11/8/04
//
// \verbatim
//  Modifications
// \endverbatim


#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>

#include <dbecl/dbecl_episeg.h>


class dbecl_builder_process : public bpro1_process
{
public:

  dbecl_builder_process();
  ~dbecl_builder_process();

  dbecl_builder_process(const dbecl_builder_process& other);

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

#endif // dbecl_builder_process_h_
