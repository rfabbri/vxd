// This is basic/bspid/pro/bspid_curvemap_storage.h
#ifndef bspid_curvemap_storage_h_
#define bspid_curvemap_storage_h_

//:
// \file
// \brief Stores a bspid_curve_map and a vil image of it
// \author Based on original code by  Matt Leotta
// \date 7/19/04

#include <bpro1/bpro1_storage.h>
#include "bspid_curvemap_storage_sptr.h"
#include <vil/vil_image_resource_sptr.h>
#include <bspid/bspid_curve_map.h>


class bspid_curvemap_storage : public bpro1_storage {

public:

  bspid_curvemap_storage();
  virtual ~bspid_curvemap_storage();
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
  virtual vcl_string is_a() const { return "bspid_curvemap_storage"; }

  

  void set_image( const vil_image_resource_sptr& img );
  void set_curvemap( bspid_curve_map * cmap);
  vil_image_resource_sptr get_image();
  bspid_curve_map *get_curvemap();
  
protected:

private:
  
  vil_image_resource_sptr image_;
  bspid_curve_map * cmap_;
  
};

//: Create a smart-pointer to a bspid_curvemap_storage.
struct bspid_curvemap_storage_new : public bspid_curvemap_storage_sptr
{
  typedef bspid_curvemap_storage_sptr base;

  //: Constructor - creates a default bspid_curvemap_storage_sptr.
  bspid_curvemap_storage_new() : base(new bspid_curvemap_storage()) { }
};

#endif // bspid_curvemap_storage_h_
