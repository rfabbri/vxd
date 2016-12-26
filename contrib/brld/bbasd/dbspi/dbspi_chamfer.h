// This is basic/dbspi/dbspi_chamfer.h
#ifndef dbspi_chamfer_h_
#define dbspi_chamfer_h_

//:
// \file
// \author Matt Leotta
// \date   7/19/04
// \brief computes a distance transform for any object
//
//  This templated class maintains two sets of 2D arrays, one of
//  objects and one of distances.  An initial map of object locations
//  is provided to the constructor.  At construction time a similar
//  array of distances is initialized assigning zero distance at all
//  locations where objects are found.  The chamfer algorithms is
//  applied and objects are propagated with distances.  The result
//  is a distance transform with refences to the nearest object at
//  each location.  
//
// \verbatim
// Modifications
//   Adapted from the original by Charlie Rothwell - 4/5/95
// \endverbatim
//

#include <vbl/vbl_array_2d.h>


//: chamfer distance transform class
// This class is templated over object type \pOBJ_T and distance type \pDIST_T
//
// Object types need to support a cast to boolean, where false indicates the
// absence of an object.  Typically objects are pointers, smart pointers, or
// indices where the NULL pointer or zero index casts to false.
//
// Distance types need to support addition, the <= operator, and
// vcl_numeric_limits<DIST_T>::max().  They must also exibit
// a strict weak ordering.  Distance types are typically \pint or \pfloat.
template<class OBJ_T, class DIST_T>
class dbspi_chamfer
{
 public:

  //: Constructor
  // \param object_map is a map of the initial object locations.  Occupied locations
  //                   should evaluate to true under boolean cast while unoccupied
  //                   locations should evaluate to false.
  // \param dist1 is the distance in the horizontal and vertical directions between elements.
  // \param dist2 is the distance in the diagonal directions between elements.
  // \note typically \pdist2 = \pdist1 * sqrt(2) but this must be approximated if
  //       \pDIST_T is descrete (i.e. dist1=3 and dist2=4)
  dbspi_chamfer(const vbl_array_2d<OBJ_T>& object_map, DIST_T dist1, DIST_T dist2);
  //: Destructor
  ~dbspi_chamfer();

  //: Distance to the nearest object at (x,y)
  DIST_T distance(int x, int y) const { return distance_[y][x]; }
  //: The nearest object to (x,y)
  // \note it is possible that other objects are equally close
  OBJ_T closest(int x, int y) const { return objects_[y][x]; }

  //: Return the width of the chamfer arrays
  unsigned int width() const { return objects_.cols(); }
  //: Return the height of the chamfer arrays
  unsigned int height() const { return objects_.rows(); }

  //: Return the number of elements with zero distance
  unsigned int zero_count() const { return zero_count_; }


 private:
  //: Apply the chamfer algorithm in the forward direction
  void forward_chamfer();
  //: Apply the chamfer algorithm in the backward direction
  void backward_chamfer();

  //: Distance to the horizontal and vertical neighbors
  DIST_T dist1_;
  //: Distance to the diagonal neighbors
  DIST_T dist2_;

  //: The size of the distance image
  int xsize_, ysize_;

  //: The number of elements with zero distance
  unsigned int zero_count_;

  //: The distance image
  vbl_array_2d<DIST_T> distance_;
  //: Object nearest to each point
  vbl_array_2d<OBJ_T> objects_;

};

#endif // dbspi_chamfer_h_ 
