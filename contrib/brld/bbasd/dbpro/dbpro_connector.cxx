// This is basic/dbpro/dbpro_connector.cxx

//:
// \file

#include <dbpro/dbpro_connector.h>
#include <dbpro/dbpro_process.h>


//: Transmit the data to the input of a process
bool
dbpro_connector::notify(const dbpro_storage_sptr& data,
                        unsigned long timestamp)
{
  data_ = data;
  timestamp_ = timestamp;
  return true;
}


//: disconnect from a process output pin
bool
dbpro_connector::unlink()
{
  if(source_){
    if(!source_->remove_output_observer(source_pin_,this))
      return false;
  }
  source_ = NULL;
  source_pin_ = 0;
  return true;
}

//: Connect this connector to the output of a process
bool
dbpro_connector::link(const dbpro_process_sptr& process, unsigned int pin)
{
  if(!unlink() || !process )
    return false;

  process->add_output_observer(pin,this);
  source_ = process.ptr();
  source_pin_ = pin;
  return true;
}


//: Make a request to the source for data
dbpro_signal
dbpro_connector::request_data(unsigned long timestamp,
                              dbpro_debug_observer* const debug) const
{
  if(!source_)
    return DBPRO_INVALID;
  dbpro_signal result = source_->run(timestamp,debug);
  if(result == DBPRO_VALID && !data_)
    return DBPRO_INVALID;
  return result;
}
