// This is basic/bprod/bprod_delay.h
#ifndef bprod_delay_h_
#define bprod_delay_h_

//:
// \file
// \brief A filter that intoduces a time delay
// \author Matt Leotta
// \date 6/2/06
//
// \verbatim
//  Modifications
// \endverbatim


#include <bprod/bprod_process.h>
#include <bprod/bprod_connector.h>
#include <bprod/bprod_mutex.h>
#include <vcl_deque.h>


//: A process that intoduces a time delay
class bprod_delay : public bprod_process
{
 public:

  //: Constructor
  bprod_delay(unsigned int size = 1);

  //: Constructor
  template <class T>
  bprod_delay(unsigned int size, const T& data)
  : max_size_(size), recieved_timestamp_(0), recieved_signal_(BPROD_VALID),
    queue_(size,new bprod_storage_type<T>(data)) 
  {
  }

  //: Destructor
  virtual ~bprod_delay();

  //: Runs the process
  bprod_signal run(unsigned long timestamp,
                   bprod_debug_observer* const debug = NULL);

  //: Execute this process
  bprod_signal execute();
  
  //: Request data from dependents and shift the queue 
  void shift_queue(unsigned long timestamp,
                   bprod_debug_observer* const debug = NULL);

 private:
  unsigned max_size_;
  unsigned long recieved_timestamp_;
  bprod_signal recieved_signal_;
  vcl_deque<bprod_storage_sptr> queue_;

  //: mutex for updating the queue
  bprod_mutex queue_mutex_;

};


#endif // bprod_delay_h_
