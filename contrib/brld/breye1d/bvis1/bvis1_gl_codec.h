// This is breye1/bvis1/bvis1_gl_codec.h
#ifndef bvis1_gl_codec_h
#define bvis1_gl_codec_h
//:
// \file
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 4/27/04
//
// \verbatim
//  Modifications
// \endverbatim

#include <vcl_string.h>
#include <vcl_cstring.h>
#include <bvis1/bvis1_gl_codec_sptr.h>
//#include <vidl1/vidl1_codec.h>
#include <vil/vil_image_resource.h>
#include <vcl_list.h>
#include <vcl_vector.h>

//: This class treats the bvis1_manager as a video and
// produces images by rendering each frame into a buffer
// with OpenGL
class bvis1_gl_codec :  public vidl1_codec
{
  // PUBLIC INTERFACE----------------------------------------------------------
 public:

  // Constructors
  bvis1_gl_codec(int start_frame = -1, int end_frame = -1);
  // Destructor
  ~bvis1_gl_codec();

  // Operators-----------------------------------------------------------------

  virtual vil_image_view_base_sptr get_view(int position, int x0, int w, int y0, int h) const;
  virtual bool put_view(int position, const vil_image_view_base &im, int x0, int y0);

  // I/O

  //: Always returns NULL - this codec is for saving only
  virtual vidl1_codec_sptr load(vcl_string const& fname, char mode = 'r' );
  virtual bool save(vidl1_movie* movie, vcl_string const& fname);
  virtual bool probe(vcl_string const& fname);

  virtual vcl_string type() const {return "bvisManager";}


 protected:
  virtual bool init();

  //: The first frame to save
  int start_;
  //: The last frame to save
  int end_;

};

#endif // bvis1_gl_codec_h
