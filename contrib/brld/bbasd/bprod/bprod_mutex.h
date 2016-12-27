// This is basic/bprod/bprod_mutex.h
#ifndef bprod_mutex_h_
#define bprod_mutex_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief A mutex used if threads are enabled 
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 3/23/09
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------


#include <vcl_memory.h>

//: A mutex used if threads are enabled 
// If threads are not enabled, do nothing
class bprod_mutex
{
public:
  //: Default Constructor
  bprod_mutex();
  
  //: Destructor
  ~bprod_mutex();
  
  //: Lock the mutex
  void lock();
  
  //: Unlock the mutex
  void unlock();

private:
  
  //: private implementation struct
  struct pimpl;
  //: private implementation instance
  vcl_auto_ptr<pimpl> data_;

};



#endif // bprod_mutex_h_
