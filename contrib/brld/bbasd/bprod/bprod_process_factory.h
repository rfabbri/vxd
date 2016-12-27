// This is basic/bprod/bprod_process_factory.h
#ifndef bprod_process_factory_h_
#define bprod_process_factory_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief A factory for creating processes from names and params
//
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 5/31/06
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------

#include <vcl_set.h>
#include <vcl_map.h>
#include <vcl_string.h>
#include <vbl/vbl_ref_count.h>

#include <bprod/bprod_parameters_sptr.h>
#include <bprod/bprod_process_sptr.h>


//: A factory for creating processes from names and params
class bprod_process_factory
{
  public:
  //: Return the default set of parameters for the process
  virtual bprod_parameters_sptr default_params() const = 0;

  //: Construct a process from a set of parameters
  virtual bprod_process_sptr create(const bprod_parameters_sptr& params) const = 0;

  //: The name of the process
  virtual vcl_string name() const = 0;

  typedef vcl_map<vcl_string, const bprod_process_factory*> Reg_Type;

  //: Return a const reference to the global registry of storage classes
  static Reg_Type const & registry() { return mut_registry(); }

  //: Return the default parameters for a process with the given name
  static bprod_parameters_sptr default_params(const vcl_string& name);

  //: Construct a process with the given name and parameters
  static bprod_process_sptr create(const vcl_string& name,
                                   const bprod_parameters_sptr& params);

  //: Create static instances of this struct to register a storage class
  struct registrar{
    registrar(const bprod_process_factory* example);
  };

  friend struct registrar;


 private:
  //: Return a reference to the global registry of storage classes
  static Reg_Type & mut_registry();

 protected:
  //: Copy Constructor
  bprod_process_factory(const bprod_process_factory& /*other*/) {}
  bprod_process_factory() {}
  virtual ~bprod_process_factory() {}
};



#endif // bprod_process_factory_h_
