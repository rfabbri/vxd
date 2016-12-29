#ifndef dvpgl_fm_storage_h_
#define dvpgl_fm_storage_h_

//:
// \file
// \brief The vidpro1 storage class for fundamental matrices.
// \author Thomas Pollard
// \date 7/14/05
//
// \verbatim
//  This storage object contains a list of fundamental matrices whose RHS corresponds to
//  the frame at which the storage object is attached to, and whose LHS corresponds to 
//  another frame specified by the add_fm and get_fm functions.
//  Note: Adapted from Matt Leotta's vidpro1_fmatrix_storage.
// \endverbatim


#include <bpro1/bpro1_storage.h>
#include "dvpgl_fm_storage_sptr.h"
#include <vpgl/vpgl_fundamental_matrix.h>

#include <vcl_map.h>
#include <vcl_string.h>

class dvpgl_fm_storage : public bpro1_storage {

public:

  
  // Data is stored as vnl_double_3x3 rather than dvpgl_fundamental_matrix, since 
  // all attempts at doing this properly have been rewarded only by SUFFERING.
  typedef vcl_map< int, vcl_map< vcl_string, vnl_double_3x3 > > data_map;

  //: Constructor
  dvpgl_fm_storage(const vcl_string& from_name = "");

  //: Destructor
  virtual ~dvpgl_fm_storage();

  //: Return the type identifier string
  virtual vcl_string type() const { return "vpgl fm"; }

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
  virtual vcl_string is_a() const { return "dvpgl_fm_storage"; }

  //: Add a fundamental matrix related to an object with \p name at \p frame
  void add_fm( const vpgl_fundamental_matrix<double>& fm, int frame, const vcl_string& name );

  //: Retrieve a fundamental matrix related to an object with \p name at \p frame
  bool get_fm( vpgl_fundamental_matrix<double>& fm, int frame, const vcl_string& name ) const;

private:

  //: The name of the image that all fundamental matrices map from
  const vcl_string from_name_;
  //: The data
  data_map f_map_; 

};


//: Create a smart-pointer to a dvpgl_fm_storage.
struct dvpgl_fm_storage_new : public dvpgl_fm_storage_sptr
{
  typedef dvpgl_fm_storage_sptr base;

  //: Constructor - creates a default vidpro1_fmatrix_storage_sptr.
  dvpgl_fm_storage_new(const vcl_string& from_name = "") 
    : base(new dvpgl_fm_storage(from_name)) { }
};


#endif //dvpgl_fm_storage_h_
