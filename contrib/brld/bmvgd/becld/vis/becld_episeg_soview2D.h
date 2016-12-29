// This is brcv/mvg/becld/vis/bgui_becld_soview2D.h
#ifndef becld_episeg_soview2D_h_
#define becld_episeg_soview2D_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief vgui_soview2D objects for becld classes
// \author Based on original code by  Matt Leotta (@lems.brown.edu)
// \date 11/8/04
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------

#include <vcl_iosfwd.h>

#include <becld/becld_episeg_sptr.h>
#include <vcl_vector.h>
#include <vgl/vgl_point_2d.h>
#include <vgui/vgui_soview2D.h>

//: This is a soview2D for a becld_episeg with a smart pointer back to the becld_episeg
class becld_episeg_soview2D : public vgui_soview2D
{
 public:
  //: Constructor - creates a default view
  becld_episeg_soview2D( becld_episeg_sptr const & seg);

  //: Destructor
  virtual ~becld_episeg_soview2D();

  //: Render the becld_episeg on the display.
  virtual void draw() const;

  //: Print details about this becld_episeg to the given stream.
  virtual vcl_ostream& print(vcl_ostream&) const;

  //: Returns the type of this class ('becld_episeg_soview2D').
  vcl_string type_name() const { return "becld_episeg_soview2D"; }

  //: Returns the distance squared from this episeg to the given position.
  virtual float distance_squared(float x, float y) const;

  //: Returns the centroid of this episeg.
  void get_centroid(float* x, float* y) const;

  //: Translate this soview2D by the given x and y distances.
  virtual void translate(float x, float y);

  //: Returns a smart pointer to the segment
  becld_episeg_sptr episeg() const { return episeg_; }

protected:
  //: Update the sample points from the episeg
  void update_sample_points();

  //: Smart pointer to a becld_episeg
  becld_episeg_sptr episeg_;

  //: A cached vector of sample points
  vcl_vector<vgl_point_2d<double> > samples_;
};


#endif // becld_episeg_soview2D_h_
