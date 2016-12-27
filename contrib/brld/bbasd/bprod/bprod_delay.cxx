// This is basic/dbpro/dbpro_delay.cxx

//:
// \file

#include "dbpro_delay.h"
#include <vcl_iostream.h>
#include <vcl_cstdlib.h>

#include <dbpro/dbpro_config.h>
#if DBPRO_HAS_PTHREADS
#include <pthread.h>
#endif


//: Constructor
dbpro_delay::dbpro_delay(unsigned int size) 
 : max_size_(size),
   recieved_timestamp_(0), 
   recieved_signal_(DBPRO_WAIT) 
{
}


//: Destructor
dbpro_delay::~dbpro_delay()
{
}


#if DBPRO_HAS_PTHREADS
// helper code to launch and manage pthreads
namespace{
  struct pthread_data
  {
    dbpro_delay* delay;
    unsigned long timestamp;
    dbpro_debug_observer* debug;
  };
  
  void* pthread_launcher(void *arg)
  {
    pthread_data *pd = reinterpret_cast<pthread_data*>(arg);
    pd->delay->shift_queue(pd->timestamp,pd->debug);
    delete pd->debug;
    delete pd;
  }
}
#endif 


//: Runs the filter
dbpro_signal
dbpro_delay::run(unsigned long timestamp,
                 dbpro_debug_observer* const debug)
{
  // notify the debugger if available
  if (debug) debug->notify_enter(this, timestamp);
  
  update_mutex_.lock();
  queue_mutex_.lock();

  if(timestamp > this->timestamp_){

    this->timestamp_ = timestamp;

    if(recieved_timestamp_ < timestamp-1){
      // get the previous round of data
      this->shift_queue(timestamp-1,debug);
    }
    this->last_signal_ = this->recieved_signal_;

    if(this->last_signal_ == DBPRO_WAIT)
      this->last_signal_ = DBPRO_VALID;


    for(unsigned int i=0; i<queue_.size(); ++i){
      set_output(i,queue_[i]);
    }
    if(!this->notify_observers(DBPRO_WAIT))
      this->last_signal_ = DBPRO_WAIT;
    
    queue_mutex_.unlock();
    
#if DBPRO_HAS_PTHREADS
    // launch a thread to get the next round of data
    pthread_data* pd = new pthread_data();
    pd->delay = this;
    pd->timestamp = timestamp;
    pd->debug = debug ? debug->clone() : NULL;
    pthread_t thread;
    int rc = pthread_create(&thread, NULL, pthread_launcher, (void*) pd );
    pthread_detach(thread);
    if(rc)
    {
      vcl_cerr << "Error creating pthread, return code: "<<rc<<vcl_endl;
      vcl_exit(-1);
    }
#else
    // get the next round of data
    this->shift_queue(timestamp,debug);
#endif
  }
  
  update_mutex_.unlock();

  // notify the debugger if available
  if (debug) debug->notify_exit(this, timestamp);

  return this->last_signal_;
}

//: Run the process on the current frame
dbpro_signal 
dbpro_delay::execute()
{
  return DBPRO_INVALID;
}


//: Request data from dependents and shift the queue  
void 
dbpro_delay::shift_queue(unsigned long timestamp,
                         dbpro_debug_observer* const debug)
{
  queue_mutex_.lock();
  this->recieved_signal_ = this->request_inputs(timestamp,debug);
  if( this->recieved_signal_ == DBPRO_VALID){
    dbpro_connector_sptr connector = input_connectors_.begin()->second;
    recieved_timestamp_ = connector->timestamp();
    assert(recieved_timestamp_ == timestamp);
    queue_.push_front(connector->data());
    while(queue_.size() > max_size_)
      queue_.pop_back();
    this->clear();
  }
  queue_mutex_.unlock();
}



