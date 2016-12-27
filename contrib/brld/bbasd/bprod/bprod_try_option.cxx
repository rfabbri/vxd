// This is basic/dbpro/dbpro_try_option.cxx

//:
// \file

#include "dbpro_try_option.h"
#include "dbpro_connector.h"
#include <vcl_iostream.h>

//: Runs the filter
dbpro_signal
dbpro_try_option::run(unsigned long timestamp,
                      dbpro_debug_observer* const debug)
{
  // notify the debugger if available
  if (debug) debug->notify_enter(this, timestamp);

  update_mutex_.lock();
  
  if(timestamp > this->timestamp_){

    this->timestamp_ = timestamp;

    if(this->input_request_active_){
      vcl_cerr << "Warning detected cycle in data flow" <<vcl_endl;
      return DBPRO_INVALID;
    }
    this->input_request_active_ = true;
    dbpro_signal retval = DBPRO_INVALID;
    typedef vcl_map<unsigned int, dbpro_connector_sptr >::iterator Itr;
    for(Itr i = input_connectors_.begin(); i != input_connectors_.end(); ++i)
    {
      dbpro_connector_sptr connector = i->second;
      if(!connector->data()){
        retval = connector->request_data(timestamp,debug);
        if(retval == DBPRO_INVALID)
          continue;
      }

      dbpro_signal m = connector->data()->info();
      if(m == DBPRO_INVALID){
        retval = m;
        continue;
      }

      set_output(0,connector->data());
      break;
    }
    this->input_request_active_ = false;

    this->notify_observers(retval);
    this->clear();
    this->last_signal_ = retval;

  }

  update_mutex_.unlock();


  // notify the debugger if available
  if (debug) debug->notify_exit(this, timestamp);

  return last_signal_;
}

//: Run the process on the current frame
dbpro_signal 
dbpro_try_option::execute()
{
  return DBPRO_INVALID;
}




