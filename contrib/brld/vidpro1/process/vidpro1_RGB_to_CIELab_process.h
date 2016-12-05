// This is brl/vidpro1/process/vidpro1_RGB_to_CIELab_process.h
#ifndef vidpro1_RGB_to_CIELab_process_h_
#define vidpro1_RGB_to_CIELab_process_h_

//:
// \file
// \brief Process that converts an RGB image to a CIE-Lab image
// \author Amir Tamrakar
// \date 01/29/07
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <vcl_vector.h>
#include <vcl_string.h>
#include <vil/vil_image_view.h>

//: Process that converts an RGB image to a CIE-Lab image
class vidpro1_RGB_to_CIELab_process : public bpro1_process {

public:
  //: Constructor
  vidpro1_RGB_to_CIELab_process();
  //: Destructor
  virtual ~vidpro1_RGB_to_CIELab_process();

  //: Clone the process
  virtual bpro1_process* clone() const;

  //: Returns the name of this process
  vcl_string name();

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  int input_frames();
  int output_frames();

  bool execute();
  bool finish();

  //functions for converting an RGB image to a CIE-Lab image
  void rgb2lab(double R,double G, double B, double &L, double &a, double &b);
  void convert_to_Lab(vil_image_view<unsigned char> const& image,
                      vil_image_view<float>& L_img,
                      vil_image_view<float>& a_img,
                      vil_image_view<float>& b_img);
};

#endif
