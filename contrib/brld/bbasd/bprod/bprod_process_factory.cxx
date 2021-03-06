// This is basic/bprod/bprod_process_factory.cxx

//:
// \file

#include <bprod/bprod_process_factory.h>
#include <vcl_iostream.h>



//: Return a reference to the global registry of storage classes
bprod_process_factory::Reg_Type &
bprod_process_factory::mut_registry()
{
  static Reg_Type reg;
  return reg;
}


//: The Constructor for this struct registers an exemplar storage object with the base class
bprod_process_factory::registrar::registrar(const bprod_process_factory* example)
{
  vcl_string name = example->name();
  // register with the base class
  Reg_Type& reg = bprod_process_factory::mut_registry();
  Reg_Type::iterator result = reg.find(name);
  if(result != reg.end()){
    delete result->second;
    vcl_cerr << "Warning multiple processes registered with the name: "<< name <<vcl_endl;
  }

  reg[name] = example;
}


//: Return the default parameters for a process with the given name
bprod_parameters_sptr
bprod_process_factory::default_params(const vcl_string& name)
{
  const Reg_Type& reg = bprod_process_factory::registry();
  Reg_Type::const_iterator result = reg.find(name);
  if(result == reg.end())
    return NULL;

  return result->second->default_params();
}


//: Construct a process with the given name and parameters
bprod_process_sptr
bprod_process_factory::create(const vcl_string& name,
                              const bprod_parameters_sptr& params)
{
  const Reg_Type& reg = bprod_process_factory::registry();
  Reg_Type::const_iterator result = reg.find(name);
  if(result == reg.end())
    return NULL;

  return result->second->create(params);
}
