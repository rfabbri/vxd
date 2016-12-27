// This is basic/dbpro/dbpro_process_factory.h
#ifndef dbpro_process_factory_h_
#define dbpro_process_factory_h_
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

#include <dbpro/dbpro_parameters_sptr.h>
#include <dbpro/dbpro_process_sptr.h>


//: A factory for creating processes from names and params
class dbpro_process_factory
{
  public:
  //: Return the default set of parameters for the process
  virtual dbpro_parameters_sptr default_params() const = 0;

  //: Construct a process from a set of parameters
  virtual dbpro_process_sptr create(const dbpro_parameters_sptr& params) const = 0;

  //: The name of the process
  virtual vcl_string name() const = 0;

  typedef vcl_map<vcl_string, const dbpro_process_factory*> Reg_Type;

  //: Return a const reference to the global registry of storage classes
  static Reg_Type const & registry() { return mut_registry(); }

  //: Return the default parameters for a process with the given name
  static dbpro_parameters_sptr default_params(const vcl_string& name);

  //: Construct a process with the given name and parameters
  static dbpro_process_sptr create(const vcl_string& name,
                                   const dbpro_parameters_sptr& params);

  //: Create static instances of this struct to register a storage class
  struct registrar{
    registrar(const dbpro_process_factory* example);
  };

  friend struct registrar;


 private:
  //: Return a reference to the global registry of storage classes
  static Reg_Type & mut_registry();

 protected:
  //: Copy Constructor
  dbpro_process_factory(const dbpro_process_factory& /*other*/) {}
  dbpro_process_factory() {}
  virtual ~dbpro_process_factory() {}
};



#endif // dbpro_process_factory_h_
