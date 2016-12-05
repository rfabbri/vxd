// This is brl/vidpro1/process/vidpro1_load_video_process.h

#ifndef vidpro1_load_video_process_h_
#define vidpro1_load_video_process_h_

//:
// \file
// \brief A process for loading a video into a series of frames
// \author Matt Leotta
// \date 1/27/04
//
// \verbatim
//  Modifications
//  July-06-2005 Ozge Can Ozcanli added loading option from start frame to end frame
//  March-28-2006 Ozge Can Ozcanli added loading option using direct show if installed in once computer
//                                 Note: this can show anything your Windows Media Player can show.
//                                 For .m2t files one needs K-Lite Full codec bundle (note: 2.71 full version, 
//                                 use MainConcept options instead of Ligos during setup)
//                                 If the file has more 200 frames, clip while loading, otherwise takes 
//                                 forever to load, bvis1_manager should be updated to handle vidl instances
//                                 for fast and adequate access
// Oct-17-2009  Nhon Trinh    Modified loading a folder of images using the new vidl library, which
//                             was promoted to core/vidl from brl/bbas/vidl2. The old vidl1 library
//                             that was used to load video/image list is no longer maintained on vxl.
// \endverbatim


#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>

class vidpro1_load_video_process : public bpro1_process
{
 public:
  
  vidpro1_load_video_process();
 ~vidpro1_load_video_process();

  //: Clone the process
  virtual bpro1_process* clone() const;

  vcl_string name();

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  virtual void clear_output(int resize = -1);

  int input_frames();
  int output_frames();
  
  bool execute();
  bool finish();
 
  int num_frames() {return num_frames_;}
 protected:

 int num_frames_;
 
};

#endif // vidpro1_load_video_process
