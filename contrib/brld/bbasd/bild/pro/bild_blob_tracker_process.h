// This is basic/bild/pro/bild_blob_tracker_process.h
#ifndef bild_blob_tracker_process_h_
#define bild_blob_tracker_process_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief  A process for tracking blobs in binary images
//
// \author Based on original code by  Matt Leotta
// \date 02/21/06
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------

#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>

class bild_blob_tracker_process : public bpro1_process
{
public:

  bild_blob_tracker_process();
  virtual ~bild_blob_tracker_process();

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


#endif // bild_blob_tracker_process_h_
