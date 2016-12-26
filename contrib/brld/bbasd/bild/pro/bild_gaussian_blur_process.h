// This is basic/bild/pro/bild_gaussian_blur_process.h
#ifndef bild_gaussian_blur_process_h_
#define bild_gaussian_blur_process_h_

//:
// \file
// \brief A process that smooths an image with a Gaussian filter
// \author Based on original code by  Matt Leotta
// \date 12/20/04
//
// \verbatim
//  Modifications
// \endverbatim


#include <vcl_vector.h>
#include <vcl_string.h>
#include <dbpro/dbpro_process.h>
#include <dbpro/dbpro_process_factory.h>


class bild_gaussian_blur_process : public dbpro_filter
{
 public:
  class factory : public dbpro_process_factory
  {
   public:
    //: Return the default set of parameters for the process
    virtual dbpro_parameters_sptr default_params() const;

    //: Construct a process from a set of parameters
    virtual dbpro_process_sptr create(const dbpro_parameters_sptr& params) const;

    //: The name of the process
    virtual vcl_string name() const { return "Gaussian Blur"; }

    virtual ~factory() {}
  };

  //: Constructor
  bild_gaussian_blur_process(float sigma) : sigma_(sigma) {}

  //: Destructor
  virtual ~bild_gaussian_blur_process(){}


  //: Execute this process
  dbpro_signal execute();

 private:
  float sigma_;
};

#endif // bild_gaussian_blur_process_h_
