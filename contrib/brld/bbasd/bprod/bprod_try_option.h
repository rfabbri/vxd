// This is basic/bprod/bprod_try_option.h
#ifndef bprod_try_option_h_
#define bprod_try_option_h_

//:
// \file
// \brief A filter that transmits the first non-failing input
// \author Matt Leotta
// \date 10/17/06
//
// \verbatim
//  Modifications
// \endverbatim


#include <bprod/bprod_process.h>
#include <vcl_deque.h>


//: A filter that transmits the first non-failing input
// This filter will request data from input pins sequentially.
// If a pin fails (produces invalid data) then the next pin is tried.
// As soon as a pin does not fail (sends a VALID, WAIT, EOF signal)
// the output from this pin is forwarded to output pin 0.  No further
// input requests are made on the remaining pins.  The output of this
// filter is invalid only if all inputs are invalid.
class bprod_try_option : public bprod_process
{
 public:

  //: Constructor
  bprod_try_option() {}


  //: Destructor
  virtual ~bprod_try_option() {}

  //: Runs the process
  bprod_signal run(unsigned long timestamp,
                   bprod_debug_observer* const debug = NULL);

  //: Execute this process
  bprod_signal execute();

};

#endif // bprod_try_option_h_
