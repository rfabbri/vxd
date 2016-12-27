// This is basic/bprod/bprod_process.h
#ifndef bprod_process_h_
#define bprod_process_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief The bprod process base class
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


#include <bprod/bprod_fwd.h>
#include <bprod/bprod_mutex.h>
#include <bprod/bprod_storage.h>
#include <bprod/bprod_process_sptr.h>
#include <bprod/bprod_observer_sptr.h>
#include <bprod/bprod_connector_sptr.h>

class bprod_debug_observer
{
public:
  virtual ~bprod_debug_observer(){}
  //: called when first entering a process
  virtual void notify_enter(const bprod_process* const pro, unsigned int timestamp) = 0;
  //: called just before exiting a process
  virtual void notify_exit(const bprod_process* const pro, unsigned int timestamp) = 0;
  //: called just before executing the process (when all inputs are available)
  virtual void notify_pre_exec(const bprod_process* const pro) = 0;
  //: called just after the execution finishes (before notifying observers)
  virtual void notify_post_exec(const bprod_process* const pro) = 0;
  
  //: clone the debug observer 
  //  clones are made for each thread during a branch
  virtual bprod_debug_observer* clone() const = 0;
  //: merge the data from another into this
  //  clones are merged when threads are joined threads
  virtual void merge(const bprod_debug_observer& other) = 0;
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
// bprod_source, bprod_filter, or bprod_sink and implement the execute() method.
//
class bprod_process : public vbl_ref_count
{
 public:

  //: Connect pin \p in_idx to the output pin \p out_idx of the process \p source
  void connect_input(unsigned int in_idx,
                     const bprod_process_sptr& source, unsigned int out_idx);

  //: Disconnect pin \p in_idx
  void disconnect_input(unsigned int in_idx);

  //: Add an output observer on pin \p idx
  void add_output_observer(unsigned int idx, const bprod_observer_sptr& obs);

  //: Remove an output observer on pin \p idx if it exists
  bool remove_output_observer(unsigned int idx, const bprod_observer_sptr& obs);

  //: Runs the process
  virtual bprod_signal run(unsigned long timestamp,
                           bprod_debug_observer* const debug = NULL) = 0;

  //: Execute the process
  // derived classes should implement this
  virtual bprod_signal execute() = 0;

  //: Return the last signal
  bprod_signal last_signal() const;

 protected:
  //: Disable direct construction and destruction of the base class
  bprod_process(const bprod_process& other);
  bprod_process();
  virtual ~bprod_process();

  //: Clear data sitting on the input connectors
  void clear();
  //: Request any missing inputs
  bprod_signal request_inputs(unsigned long timestamp,
                              bprod_debug_observer* const debug = NULL);

  //: Request any missing inputs. Guaranteed to be serial (non-threaded).
  bprod_signal request_inputs_serial(unsigned long timestamp,
                              bprod_debug_observer* const debug = NULL);

  //: Notify the output observers
  // \param message is the default signal sent when data is missing
  // return false if data is not available for all observers
  bool notify_observers(bprod_signal message = BPROD_INVALID);

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
    return set_output(idx,new bprod_storage_type<T>(data));
  }

  //: Returns the number of observers of a particular output pin
  unsigned int num_observers(unsigned int idx) const;

  //: Returns the set of all pins that have observers
  vcl_set<unsigned int> observed_outputs() const;

  //: Access the storage class on the given input pin
  bprod_storage_sptr get_input(unsigned int idx) const;

  //: Set the output storage class on the given pin
  // all observers of this output are notified
  bool set_output(unsigned int idx, const bprod_storage_sptr& data);
  
  
  vcl_map<unsigned int, bprod_connector_sptr> input_connectors_;
  vcl_map<unsigned int, bprod_storage_sptr > output_data_;
  vcl_map<unsigned int, vcl_set<bprod_observer_sptr> > output_observers_;
  
  //: the current timestamp
  unsigned long timestamp_;
  //: detects loops in data flow
  bool input_request_active_;
  //: the last signal returned
  bprod_signal last_signal_;

  //: The mutex for updating (if using threads)
  bprod_mutex update_mutex_;

};


//: Abstract base class for a filter process
// A filter has both input and output
class bprod_filter : public bprod_process
{
 public:
  //: Runs the process
  bprod_signal run(unsigned long timestamp,
                   bprod_debug_observer* const debug = NULL);

 protected:
  //: Disable direct construction and destruction of the base class
  bprod_filter(const bprod_process& other);
  bprod_filter(){}
  virtual ~bprod_filter(){}
};


//: Abstract base class for a source process
// A source provides output without any input
class bprod_source : public bprod_process
{
 public:
  //: Runs the process
  bprod_signal run(unsigned long timestamp,
                   bprod_debug_observer* const debug = NULL);

 protected:
  //: Disable direct construction and destruction of the base class
  bprod_source(const bprod_source& other);
  bprod_source(){}
  virtual ~bprod_source(){}
};


//: Abstract base class for a sink process
// A sink accepts input without producing output
class bprod_sink : public bprod_process
{
 public:
  //: Runs the process
  bprod_signal run(unsigned long timestamp,
                   bprod_debug_observer* const debug = NULL);

  //: The process will only run if this function returns true
  virtual bool enabled() const { return true; }

 protected:
  //: Disable direct construction and destruction of the base class
  bprod_sink(const bprod_sink& other);
  bprod_sink(){}
  virtual ~bprod_sink(){}
};



#endif // bprod_process_h_
