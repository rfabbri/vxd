// This is basic/dbspi/dbspi_curve_map.cxx

//:
// \file

#include "dbspi_curve_map.h"
#include <vsol/vsol_digital_curve_2d.h>
#include <vsol/vsol_point_2d.h>
#include <vcl_limits.h>
#include <vcl_cmath.h>



//: Constructor
dbspi_curve_map::dbspi_curve_map(const vcl_vector<vsol_digital_curve_2d_sptr>& curves,
                                 unsigned int width, unsigned int height,
                                 int x_offset, int y_offset)
 : x_offset_(x_offset), y_offset_(y_offset),
   map_(make_array_fixed(curves, width+2, height+2), 3.0, 4.242641f)
{
}


//: Constructor
dbspi_curve_map::dbspi_curve_map(const vcl_vector<vsol_digital_curve_2d_sptr>& curves)
 : x_offset_(0), y_offset_(0),
   map_(make_array(x_offset_, y_offset_, curves), 3.0, 4.242641f)
{
}


//: Destructor
dbspi_curve_map::~dbspi_curve_map()
{
}


//: Return the approximate distance to a curve at (x,y)
float 
dbspi_curve_map::distance(int x, int y) const
{
  if( x < x_offset_ || x >= (int(width())+x_offset_) ||
      y < y_offset_ || y >= (int(height())+y_offset_) )
    return vcl_numeric_limits<float>::infinity();
  return float(map_.distance(x-x_offset_+1,y-y_offset_+1))/3.0f;
}


//: Return a smart pointer to the closest curve at (x,y)
vsol_digital_curve_2d_sptr 
dbspi_curve_map::closest(int x, int y) const
{
  if( x < x_offset_ || x >= (int(width())+x_offset_) ||
      y < y_offset_ || y >= (int(height())+y_offset_) )
    return NULL;
  return map_.closest(x-x_offset_+1,y-y_offset_+1).first;
}


//: Return the label of the closest curve pixel to (x,y)
unsigned int 
dbspi_curve_map::label(int x, int y) const
{
  if( x < x_offset_ || x >= (int(width())+x_offset_) ||
      y < y_offset_ || y >= (int(height())+y_offset_) )
    return 0;
  return map_.closest(x-x_offset_+1,y-y_offset_+1).second;
}


//: Recursive helper function to fill in the gaps between sample points
void
dbspi_curve_map::fill_between(vbl_array_2d<map_element>& array,
                              const vsol_digital_curve_2d_sptr dc,
                              const vgl_point_2d<int>& min_pt, 
                              const vgl_point_2d<int>& max_pt )
{
  if( vcl_abs((float) (max_pt.x() - min_pt.x())) < 2 && 
      vcl_abs((float) (max_pt.y() - min_pt.y())) < 2 )
    return;
  
  vgl_point_2d<int> mid_pt = centre(min_pt, max_pt);
  array(mid_pt.y()+1,mid_pt.x()+1) = map_element(dc.ptr(),1);

  fill_between(array, dc, mid_pt, max_pt);
  fill_between(array, dc, min_pt, mid_pt);
}


//: Make an array from these curves
vbl_array_2d<dbspi_curve_map::map_element> 
dbspi_curve_map::make_array_fixed(const vcl_vector<vsol_digital_curve_2d_sptr>& curves,
                                  unsigned int width, unsigned int height,
                                  int x_offset, int y_offset)
{
  vbl_array_2d<map_element> array(height, width, map_element(NULL,0));
  // Iterate through all curves
  for ( vcl_vector<vsol_digital_curve_2d_sptr>::const_iterator itr=curves.begin();
        itr != curves.end();  ++itr )
  {
    vsol_digital_curve_2d_sptr dc = *itr;
    if(dc->size() < 1) // ignore any empty curves
      continue;
    // Round each point in the curve to a pixel location
    vgl_point_2d<double> temp_pt = dc->p0()->get_p();
    vgl_point_2d<int> last_pt((int)vcl_floor(temp_pt.x()+0.5)-x_offset, 
                              (int)vcl_floor(temp_pt.y()+0.5)-y_offset);
    for( unsigned int p=0; p<dc->size(); ++p )
    {
      temp_pt = dc->point(p)->get_p();
      vgl_point_2d<int> pt((int)vcl_floor(temp_pt.x()+0.5)-x_offset, 
                           (int)vcl_floor(temp_pt.y()+0.5)-y_offset);
      // check that the pixel falls within the array
      assert(pt.x() >= 0 && pt.x() < int(width));
      assert(pt.y() >= 0 && pt.y() < int(height));
      // plot this curve pointer as a pixel in the object pointer image
      array(pt.y()+1,pt.x()+1) = map_element(dc.ptr(),1);
      // recursively fill in the pixels on the line between the two
      fill_between(array, dc, last_pt, pt); 
      
      last_pt = pt;
    }
  }

  // label each pixel with a number
  unsigned int count = 0;
  for( vbl_array_2d<map_element>::iterator itr = array.begin();
       itr != array.end();  ++itr )
    if( itr->second != 0 )
      itr->second = ++count;

  return array;
}


//: Make an array from these curves
vbl_array_2d<dbspi_curve_map::map_element> 
dbspi_curve_map::make_array(int& x_offset, int& y_offset,
                            const vcl_vector<vsol_digital_curve_2d_sptr>& curves)
{
  int x_min = vcl_numeric_limits<int>::max();
  int y_min = vcl_numeric_limits<int>::max();
  int x_max = vcl_numeric_limits<int>::min();
  int y_max = vcl_numeric_limits<int>::min();
  // Iterate through all curves
  for ( vcl_vector<vsol_digital_curve_2d_sptr>::const_iterator itr=curves.begin();
        itr != curves.end();  ++itr )
  {
    vsol_digital_curve_2d_sptr dc = *itr;
    if(dc->size() < 1) // ignore any empty curves
      continue;
    // Round each point in the curve to a pixel location
    // and update the bounds
    for( unsigned int p=0; p<dc->size(); ++p )
    {
      vgl_point_2d<double> temp_pt = dc->point(p)->get_p();
      int x = (int)vcl_floor(temp_pt.x()+0.5);
      int y = (int)vcl_floor(temp_pt.y()+0.5);
      if( x > x_max ) x_max = x;
      if( x < x_min ) x_min = x;
      if( y > y_max ) y_max = y;
      if( y < y_min ) y_min = y;
    }
  }
  x_offset = x_min-100;
  y_offset = y_min-100;

  return make_array_fixed(curves,x_max-x_min+200, y_max-y_min+200, x_offset, y_offset);
}
