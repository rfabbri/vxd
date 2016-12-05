// This is bdifd_3rd_order_geometry_storage.h
#ifndef bdifd_3rd_order_geometry_storage_h
#define bdifd_3rd_order_geometry_storage_h
//:
//\file
//\brief Stores a sequence of 2D point-tangent-curvature-kderivative
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 04/24/2006 02:32:22 PM EDT
//

#include <bpro1d/bpro1d_storage.h>
#include <bdifd/pro/bdifd_3rd_order_geometry_storage_sptr.h>
#include <bdifd/bdifd_frenet.h>



//: Stores a sequence of 2D point-tangent-curvature-kderivative
class bdifd_3rd_order_geometry_storage: public bpro1d_storage 
{
public:
  bdifd_3rd_order_geometry_storage();
  virtual ~bdifd_3rd_order_geometry_storage();
  virtual vcl_string type() const { return "3rdOrderGeometry"; }

  //: Create a copy of the object on the heap.
  // The caller is responsible for deletion
  virtual bpro1d_storage* clone() const;
  
  //: Return a platform independent string identifying the class
  virtual vcl_string is_a() const { return "bdifd_3rd_order_geometry_storage"; }

  vcl_vector<bdifd_3rd_order_point_2d> &c() {
    return c_;
  }
 
  //: Sets the corresp object pointer. Storage must be allocated by the user and will be
  // deleted by _this_ class. In the future, this might be replaced by a smart pointer.
  void set_c(vcl_vector<bdifd_3rd_order_point_2d> &new_c) {
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

  vcl_vector<bdifd_3rd_order_point_2d> c_;
};

//: Create a smart-pointer to a bdifd_3rd_order_geometry_storage
struct bdifd_3rd_order_geometry_storage_new : public bdifd_3rd_order_geometry_storage_sptr
{
  typedef bdifd_3rd_order_geometry_storage_sptr base;

  //: Constructor - creates a default vidpro1_curve_storage_sptr.
  bdifd_3rd_order_geometry_storage_new() : base(new bdifd_3rd_order_geometry_storage()) { }
};

#endif // bdifd_3rd_order_geometry_storage_h

