// This is dbdif_3rd_order_geometry_storage.h
#ifndef dbdif_3rd_order_geometry_storage_h
#define dbdif_3rd_order_geometry_storage_h
//:
//\file
//\brief Stores a sequence of 2D point-tangent-curvature-kderivative
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 04/24/2006 02:32:22 PM EDT
//

#include <bpro1/bpro1_storage.h>
#include <dbdif/pro/dbdif_3rd_order_geometry_storage_sptr.h>
#include <dbdif/dbdif_frenet.h>



//: Stores a sequence of 2D point-tangent-curvature-kderivative
class dbdif_3rd_order_geometry_storage: public bpro1_storage 
{
public:
  dbdif_3rd_order_geometry_storage();
  virtual ~dbdif_3rd_order_geometry_storage();
  virtual vcl_string type() const { return "3rdOrderGeometry"; }

  //: Create a copy of the object on the heap.
  // The caller is responsible for deletion
  virtual bpro1_storage* clone() const;
  
  //: Return a platform independent string identifying the class
  virtual vcl_string is_a() const { return "dbdif_3rd_order_geometry_storage"; }

  vcl_vector<dbdif_3rd_order_point_2d> &c() {
    return c_;
  }
 
  //: Sets the corresp object pointer. Storage must be allocated by the user and will be
  // deleted by _this_ class. In the future, this might be replaced by a smart pointer.
  void set_c(vcl_vector<dbdif_3rd_order_point_2d> &new_c) {
    c_ = new_c;
  }

  //: Return IO version number;
  short version() const { return 1; }
  
  /*
  //: Binary save self to stream.
  void b_write(vsl_b_ostream &os) const;

  //: Binary load self from stream.
  void b_read(vsl_b_istream &is);
  */

  
protected:

private:

  vcl_vector<dbdif_3rd_order_point_2d> c_;
};

//: Create a smart-pointer to a dbdif_3rd_order_geometry_storage
struct dbdif_3rd_order_geometry_storage_new : public dbdif_3rd_order_geometry_storage_sptr
{
  typedef dbdif_3rd_order_geometry_storage_sptr base;

  //: Constructor - creates a default vidpro1_curve_storage_sptr.
  dbdif_3rd_order_geometry_storage_new() : base(new dbdif_3rd_order_geometry_storage()) { }
};

#endif // dbdif_3rd_order_geometry_storage_h

