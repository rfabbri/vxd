// This is mw_discrete_corresp_storage_3.h
#ifndef mw_discrete_corresp_storage_3_h
#define mw_discrete_corresp_storage_3_h
//:
//\file
//\brief Stores point correspondences
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri@gmail.com)
//\date 04/24/2006 02:32:22 PM EDT
//

#include <bpro1/bpro1_storage.h>
#include <mw/pro/mw_discrete_corresp_storage_3_sptr.h>
#include <bmcsd/bmcsd_discrete_corresp_3.h>



class mw_discrete_corresp_storage_3: public bpro1_storage 
{
public:
  mw_discrete_corresp_storage_3();
  virtual ~mw_discrete_corresp_storage_3();
  virtual vcl_string type() const { return "mw_3_pt_corresp"; }

  //: Create a copy of the object on the heap.
  // The caller is responsible for deletion
  virtual bpro1_storage* clone() const;
  
  //: Return a platform independent string identifying the class
  virtual vcl_string is_a() const { return "mw_point_corresp_storage_3"; }

  bmcsd_discrete_corresp_3 *corresp() {
    return c_;
  }
 
  //: Sets the corresp object pointer. Storage must be allocated by the user and will be
  // deleted by _this_ class. In the future, this might be replaced by a smart pointer.
  void set_corresp(bmcsd_discrete_corresp_3 *new_c) {
    c_ = new_c;
  }

  //: Return IO version number;
  short version() const { return 1; }
  
  //: Binary save self to stream.
  void b_write(vsl_b_ostream &os) const;

  //: Binary load self from stream.
  void b_read(vsl_b_istream &is);

  
protected:

private:

  bmcsd_discrete_corresp_3 *c_;
};

//: Create a smart-pointer to a mw_discrete_corresp_storage_3
struct mw_discrete_corresp_storage_3_new : public mw_discrete_corresp_storage_3_sptr
{
  typedef mw_discrete_corresp_storage_3_sptr base;

  //: Constructor - creates a default vidpro1_curve_storage_sptr.
  mw_discrete_corresp_storage_3_new() : base(new mw_discrete_corresp_storage_3()) { }
};

#endif // mw_discrete_corresp_storage_3_h

