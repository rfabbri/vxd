// This is basic/bprod/bprod_observer.h
#ifndef bprod_observer_h_
#define bprod_observer_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief The output observer base class
//
//  Using this observer design pattern, this class implements the observers of
//  output from a process.  The process will notify it's output observers
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 5/30/06
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------


#include <vbl/vbl_ref_count.h>

#include <bprod/bprod_process_sptr.h>
#include <bprod/bprod_storage_sptr.h>
#include <bprod/bprod_observer_sptr.h>

//: This abstract class is the base class for observer objects

class bprod_observer : public vbl_ref_count
{
 public:

  //: Called by the process when the data is ready
  virtual bool notify(const bprod_storage_sptr& data, unsigned long timestamp) = 0;

  //: Indicate that the observer will no longer be notified
  virtual bool unlink() { return true; }

 protected:
  //: Disable direct construction and destruction of the base class
  bprod_observer(const bprod_observer& /*other*/) : vbl_ref_count() {}
  bprod_observer(){}
  virtual ~bprod_observer(){}

};




#endif // bprod_observer_h_
