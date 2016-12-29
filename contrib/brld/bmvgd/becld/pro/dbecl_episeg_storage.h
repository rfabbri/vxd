// This is brcv/mvg/dbecl/pro/dbecl_episeg_storage.h
#ifndef ddbecl_episeg_storage_h
#define ddbecl_episeg_storage_h

// \file
// \brief A storage class for dbecl episegments
// \author  mleotta
// \date  11/8/2004

#include <bpro1/bpro1_storage.h>
#include <dbecl/dbecl_episeg_sptr.h>
#include "dbecl_episeg_storage_sptr.h"

//: Storage class for dbecl episegments
class dbecl_episeg_storage : public bpro1_storage {

public:
  //: Constructor
  dbecl_episeg_storage() {}
  //: Destructor
  virtual ~dbecl_episeg_storage() {}
  virtual vcl_string type() const { return "episeg"; }

  //: Create a copy of the object on the heap.
  // The caller is responsible for deletion
  virtual bpro1_storage* clone() const;
  
  //: Return a platform independent string identifying the class
  virtual vcl_string is_a() const { return "dbecl_episeg_storage"; }

  //: Set the episegments
  void set_episegs(const vcl_vector<dbecl_episeg_sptr>& episegs);

  //: Access the episegments
  vcl_vector<dbecl_episeg_sptr> episegs() const;
  
private:
  vcl_vector<dbecl_episeg_sptr> episegs_;

};


//: Create a smart-pointer to a dbecl_episeg_storage.
struct dbecl_episeg_storage_new : public dbecl_episeg_storage_sptr
{
  typedef dbecl_episeg_storage_sptr base;

  //: Constructor - creates a default dbecl_episeg_storage_sptr.
  dbecl_episeg_storage_new() : base(new dbecl_episeg_storage()) { }
};

#endif
