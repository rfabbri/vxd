// This is basic/dbpro/dbpro_mutex.cxx

//:
// \file

#include <dbpro/dbpro_mutex.h>

#include <dbpro/dbpro_config.h>
#if DBPRO_HAS_PTHREADS
#include <pthread.h>
#endif

//: private implementation struct for thread data
struct dbpro_mutex::pimpl
{
public:
  pimpl()
  {
#if DBPRO_HAS_PTHREADS
    pthread_mutex_init(&mutex_, NULL);
#endif
  }
  
  ~pimpl()
  {
#if DBPRO_HAS_PTHREADS
    pthread_mutex_destroy(&mutex_);
#endif
  }
  

#if DBPRO_HAS_PTHREADS
  pthread_mutex_t mutex_;
#endif
};


//: Default Constructor
dbpro_mutex::dbpro_mutex()
: data_(new pimpl())
{
}


//: Destructor
dbpro_mutex::~dbpro_mutex()
{
}


//: Lock the mutex
void dbpro_mutex::lock()
{
#if DBPRO_HAS_PTHREADS
  pthread_mutex_lock(&data_->mutex_);
#endif
}


//: Unlock the mutex
void dbpro_mutex::unlock()
{
#if DBPRO_HAS_PTHREADS
  pthread_mutex_unlock(&data_->mutex_);
#endif
}
