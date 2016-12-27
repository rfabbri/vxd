// This is basic/bprod/bprod_mutex.cxx

//:
// \file

#include <bprod/bprod_mutex.h>

#include <bprod/bprod_config.h>
#if BPROD_HAS_PTHREADS
#include <pthread.h>
#endif

//: private implementation struct for thread data
struct bprod_mutex::pimpl
{
public:
  pimpl()
  {
#if BPROD_HAS_PTHREADS
    pthread_mutex_init(&mutex_, NULL);
#endif
  }
  
  ~pimpl()
  {
#if BPROD_HAS_PTHREADS
    pthread_mutex_destroy(&mutex_);
#endif
  }
  

#if BPROD_HAS_PTHREADS
  pthread_mutex_t mutex_;
#endif
};


//: Default Constructor
bprod_mutex::bprod_mutex()
: data_(new pimpl())
{
}


//: Destructor
bprod_mutex::~bprod_mutex()
{
}


//: Lock the mutex
void bprod_mutex::lock()
{
#if BPROD_HAS_PTHREADS
  pthread_mutex_lock(&data_->mutex_);
#endif
}


//: Unlock the mutex
void bprod_mutex::unlock()
{
#if BPROD_HAS_PTHREADS
  pthread_mutex_unlock(&data_->mutex_);
#endif
}
