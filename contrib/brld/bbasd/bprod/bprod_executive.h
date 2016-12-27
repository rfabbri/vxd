// This is basic/bprod/bprod_executive.h
#ifndef bprod_executive_h_
#define bprod_executive_h_

//:
// \file
// \brief An object to contain and execute a process graph
// \author Matt Leotta
// \date 6/7/06
//
// \verbatim
//  Modifications
// \endverbatim


#include <vcl_map.h>
#include <vcl_string.h>
#include <bprod/bprod_process.h>



//: An object to contain and execute a process graph
class bprod_executive 
{
 public:
   class proxy{
     public:
       friend class bprod_executive;
       //: Copy constructor
       proxy(const proxy& other) : exec(other.exec), name(other.name) {}
       operator bprod_process_sptr() const { return exec.find(name); }
       proxy& operator= (const bprod_process_sptr& p)
       { exec.pmap_[name] = p; return *this; }
       bprod_process* operator->() { return exec.find(name).as_pointer(); }
       bprod_process* ptr() { return exec.find(name).as_pointer(); }
     private:
       proxy(bprod_executive& e, const vcl_string& n)
       : exec(e), name(n) {}
       bprod_executive& exec;
       vcl_string name;
   };

   //: Constructor
   bprod_executive() : time_(0), debug_(NULL) {}

   //: Destructor
   ~bprod_executive() { delete debug_; }

  //: Accessor for assignment
  proxy operator[](const vcl_string& name) { return proxy(*this,name); }

  //: Accessor for const access
  bprod_process_sptr operator[](const vcl_string& name) const  { return find(name); }

  //: Initialize the process graph
  //: Call this after adding processes but before running
  void init();

  //: Run the entire graph and iterate until error or end of stream
  bprod_signal run_all() const;

  //: Run one step of the stream processing
  bprod_signal run_step() const;

  //: enable debug statements
  void enable_debug();
  
  //: Lookup a process name by process pointer
  vcl_string process_name(const bprod_process* const pro) const;

 private:
  //: Find a process by name or return NULL
  bprod_process_sptr find(const vcl_string& name) const;
  //: Map from names to processes
  vcl_map<vcl_string, bprod_process_sptr> pmap_;
  //: The set of sinks
  vcl_set<bprod_process_sptr> sinks_;

  //: The current time
  mutable unsigned long time_;

  bprod_debug_observer* debug_;


};

#endif // bprod_executive_h_
