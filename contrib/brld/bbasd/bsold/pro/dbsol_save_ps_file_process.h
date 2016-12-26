// This is basic/dbsol/pro/dbsol_save_ps_file_process.h
#ifndef dbsol_save_ps_file_process_h_
#define dbsol_save_ps_file_process_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief  A process to write an image and vsol objects to a PS file
//
// \author Nhon Trinh (ntrinh@lems.brown.edu)
// \date Mar 8, 2008
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------

#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>

class dbsol_save_ps_file_process : public bpro1_process 
{
public:

  dbsol_save_ps_file_process();
  virtual ~dbsol_save_ps_file_process();

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
};


#endif // dbsol_save_ps_file_process_h_
