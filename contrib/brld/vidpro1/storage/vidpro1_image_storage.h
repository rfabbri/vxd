// This is brl/vidpro1/storage/vidpro1_image_storage.h
#ifndef vidpro1_image_storage_h_
#define vidpro1_image_storage_h_

//:
// \file
// \brief The vidpro1 storage class for an image
// \author Mark Johnson (mrj@lems.brown.edu)
// \date 7/15/03
//
//
// \verbatim
//  Modifications
// \endverbatim



#include <bpro1/bpro1_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>
#include <vil/vil_image_resource_sptr.h>


class vidpro1_image_storage : public bpro1_storage {

public:

  vidpro1_image_storage();
  virtual ~vidpro1_image_storage();
  virtual vcl_string type() const { return "image"; }
  
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
  virtual vcl_string is_a() const { return "vidpro1_image_storage"; }

  

  void set_image( const vil_image_resource_sptr& img );
  vil_image_resource_sptr get_image();
  
protected:

private:
  
  vil_image_resource_sptr image_;
  
};

//: Create a smart-pointer to a vidpro1_image_storage.
struct vidpro1_image_storage_new : public vidpro1_image_storage_sptr
{
  typedef vidpro1_image_storage_sptr base;

  //: Constructor - creates a default vidpro1_image_storage_sptr.
  vidpro1_image_storage_new() : base(new vidpro1_image_storage()) { }
};

#endif // vidpro1_image_storage_h_
