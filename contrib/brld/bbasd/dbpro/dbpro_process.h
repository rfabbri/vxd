// This is basic/dbpro/dbpro_process.h
#ifndef dbpro_process_h_
#define dbpro_process_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief The dbpro process base class
//
//  A process is an object that wraps an algorithm to create
//  standardized interface.
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 5/31/06
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------

#include <vcl_set.h>
#include <vcl_map.h>
#include <vcl_typeinfo.h>
#include <vbl/vbl_ref_count.h>


#include <dbpro/dbpro_fwd.h>
#include <dbpro/dbpro_mutex.h>
#include <dbpro/dbpro_storage.h>
#include <dbpro/dbpro_process_sptr.h>
#include <dbpro/dbpro_observer_sptr.h>
#include <dbpro/dbpro_connector_sptr.h>

class dbpro_debug_observer
{
public:
  virtual ~dbpro_debug_observer(){}
  //: called when first entering a process
  virtual void notify_enter(const dbpro_process* const pro, unsigned int timestamp) = 0;
  //: called just before exiting a process
  virtual void notify_exit(const dbpro_process* const pro, unsigned int timestamp) = 0;
  //: called just before executing the process (when all inputs are available)
  virtual void notify_pre_exec(const dbpro_process* const pro) = 0;
  //: called just after the execution finishes (before notifying observers)
  virtual void notify_post_exec(const dbpro_process* const pro) = 0;
  
  //: clone the debug observer 
  //  clones are made for each thread during a branch
  virtual dbpro_debug_observer* clone() const = 0;
  //: merge the data from another into this
  //  clones are merged when threads are joined threads
  virtual void merge(const dbpro_debug_observer& other) = 0;
};


//: This abstract class is the base class for process objects
//
// A process is a node in a data stream processing graph.  A process
// may be one of three types:
// - source
// - filter
// - sink
// A source process produces a stream of data with no input data stream.
// A filter process modifies one or more input streams to produce one or
// more output streams.  A sink process accepts an input stream but produces
// no output stream.  To create a new process derive a new class from
// dbpro_source, dbpro_filter, or dbpro_sink and implement the execute() method.
//
class dbpro_process : public vbl_ref_count
{
 public:

  //: Connect pin \p in_idx to the output pin \p out_idx of the process \p source
  void connect_input(unsigned int in_idx,
                     const dbpro_process_sptr& source, unsigned int out_idx);

  //: Disconnect pin \p in_idx
  void disconnect_input(unsigned int in_idx);

  //: Add an output observer on pin \p idx
  void add_output_observer(unsigned int idx, const dbpro_observer_sptr& obs);

  //: Remove an output observer on pin \p idx if it exists
  bool remove_output_observer(unsigned int idx, const dbpro_observer_sptr& obs);

  //: Runs the process
  virtual dbpro_signal run(unsigned long timestamp,
                           dbpro_debug_observer* const debug = NULL) = 0;

  //: Execute the process
  // derived classes should implement this
  virtual dbpro_signal execute() = 0;

  //: Return the last signal
  dbpro_signal last_signal() const;

 protected:
  //: Disable direct construction and destruction of the base class
  dbpro_process(const dbpro_process& other);
  dbpro_process();
  virtual ~dbpro_process();

  //: Clear data sitting on the input connectors
  void clear();
  //: Request any missing inputs
  dbpro_signal request_inputs(unsigned long timestamp,
                              dbpro_debug_observer* const debug = NULL);

  //: Request any missing inputs. Guaranteed to be serial (non-threaded).
  dbpro_signal request_inputs_serial(unsigned long timestamp,
                              dbpro_debug_observer* const debug = NULL);

  //: Notify the output observers
  // \param message is the default signal sent when data is missing
  // return false if data is not available for all observers
  bool notify_observers(dbpro_signal message = DBPRO_INVALID);

  //: Extract the data from an input pin assuming it exists with the correct type
  //  This should be called from the derived class execute call
  template <class T>
  const T& input(unsigned int idx)
  {
    assert(typeid(T) == get_input(idx)->type_id());
    return get_input(idx)->template data<T>();
  }

  //: Returns the typeinfo struct for the data type stored on the given pin
  const vcl_type_info& input_type_id(unsigned int idx) const;

  //: Wrap the output data in a storage class and set on an output pin
  //  This should be called from the derived class execute call
  template <class T>
  bool output(unsigned int idx, const T& data)
  {
    return set_output(idx,new dbpro_storage_type<T>(data));
  }

  //: Returns the number of observers of a particular output pin
  unsigned int num_observers(unsigned int idx) const;

  //: Returns the set of all pins that have observers
  vcl_set<unsigned int> observed_outputs() const;

  //: Access the storage class on the given input pin
  dbpro_storage_sptr get_input(unsigned int idx) const;

  //: Set the output storage class on the given pin
  // all observers of this output are notified
  bool set_output(unsigned int idx, const dbpro_storage_sptr& data);
  
  
  vcl_map<unsigned int, dbpro_connector_sptr> input_connectors_;
  vcl_map<unsigned int, dbpro_storage_sptr > output_data_;
  vcl_map<unsigned int, vcl_set<dbpro_observer_sptr> > output_observers_;
  
  //: the current timestamp
  unsigned long timestamp_;
  //: detects loops in data flow
  bool input_request_active_;
  //: the last signal returned
  dbpro_signal last_signal_;

  //: The mutex for updating (if using threads)
  dbpro_mutex update_mutex_;

};


//: Abstract base class for a filter process
// A filter has both input and output
class dbpro_filter : public dbpro_process
{
 public:
  //: Runs the process
  dbpro_signal run(unsigned long timestamp,
                   dbpro_debug_observer* const debug = NULL);

 protected:
  //: Disable direct construction and destruction of the base class
  dbpro_filter(const dbpro_process& other);
  dbpro_filter(){}
  virtual ~dbpro_filter(){}
};


//: Abstract base class for a source process
// A source provides output without any input
class dbpro_source : public dbpro_process
{
 public:
  //: Runs the process
  dbpro_signal run(unsigned long timestamp,
                   dbpro_debug_observer* const debug = NULL);

 protected:
  //: Disable direct construction and destruction of the base class
  dbpro_source(const dbpro_source& other);
  dbpro_source(){}
  virtual ~dbpro_source(){}
};


//: Abstract base class for a sink process
// A sink accepts input without producing output
class dbpro_sink : public dbpro_process
{
 public:
  //: Runs the process
  dbpro_signal run(unsigned long timestamp,
                   dbpro_debug_observer* const debug = NULL);

  //: The process will only run if this function returns true
  virtual bool enabled() const { return true; }

 protected:
  //: Disable direct construction and destruction of the base class
  dbpro_sink(const dbpro_sink& other);
  dbpro_sink(){}
  virtual ~dbpro_sink(){}
};



#endif // dbpro_process_h_
