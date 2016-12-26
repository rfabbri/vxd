// This is basic/dbpro/dbpro_observer.h
#ifndef dbpro_observer_h_
#define dbpro_observer_h_
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

#include <dbpro/dbpro_process_sptr.h>
#include <dbpro/dbpro_storage_sptr.h>
#include <dbpro/dbpro_observer_sptr.h>

//: This abstract class is the base class for observer objects

class dbpro_observer : public vbl_ref_count
{
 public:

  //: Called by the process when the data is ready
  virtual bool notify(const dbpro_storage_sptr& data, unsigned long timestamp) = 0;

  //: Indicate that the observer will no longer be notified
  virtual bool unlink() { return true; }

 protected:
  //: Disable direct construction and destruction of the base class
  dbpro_observer(const dbpro_observer& /*other*/) : vbl_ref_count() {}
  dbpro_observer(){}
  virtual ~dbpro_observer(){}

};




#endif // dbpro_observer_h_
