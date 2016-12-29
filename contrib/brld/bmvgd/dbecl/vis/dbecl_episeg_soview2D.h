// This is brcv/mvg/dbecl/vis/bgui_dbecl_soview2D.h
#ifndef dbecl_episeg_soview2D_h_
#define dbecl_episeg_soview2D_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief vgui_soview2D objects for dbecl classes
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 11/8/04
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------

#include <vcl_iosfwd.h>

#include <dbecl/dbecl_episeg_sptr.h>
#include <vcl_vector.h>
#include <vgl/vgl_point_2d.h>
#include <vgui/vgui_soview2D.h>

//: This is a soview2D for a dbecl_episeg with a smart pointer back to the dbecl_episeg
class dbecl_episeg_soview2D : public vgui_soview2D
{
 public:
  //: Constructor - creates a default view
  dbecl_episeg_soview2D( dbecl_episeg_sptr const & seg);

  //: Destructor
  virtual ~dbecl_episeg_soview2D();

  //: Render the dbecl_episeg on the display.
  virtual void draw() const;

  //: Print details about this dbecl_episeg to the given stream.
  virtual vcl_ostream& print(vcl_ostream&) const;

  //: Returns the type of this class ('dbecl_episeg_soview2D').
  vcl_string type_name() const { return "dbecl_episeg_soview2D"; }

  //: Returns the distance squared from this episeg to the given position.
  virtual float distance_squared(float x, float y) const;

  //: Returns the centroid of this episeg.
  void get_centroid(float* x, float* y) const;

  //: Translate this soview2D by the given x and y distances.
  virtual void translate(float x, float y);

  //: Returns a smart pointer to the segment
  dbecl_episeg_sptr episeg() const { return episeg_; }

protected:
  //: Update the sample points from the episeg
  void update_sample_points();

  //: Smart pointer to a dbecl_episeg
  dbecl_episeg_sptr episeg_;

  //: A cached vector of sample points
  vcl_vector<vgl_point_2d<double> > samples_;
};


#endif // dbecl_episeg_soview2D_h_
