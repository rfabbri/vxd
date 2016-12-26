// This is basic/dbpro/dbpro_connector.h
#ifndef dbpro_connector_h_
#define dbpro_connector_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief An observer that connects a process output to a process input
//
//  Using this observer design pattern, this class implements the observers of
//  output from a process.  The process will notify it's output observers
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 5/30/06
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------


#include <dbpro/dbpro_observer.h>
#include <dbpro/dbpro_process.h>

//: An observer that connects a process output to a process input
class dbpro_connector : public dbpro_observer
{
 public:
  //: Constructor
  dbpro_connector() : data_(NULL), source_(NULL), source_pin_(0), timestamp_(0) {}

  //: Transmit the data to the input of a process
  bool notify(const dbpro_storage_sptr& data, unsigned long timestamp);

  //: Access the data
  dbpro_storage_sptr data() const { return data_; }

  //: Access the timestamp
  unsigned long timestamp() const { return timestamp_; }

  //: Disconnect from a process output pin
  bool unlink();

  //: Make a request to the source for data
  dbpro_signal request_data(unsigned long timestamp,
                            dbpro_debug_observer* const debug = NULL) const;

  //: Connect this connector to the output of a process
  bool link(const dbpro_process_sptr& process, unsigned int pin);

 private:
  dbpro_storage_sptr data_;
  dbpro_process* source_;
  unsigned int source_pin_;
  unsigned long timestamp_;
};




#endif // dbpro_connector_h_
