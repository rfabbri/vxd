// This is brl/vidpro1/storage/vidpro1_vsol3D_storage.cxx

//:
// \file

#include "vidpro1_vsol3D_storage.h"
#include <vcl_utility.h>
#include <vcl_algorithm.h>
#include <vsl/vsl_map_io.h>
#include <vsol/vsol_point_3d.h>
#include <vsol/vsol_line_3d.h>
#include <vsol/vsol_polyline_3d.h>
#include <vsol/vsol_polygon_3d.h>
#include <vsol/vsol_rectangle_3d.h>
#include <vsol/vsol_triangle_3d.h>
//#include <vsol/vsol_conic_3d.h>
#include <vsol/vsol_group_3d.h>

#include <vdgl/vdgl_digital_curve.h>

bool vidpro1_vsol3D_storage::registered_ = false;

//: Constructor
vidpro1_vsol3D_storage::vidpro1_vsol3D_storage()
{
}

//: Destructor
vidpro1_vsol3D_storage::~vidpro1_vsol3D_storage()
{
}

#if 0  
//: Register vsol_spatial_object_3D types for I/O
void 
vidpro1_vsol3D_storage::register_binary_io() const
{
  if(!registered_){
    vsl_add_to_binary_loader(vsol_point_3d(0.0,0.0,0.0));
    vsl_add_to_binary_loader( vsol_line_3d( new vsol_point_3d(0.0,0.0,0.0), new vsol_point_3d(0.0,0.0,0.0) ) ) ;
    vsl_add_to_binary_loader(vsol_polyline_3d());
  //  vsl_add_to_binary_loader(vsol_polygon_3d());
  //  vsl_add_to_binary_loader(vsol_rectangle_3d());
  //  vsl_add_to_binary_loader(vsol_triangle_3d());
   
    vsl_add_to_binary_loader(vsol_group_3d());
    
    vsl_add_to_binary_loader(vdgl_digital_curve());

    registered_ = true;
  }
}

#endif
//: Create a copy of the object on the heap.
// The caller is responsible for deletion
bpro1_storage* 
vidpro1_vsol3D_storage::clone() const
{
  return new vidpro1_vsol3D_storage(*this);
}

#if 0
//: Return IO version number;
short 
vidpro1_vsol3D_storage::version() const
{
  return 2;
}


//: Binary save self to stream.
void 
vidpro1_vsol3D_storage::b_write(vsl_b_ostream &os) const
{
  vsl_b_write(os, version());
  vidpro1_storage::b_write(os);
  vsl_b_write(os, vsol_map_);
}


//: Binary load self from stream.
void 
vidpro1_vsol3D_storage::b_read(vsl_b_istream &is)
{
  if (!is) return;

  short ver;
  vsl_b_read(is, ver);
  switch(ver)
  {
  case 1:
  {
    int count; // not used anymore
    vidpro1_storage::b_read(is);
    vsl_b_read(is, count);
    vsl_b_read(is, vsol_map_);
    break;
  }
  case 2:
  {
    vidpro1_storage::b_read(is);
    vsl_b_read(is, vsol_map_);
    break;
  }

  default:
    vcl_cerr << "I/O ERROR: vidpro1_vsol3D_storage::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}

#endif


//: Remove all vsol3D
void
vidpro1_vsol3D_storage::clear_all()
{
  vsol_map_.clear();
}


//: Add a vector of vsol3D objects to the group named \p which
void
vidpro1_vsol3D_storage::add_objects( const vcl_vector< vsol_spatial_object_3d_sptr >& objects,  
                                    const vcl_string& which)
{
  data_map::iterator result = vsol_map_.find( which );

  if( result == vsol_map_.end() ) {
    typedef vcl_pair<vcl_string, vcl_vector<vsol_spatial_object_3d_sptr> > data_pair;
    vsol_map_.insert( data_pair( which, objects ) );
  }
  else {
    for( vcl_vector<vsol_spatial_object_3d_sptr>::const_iterator 
         it = objects.begin();  it != objects.end();  ++it )
      result->second.push_back( *it );
  }
}


//: Add an object to the group named \p which
void
vidpro1_vsol3D_storage::add_object( const vsol_spatial_object_3d_sptr& object,  
                                   const vcl_string& which )
{
  vsol_map_[which].push_back(object);
}


//: Search for the object in all groups and remove it
// \retval The name of the group removed from of the empty string if not found
vcl_string 
vidpro1_vsol3D_storage::remove_object( const vsol_spatial_object_3d_sptr& object )
{
  for( data_map::const_iterator it = vsol_map_.begin();
       it != vsol_map_.end();  ++it ) {
    if(remove_object(object, it->first))
      return it->first;
  }
  return "";
}


//: Search for the object in the given group only and remove it
// \return true if the object was successfully removed
// \return false if the object was not found
bool 
vidpro1_vsol3D_storage::remove_object( const vsol_spatial_object_3d_sptr& object, 
                                      const vcl_string& group )
{
  data_map::iterator it =vsol_map_.find(group);
  if( it == vsol_map_.end() ) 
    return false;
  
  vcl_vector<vsol_spatial_object_3d_sptr>::iterator 
    itv = vcl_find(it->second.begin(), it->second.end(), object);
  if( itv == it->second.end() ) 
    return false;

  it->second.erase(itv);
  return true;
}


//: Return a vector of all vsol objects in the group named \p which
vcl_vector< vsol_spatial_object_3d_sptr > 
vidpro1_vsol3D_storage::data_named( const vcl_string& which) const
{
  data_map::const_iterator it = vsol_map_.find( which );

  if( it == vsol_map_.end() ) {
    return vcl_vector< vsol_spatial_object_3d_sptr >();
  }

  return it->second;
}


//: Return a vector of all vsol objects in all groups
vcl_vector< vsol_spatial_object_3d_sptr > 
vidpro1_vsol3D_storage::all_data() const
{
  vcl_vector<vsol_spatial_object_3d_sptr> all_data;
  for( data_map::const_iterator it = vsol_map_.begin();
       it != vsol_map_.end();  ++it ) 
  {
    for( vcl_vector<vsol_spatial_object_3d_sptr>::const_iterator 
         it2 = it->second.begin();  it2 != it->second.end();  ++it2 )
    { 
      all_data.push_back( *it2 );
    }
  }
  return all_data;
}


//: Return the names of all groups 
vcl_vector< vcl_string >
vidpro1_vsol3D_storage::groups() const
{
  vcl_vector<vcl_string> name_list;
  for( data_map::const_iterator it = vsol_map_.begin();
       it != vsol_map_.end();  ++it ) {
    name_list.push_back( it->first );
  }

  return name_list;
}


