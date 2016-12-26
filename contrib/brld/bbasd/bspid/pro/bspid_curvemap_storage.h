// This is basic/dbspi/pro/dbspi_curvemap_storage.h
#ifndef dbspi_curvemap_storage_h_
#define dbspi_curvemap_storage_h_

//:
// \file
// \brief Stores a dbspi_curve_map and a vil image of it
// \author Matt Leotta
// \date 7/19/04

#include <bpro1/bpro1_storage.h>
#include "dbspi_curvemap_storage_sptr.h"
#include <vil/vil_image_resource_sptr.h>
#include <dbspi/dbspi_curve_map.h>


class dbspi_curvemap_storage : public bpro1_storage {

public:

  dbspi_curvemap_storage();
  virtual ~dbspi_curvemap_storage();
  virtual vcl_string type() const { return "curvemap"; }
  
  //: Return IO version number;
  short version() const;
  
  //: Binary save self to stream.
  void b_write(vsl_b_ostream &os) const;

  //: Binary load self from stream.
  void b_read(vsl_b_istream &is);

  //: Create a copy of the object on the heap.
  // The caller is responsible for deletion
  virtual bpro1_storage* clone() const;
  
  //: Return a platform independent string identifying the class
  virtual vcl_string is_a() const { return "dbspi_curvemap_storage"; }

  

  void set_image( const vil_image_resource_sptr& img );
  void set_curvemap( dbspi_curve_map * cmap);
  vil_image_resource_sptr get_image();
  dbspi_curve_map *get_curvemap();
  
protected:

private:
  
  vil_image_resource_sptr image_;
  dbspi_curve_map * cmap_;
  
};

//: Create a smart-pointer to a dbspi_curvemap_storage.
struct dbspi_curvemap_storage_new : public dbspi_curvemap_storage_sptr
{
  typedef dbspi_curvemap_storage_sptr base;

  //: Constructor - creates a default dbspi_curvemap_storage_sptr.
  dbspi_curvemap_storage_new() : base(new dbspi_curvemap_storage()) { }
};

#endif // dbspi_curvemap_storage_h_
