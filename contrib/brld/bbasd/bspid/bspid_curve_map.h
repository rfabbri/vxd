// This is basic/dbspi/dbspi_curve_map.h
#ifndef dbspi_curve_map_h_
#define dbspi_curve_map_h_

//:
// \file
// \author Matt Leotta
// \date   7/19/04
// \brief computes a distance transform for vsol_digital_curve_2d
//
//  This class uses the chamfer algorithm to create a distance map
//  for a vector of vsol_digital_curve_2d.  This class also labels
//  each point of the discretized curve so you can determine which
//  point on the curve is closest.
//
// \verbatim
// Modifications
// \endverbatim
//

#include "dbspi_chamfer.h"
#include <vcl_utility.h>
#include <vcl_vector.h>
#include <vsol/vsol_digital_curve_2d_sptr.h>
#include <vgl/vgl_point_2d.h>



//: A distance map for vsol_digital_curves
class dbspi_curve_map
{
 public: 
   //: The element of data at each point of the map describing the nearest curve
  class map_element: public vcl_pair<vsol_digital_curve_2d*,unsigned int>
  {
   public:
    //: Default Constructor
    map_element()
      : vcl_pair<vsol_digital_curve_2d*,unsigned int>() {}
    //: Constructor
    map_element(vsol_digital_curve_2d* dc, unsigned int i)
      : vcl_pair<vsol_digital_curve_2d*,unsigned int>(dc,i) {}
    //: Convert to bool
    operator bool() const{ return (this->first != NULL) || (this->second != 0); }
  };

  //: Constructor 
  dbspi_curve_map(const vcl_vector<vsol_digital_curve_2d_sptr>& curves);
  //: Constructor - specify the map size and offset
  // \note all curves must lie within the map array
  dbspi_curve_map(const vcl_vector<vsol_digital_curve_2d_sptr>& curves,
                  unsigned int width, unsigned int height,
                  int x_offset=0, int y_offset=0);
  //: Destructor
  ~dbspi_curve_map();

  //: Return the approximate distance to a curve at (x,y)
  float distance(int x, int y) const;
  //: Return a smart pointer to the closest curve at (x,y)
  vsol_digital_curve_2d_sptr closest(int x, int y) const;
  //: Return the label of the closest curve pixel to (x,y)
  unsigned int label(int x, int y) const;

  //: Return the width of the distance map
  unsigned int width() const { return map_.width()-2; }
  //: Return the height of the distance map
  unsigned int height() const { return map_.height()-2; }
  //: Return the offset in x
  int x_offset() const { return x_offset_; }  
  //: Return the offset in y
  int y_offset() const { return y_offset_; }

  //: return the number of elements in the map with zero distance to a curve
  unsigned int zero_count() const { return map_.zero_count(); }


 private:
  //: Make an array from these curves with dimensions width and height
  static vbl_array_2d<map_element> 
    make_array_fixed(const vcl_vector<vsol_digital_curve_2d_sptr>& curves,
                     unsigned int width, unsigned int height,
                     int x_offset = 0, int y_offset = 0);

  //: Make an array from these curves and return the offsets by reference
  static vbl_array_2d<map_element> 
    make_array(int& x_offset, int& y_offset,
               const vcl_vector<vsol_digital_curve_2d_sptr>& curves);

  //: Recursive helper function to fill in the gaps between sample points
  static void
    fill_between(vbl_array_2d<map_element>& array,
                 const vsol_digital_curve_2d_sptr dc,
                 const vgl_point_2d<int>& min_pt, 
                 const vgl_point_2d<int>& max_pt );

  // -------------  Private Data Members -----------------
  //: access offset in x
  int x_offset_;
  //: access offset in y
  int y_offset_;
  //: The distance map
  dbspi_chamfer<map_element, float> map_;

};


#endif // dbspi_curve_map_h_ 
