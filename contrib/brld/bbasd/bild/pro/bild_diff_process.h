// This is basic/bild/pro/bild_diff_process.h
#ifndef bild_diff_process_h_
#define bild_diff_process_h_

//:
// \file
// \brief Process that computes absolute difference between images
// \author Based on original code by  Matt Leotta (@lems.brown.edu)
// \date 12/21/04
//
// \verbatim
//  Modifications
// \endverbatim

#include <bprod/bprod_process.h>
#include <bprod/bprod_process_factory.h>
#include <vcl_vector.h>
#include <vcl_string.h>

//: Process that computes absolute difference between images
class bild_diff_process : public bprod_filter {

public:
  //: Constructor
  bild_diff_process() {}
  //: Destructor
  virtual ~bild_diff_process() {}

  class factory : public bprod_process_factory
  {
    public:
    //: Return the default set of parameters for the process
      virtual bprod_parameters_sptr default_params() const;

    //: Construct a process from a set of parameters
      virtual bprod_process_sptr create(const bprod_parameters_sptr& params) const;

    //: The name of the process
      virtual vcl_string name() const { return "Image Difference"; }

      virtual ~factory() {}
  };


  //: Execute this process
  bprod_signal execute();

};

#endif
