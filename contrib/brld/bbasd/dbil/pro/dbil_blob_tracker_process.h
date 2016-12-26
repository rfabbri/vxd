// This is basic/dbil/pro/dbil_blob_tracker_process.h
#ifndef dbil_blob_tracker_process_h_
#define dbil_blob_tracker_process_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief  A process for tracking blobs in binary images
//
// \author Matt Leotta
// \date 02/21/06
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------

#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>

class dbil_blob_tracker_process : public bpro1_process
{
public:

  dbil_blob_tracker_process();
  virtual ~dbil_blob_tracker_process();

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


#endif // dbil_blob_tracker_process_h_
