// This is brl/vidpro1/storage/vidpro1_fmatrix_storage.h
#ifndef vidpro1_fmatrix_storage_h_
#define vidpro1_fmatrix_storage_h_

//:
// \file
// \brief The vidpro1 storage class for fundamental matrices
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 10/21/04
//
// \verbatim
//  Modifications
// \endverbatim


#include <bpro1/bpro1_storage.h>
#include <vidpro1/storage/vidpro1_fmatrix_storage_sptr.h>
#include <vnl/vnl_double_3x3.h>

#include <vcl_map.h>
#include <vcl_string.h>

class vidpro1_fmatrix_storage : public bpro1_storage {

public:
  typedef vcl_map< int, vcl_map< vcl_string, vnl_double_3x3 > > data_map;

  //: Constructor
  vidpro1_fmatrix_storage(const vcl_string& from_name = "");

  //: Destructor
  virtual ~vidpro1_fmatrix_storage();

  //: Return the type identifier string
  virtual vcl_string type() const { return "fmatrix"; }

  //: Return the name of the image that all fundamental matrices map from
  vcl_string from_name() const { return from_name_; }
        
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
  virtual vcl_string is_a() const { return "vidpro1_fmatrix_storage"; }

  //: Add an FMatrix related to an object with \p name at \p frame
  void add_fmatrix( const vnl_double_3x3& F, int frame, const vcl_string& name );

  //: Retrieve an FMatrix related to an object with \p name at \p frame
  bool get_fmatrix( vnl_double_3x3& F, int frame, const vcl_string& name ) const;

private:

  //: The name of the image that all fundamental matrices map from
  const vcl_string from_name_;
  //: The data
  data_map f_map_; 

};


//: Create a smart-pointer to a vidpro1_fmatrix_storage.
struct vidpro1_fmatrix_storage_new : public vidpro1_fmatrix_storage_sptr
{
  typedef vidpro1_fmatrix_storage_sptr base;

  //: Constructor - creates a default vidpro1_fmatrix_storage_sptr.
  vidpro1_fmatrix_storage_new(const vcl_string& from_name = "") 
    : base(new vidpro1_fmatrix_storage(from_name)) { }
};


#endif //vidpro1_fmatrix_storage_h_
