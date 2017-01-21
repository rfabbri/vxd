// This is mw_discrete_corresp_storage.h
#ifndef mw_discrete_corresp_storage_h
#define mw_discrete_corresp_storage_h
//:
//\file
//\brief Stores point correspondences
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 04/24/2006 02:32:22 PM EDT
//

#include <bpro1/bpro1_storage.h>
#include <mw/pro/mw_discrete_corresp_storage_sptr.h>
#include <bmcsd/bmcsd_discrete_corresp.h>


class mw_discrete_corresp_storage: public bpro1_storage 
{
public:
  mw_discrete_corresp_storage();
  virtual ~mw_discrete_corresp_storage();

  // For backward compatibility reasons, the name still has 'pt' in it.
  virtual vcl_string type() const { return "mw pt corresp"; }

  //: Create a copy of the object on the heap.
  // The caller is responsible for deletion
  virtual bpro1_storage* clone() const;
  
  //: Return a platform independent string identifying the class
  // For backward compatibility reasons, the name still has 'point' in it.
  virtual vcl_string is_a() const { return "mw_point_corresp_storage"; }

  bmcsd_discrete_corresp *corresp() {
    return c_;
  }
 
  //: Sets the corresp object pointer. Storage must be allocated by the user and will be
  // deleted by _this_ class. In the future, this might be replaced by a smart pointer.
  void set_corresp(bmcsd_discrete_corresp *new_c) {
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

  bmcsd_discrete_corresp *c_;
};

//: Create a smart-pointer to a mw_discrete_corresp_storage
struct mw_discrete_corresp_storage_new : public mw_discrete_corresp_storage_sptr
{
  typedef mw_discrete_corresp_storage_sptr base;

  //: Constructor - creates a default vidpro1_curve_storage_sptr.
  mw_discrete_corresp_storage_new() : base(new mw_discrete_corresp_storage()) { }
};

#endif // mw_discrete_corresp_storage_h

