// This is basic/dbpro/dbpro_mutex.h
#ifndef dbpro_mutex_h_
#define dbpro_mutex_h_
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
class dbpro_mutex
{
public:
  //: Default Constructor
  dbpro_mutex();
  
  //: Destructor
  ~dbpro_mutex();
  
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



#endif // dbpro_mutex_h_
