// This is bmcsd_discrete_corresp_storage.h
#ifndef bmcsd_discrete_corresp_storage_h
#define bmcsd_discrete_corresp_storage_h
//:
//\file
//\brief Stores point correspondences
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 04/24/2006 02:32:22 PM EDT
//

#include <bpro1/bpro1_storage.h>
#include <bmcsd/pro/bmcsd_discrete_corresp_storage_sptr.h>
#include <bmcsd/bmcsd_discrete_corresp.h>


class bmcsd_discrete_corresp_storage: public bpro1_storage 
{
public:
  bmcsd_discrete_corresp_storage();
  virtual ~bmcsd_discrete_corresp_storage();

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

//: Create a smart-pointer to a bmcsd_discrete_corresp_storage
struct bmcsd_discrete_corresp_storage_new : public bmcsd_discrete_corresp_storage_sptr
{
  typedef bmcsd_discrete_corresp_storage_sptr base;

  //: Constructor - creates a default vidpro1_curve_storage_sptr.
  bmcsd_discrete_corresp_storage_new() : base(new bmcsd_discrete_corresp_storage()) { }
};

#endif // bmcsd_discrete_corresp_storage_h

