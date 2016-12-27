// This is basic/bprod/bprod_connector.cxx

//:
// \file

#include <bprod/bprod_connector.h>
#include <bprod/bprod_process.h>


//: Transmit the data to the input of a process
bool
bprod_connector::notify(const bprod_storage_sptr& data,
                        unsigned long timestamp)
{
  data_ = data;
  timestamp_ = timestamp;
  return true;
}


//: disconnect from a process output pin
bool
bprod_connector::unlink()
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
bprod_connector::link(const bprod_process_sptr& process, unsigned int pin)
{
  if(!unlink() || !process )
    return false;

  process->add_output_observer(pin,this);
  source_ = process.ptr();
  source_pin_ = pin;
  return true;
}


//: Make a request to the source for data
bprod_signal
bprod_connector::request_data(unsigned long timestamp,
                              bprod_debug_observer* const debug) const
{
  if(!source_)
    return BPROD_INVALID;
  bprod_signal result = source_->run(timestamp,debug);
  if(result == BPROD_VALID && !data_)
    return BPROD_INVALID;
  return result;
}
