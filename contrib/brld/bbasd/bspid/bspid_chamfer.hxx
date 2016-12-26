// This is basic/dbspi/dbspi_chamfer.hxx
#ifndef dbspi_chamfer_txx_
#define dbspi_chamfer_txx_
//:
// \file
// \brief Templated definition of dbspi_chamfer.
// \author Based on original code by  Matt Leotta, (@lems.brown.edu)
// \date 7/19/2004


#include "dbspi_chamfer.h"
#include <vcl_limits.h>
#include <vcl_algorithm.h>


//:  Constructor
template<class OBJ_T, class DIST_T>
dbspi_chamfer<OBJ_T, DIST_T>::dbspi_chamfer( const vbl_array_2d<OBJ_T>& object_map, 
                                             DIST_T dist1, DIST_T dist2 )
 : dist1_(dist1), dist2_(dist2),
   xsize_(object_map.cols()), ysize_(object_map.rows()), 
   zero_count_(0),
   distance_(ysize_, xsize_, vcl_numeric_limits<DIST_T>::max()- vcl_max(dist1,dist2)),
   objects_(object_map)
{
  // initialize the distance map and count zero distance elements
  for (int x=0; x<xsize_; ++x)
    for (int y=0; y<ysize_; ++y)
      if( bool(objects_[y][x]) ){
        distance_[y][x] = DIST_T(0);
        ++zero_count_;
      }

  // Do the Chamfering
  this->forward_chamfer();
  this->backward_chamfer();
}


//: Destructor
template<class OBJ_T, class DIST_T>
dbspi_chamfer<OBJ_T, DIST_T>::~dbspi_chamfer()
{
}


//: Inline Helper - find which distance is minimum
template<class DIST_T>
inline unsigned char 
dbspi_chamfer_minimum_5(DIST_T a, DIST_T b, DIST_T c, DIST_T d, DIST_T e)
{
  if ( (a<=b) && (a<=c) && (a<=d) && (a<=e) )
    return 1;
  else if ( (b<=c) && (b<=d) && (b<=e) )
    return 2;
  else if ( (c<=d) && (c<=e) )
    return 3;
  else if ( d<=e )
    return 4;
  else
    return 5;
}


//: Performs a forward chamfer convolution on the distance_ and objects_ images
template<class OBJ_T, class DIST_T>
void 
dbspi_chamfer<OBJ_T, DIST_T>::forward_chamfer()
{
  int i,j;
  unsigned char ind;

  for (j=1;j<xsize_-1;j++)
    for (i=1;i<ysize_-1;i++)
    {
      ind = dbspi_chamfer_minimum_5<DIST_T>( distance_[i-1][j-1]+dist2_, 
                                             distance_[i-1][j]+dist1_,
                                             distance_[i-1][j+1]+dist2_, 
                                             distance_[i][j-1]+dist1_,
                                             distance_[i][j] );
      switch (ind){
        case 1:
          distance_[i][j] = distance_[i-1][j-1]+dist2_;
          objects_[i][j] = objects_[i-1][j-1];
          break;

        case 2:
          distance_[i][j] = distance_[i-1][j]+dist1_;
          objects_[i][j] = objects_[i-1][j];
          break;

        case 3:
          distance_[i][j] = distance_[i-1][j+1]+dist2_;
          objects_[i][j] = objects_[i-1][j+1];
          break;

        case 4:
          distance_[i][j] = distance_[i][j-1]+dist1_;
          objects_[i][j] = objects_[i][j-1];
          break;

        case 5:
          break;
      }
    }
}


//: Performs a backward chamfer convolution on the distance_ and objects_ images
template<class OBJ_T, class DIST_T>
void 
dbspi_chamfer<OBJ_T, DIST_T>::backward_chamfer()
{
  int i,j;
  unsigned char ind;

  for (j=xsize_-2;j>0;j--)
    for (i=ysize_-2;i>0;i--)
    {
      ind = dbspi_chamfer_minimum_5<DIST_T>( distance_[i][j],
                                             distance_[i][j+1]+dist1_,
                                             distance_[i+1][j-1]+dist2_,
                                             distance_[i+1][j]+dist1_,
                                             distance_[i+1][j+1]+dist2_ );
      switch (ind){
        case 1:
          break;

        case 2:
          distance_[i][j] = distance_[i][j+1]+dist1_;
          objects_[i][j] = objects_[i][j+1];
          break;

        case 3:
          distance_[i][j] = distance_[i+1][j-1]+dist2_;
          objects_[i][j] = objects_[i+1][j-1];
          break;

        case 4:
          distance_[i][j] = distance_[i+1][j]+dist1_;
          objects_[i][j] = objects_[i+1][j];
          break;

        case 5:
          distance_[i][j] = distance_[i+1][j+1]+dist2_;
          objects_[i][j] = objects_[i+1][j+1];
          break;
        }
      }
}

//===========================================================================================

//: Use this macro to instantiate dbspi_chamfer
#define DBSPI_CHAMFER_INSTANTIATE(OBJ_T, DIST_T) \
template class dbspi_chamfer< OBJ_T, DIST_T >;

#endif // dbspi_chamfer_txx_
