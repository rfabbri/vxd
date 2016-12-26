// This is basic/dbpro/dbpro_delay.h
#ifndef dbpro_delay_h_
#define dbpro_delay_h_

//:
// \file
// \brief A filter that intoduces a time delay
// \author Matt Leotta
// \date 6/2/06
//
// \verbatim
//  Modifications
// \endverbatim


#include <dbpro/dbpro_process.h>
#include <dbpro/dbpro_connector.h>
#include <dbpro/dbpro_mutex.h>
#include <vcl_deque.h>


//: A process that intoduces a time delay
class dbpro_delay : public dbpro_process
{
 public:

  //: Constructor
  dbpro_delay(unsigned int size = 1);

  //: Constructor
  template <class T>
  dbpro_delay(unsigned int size, const T& data)
  : max_size_(size), recieved_timestamp_(0), recieved_signal_(DBPRO_VALID),
    queue_(size,new dbpro_storage_type<T>(data)) 
  {
  }

  //: Destructor
  virtual ~dbpro_delay();

  //: Runs the process
  dbpro_signal run(unsigned long timestamp,
                   dbpro_debug_observer* const debug = NULL);

  //: Execute this process
  dbpro_signal execute();
  
  //: Request data from dependents and shift the queue 
  void shift_queue(unsigned long timestamp,
                   dbpro_debug_observer* const debug = NULL);

 private:
  unsigned max_size_;
  unsigned long recieved_timestamp_;
  dbpro_signal recieved_signal_;
  vcl_deque<dbpro_storage_sptr> queue_;

  //: mutex for updating the queue
  dbpro_mutex queue_mutex_;

};


#endif // dbpro_delay_h_
